/*
 * xtransfer.c
 *
 * Created: 21.01.2023 13:44:59
 *  Author: maxbit89
 */ 

#include <stdbool.h>
#include <util/delay.h>
#include "xtransfer.h"
#include "../uart/uart.h"

void xtransfer_transmitHeader(int type, uint32_t size) {
	XTransfer_Header header;
	header.ioId[0] = 'X';
	header.ioId[1] = header.xId[1] = 'T';
	header.xId[0] = 'E';
	header.type = type;
	header.size = size;
	uart_putb((uint8_t*)&header, sizeof(header));;
}

void xtransfer_transmitData(uint8_t *data, uint32_t size) {
	uart_putb(data, size);
}

int32_t xtransfer_receiveHeader(XTransfer_Header *header, uint32_t timeoutMs) {
	uint32_t timeMs = 0;
	uint16_t recvd = 0;
	uint8_t recvdata = 0;
	uint32_t size = 0;
	if(header == NULL) {
		return XTRANSFER_STATE_ERR_INVALID_ARG;
	}
	header->ioId[0] = header->xId[0] = header->ioId[1] = header->xId[1] = 0;
	while(timeMs < timeoutMs) {
		recvd = uart_getc();
		if(recvd & UART_NO_DATA) {
			//Error Reciving Data
			timeMs++;
			_delay_ms(1);
		} else {
			timeMs = 0;
			recvdata = recvd & 0xff;
			if(size < sizeof(XTransfer_Header)) {
				((uint8_t*)header)[size] = recvdata;
				if(header->ioId[0] == 'X') {
					size++;
				} else {
					timeMs++;
					_delay_ms(1);
				}
			} else {
				if(header->ioId[0] == 'X' &&  header->ioId[1] == 'T' &&
				   header->xId[0] == 'E' &&  header->xId[1] == 'T') {
					//Header is valid
					if(header->size == 0) {
						return header->type;
					} else {
						return XTRANSFER_STATE_OK;;
					}
				} else {
					xtransfer_transmitHeader(XTRANSFER_STATE_ERR_RECV_HEADER, sizeof(XTransfer_Header));
					xtransfer_transmitData((uint8_t*)header, sizeof(XTransfer_Header));
					return XTRANSFER_STATE_ERR_RECV_HEADER;
				}
			}
		}
	}
	xtransfer_transmitHeader(XTRANSFER_STATE_ERR_TIMOUT, 1);
	uart_putc(0x1);
	return XTRANSFER_STATE_ERR_TIMOUT;
}

int32_t xtransfer_receiveData(uint8_t *data, uint32_t size, uint32_t timeoutMs) {
	uint32_t timeMs = 0;
	uint16_t recvd = 0;
	uint8_t recvdata = 0;
	uint32_t cSize = 0;
	if(data == NULL) {
		return XTRANSFER_STATE_ERR_INVALID_ARG;
	}
	while(timeMs < timeoutMs) {
		recvd = uart_getc();
		if(recvd & 0xff00) {
			//Error Reciving Data
			timeMs++;
			_delay_ms(1);
		} else {
			timeMs = 0;
			recvdata = recvd & 0xff;
			if(cSize < size) {
				data[cSize] = recvdata;
				cSize++;
			} else {
				return XTRANSFER_STATE_OK;;
			}
		}
	}
	xtransfer_transmitHeader(XTRANSFER_STATE_ERR_TIMOUT, 1);
	uart_putc(0x02);
	return XTRANSFER_STATE_ERR_TIMOUT;
}