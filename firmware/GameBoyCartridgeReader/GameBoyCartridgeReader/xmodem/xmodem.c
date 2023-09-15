

#include <util/delay.h>
#include <string.h>
#include "crc16.h"
#include "../uart/uart.h"
#include "xmodem.h"

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define TIME_S_AS_MS 1000
#define MAXRETRANS 25

static int uart_getc_t(int timeout_ms);
static int check(int crc, const uint8_t *data, int size);

static int check(int crc, const uint8_t *data, int len) {
	if (crc) {
		uint16_t crc = crc16(data, len);
		uint16_t tcrc = (data[len]<<8)+data[len+1];
		if (crc == tcrc) return 1;
	} else {
		int i;
		uint8_t cks = 0;
		for (i = 0; i < len; i++) {
			cks += data[i];
		}
		if (cks == data[len]) return 1;
	}

	return 0;
}

static int uart_getc_t(int timeout_ms) {
	int data = 0;
	int delay = timeout_ms;

	while ((data=uart_getc()) == UART_NO_DATA) {
		_delay_ms(1);
		if (timeout_ms) {
			if (--delay == 0)
			return -2;
		}
	}

	return (uint8_t)(data & 0xff);
}


static int xmodemRecvedFragment(uint8_t *buffer, size_t offset, size_t len) {
	return 0;
}

static void flushinput(void) {
	while (uart_getc_t(((TIME_S_AS_MS)*3)>>1) >= 0)
	;
}

int xmodemReceive(size_t maxlen, XModem_OnRecvFragment onRecvFragment) {
	uint8_t xbuff[1030]; /* 1024 = XModem 1k, 3 = head, 2 = crc, 1 = null terminator */
	uint8_t *p;
	size_t xbuffSize;
	size_t i;
	size_t len = 0;
	uint8_t crc = false;
	uint8_t trychar = 'C';
	uint8_t packetno = 1;
	int c;
	int retry;
	int retrans = MAXRETRANS;

	while(1) {
		for( retry = 0; retry < 16; retry++) {
			if (trychar) uart_putc(trychar);
			if ((c = uart_getc_t(2 * TIME_S_AS_MS)) >= 0) {
				switch (c) {
					case SOH:
						xbuffSize = 128;
						goto start_recv;
					case STX:
						xbuffSize = 1024;
						goto start_recv;
					case EOT:
						flushinput();
						uart_putc(ACK);
						return len; /* normal end */
					case CAN:
						if ((c = uart_getc_t(TIME_S_AS_MS)) == CAN) {
							flushinput();
							uart_putc(ACK);
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		if (trychar == 'C') {
			trychar = NAK;
			continue;
		}
		flushinput();
		uart_putc(CAN);
		uart_putc(CAN);
		uart_putc(CAN);
		return -2; /* sync error */

start_recv:
		if (trychar == 'C')
			crc = true;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (xbuffSize+crc+3); i++) {
			if ((c = uart_getc_t(TIME_S_AS_MS)) < 0)
				goto reject;
			*p++ = c;
		}

		if (xbuff[1] == (uint8_t)(~xbuff[2]) && (xbuff[1] == packetno || xbuff[1] == (uint8_t)packetno-1) && check(crc, &xbuff[3], xbuffSize)) {
			if (xbuff[1] == packetno)	{
				int count = maxlen - len;
				if (count > xbuffSize) count = xbuffSize;
				if (count > 0) {
					if(xmodemRecvedFragment(&(xbuff[3]), len, count) != 0) {
						return -6; //Save data fragment to Storeage failed
					}
					len += count;
				}
				packetno++;
				retrans = MAXRETRANS + 1;
			}
			if (--retrans <= 0) {
				flushinput();
				uart_putc(CAN);
				uart_putc(CAN);
				uart_putc(CAN);
				return -3; /* too many retry error */
			}
			uart_putc(ACK);
			continue;
		}
reject:
		flushinput();
		uart_putc(NAK);
	}
}

int xmodemTransmit(size_t srcsz, XModem_OnTransmitFragment onTransmitFragment)
{
	uint8_t xbuff[1030]; /* 1024 = XModem 1k, 3 = head, 2 = crc, 1 = null terminator */
	uint8_t xbuffSize;
	uint8_t crc = true;
	uint8_t packetno = 1;
	int i;
	int c;
	int count;
	int len = 0;
	int retry;

	while(1) {
		for( retry = 0; retry < 16; retry++) {
			if ((c = uart_getc_t(2 * TIME_S_AS_MS)) >= 0) {
				switch (c) {
					case 'C':
						crc = true;
						goto start_trans;
					case NAK:
						crc = false;
						goto start_trans;
					case CAN:
						if ((c = uart_getc_t(TIME_S_AS_MS)) == CAN) {
							uart_putc(ACK);
							flushinput();
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		uart_putc(CAN);
		uart_putc(CAN);
		uart_putc(CAN);
		flushinput();
		return -2; /* no sync */

		while(1) {
start_trans:
			xbuff[0] = SOH;
			xbuffSize = 128;
			xbuff[1] = packetno;
			xbuff[2] = ~packetno;
			count = srcsz - len;
			if (count > xbuffSize) {
				count = xbuffSize;
			}
				
			if (c >= 0) {
				memset (&xbuff[3], 0, xbuffSize);
				if (c == 0) {
					xbuff[3] = CTRLZ;
				} else {
					if(onTransmitFragment(&(xbuff[3]), len, count) != 0) {
						return -6; //Get data fragment from Storeage failed
					}
//					memcpy (&xbuff[3], &src[len], count);
					if (count < xbuffSize) xbuff[3+count] = CTRLZ;
				}
				if (crc) {
					uint16_t ccrc = crc16(&xbuff[3], xbuffSize);
					xbuff[xbuffSize+3] = (ccrc>>8) & 0xFF;
					xbuff[xbuffSize+4] = ccrc & 0xFF;
				} else {
					uint8_t ccks = 0;
					for (i = 3; i < xbuffSize+3; i++) {
						ccks += xbuff[i];
					}
					xbuff[xbuffSize+3] = ccks;
				}
				for (retry = 0; retry < MAXRETRANS; retry++) {
					for (i = 0; i < xbuffSize+4+(crc?1:0); i++) {
						uart_putc(xbuff[i]);
					}
					if ((c = uart_getc_t(TIME_S_AS_MS)) >= 0 ) {
						switch (c) {
							case ACK:
								packetno++;
								len += xbuffSize;
								goto start_trans;
							case CAN:
								if ((c = uart_getc_t(TIME_S_AS_MS)) == CAN) {
									uart_putc(ACK);
									flushinput();
									return -1; /* canceled by remote */
								}
								break;
							case NAK:
							default:
								break;
						}
					}
				}
				uart_putc(CAN);
				uart_putc(CAN);
				uart_putc(CAN);
				flushinput();
				return -4; /* transmit error */
			} else {
				for (retry = 0; retry < 10; retry++) {
					uart_putc(EOT);
					if ((c = uart_getc_t(2 * TIME_S_AS_MS)) == ACK)
						break;
				}
				flushinput();
				return (c == ACK) ? len : -5;
			}
		}
	}
}