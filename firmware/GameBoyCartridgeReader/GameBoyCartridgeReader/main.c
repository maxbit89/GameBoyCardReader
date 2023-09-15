/*
 * main.c
 *
 * Created: 1/6/2023 8:44:41 PM
 *  Author: maxbit89
 */ 
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#include "gbCartridge/gbCartridge.h"
#include "uart/uart.h"
#include "xtransfer/xcom.h"


GB_CartridgeSlot slot = {
	.addr_low = {0},
	.addr_high = {0},
	.d0 = &A0,
	.d1 = &A1,
	.d2 = &A2,
	.d3 = &A3,
	.d4 = &A4,
	.d5 = &A5,
	.d6 = &D11,
	.d7 = &D12,
	.write = &D10,
	.read = &D9
};

GB_RomInfo romInfo;

#define UART_BAUD_RATE      9600

int main(void)
{
	int state = 0;
	_delay_ms(100); //delay to give prevent problems with avrdude
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	
	slot.addr_low.handle = &gpio_hcf4099be;
	slot.addr_low.gpioId = hcf4099be_create(&D5, &D7, &D6, &D2, &D3, &D4);
	slot.addr_high.handle = &gpio_hcf4099be;
	slot.addr_high.gpioId = hcf4099be_create(&D5, &D8, &D6, &D2, &D3, &D4);
	gbCartridge_init(&slot);
	
	gpioInit(&D13, GPIO_PIN_MODE_OUTPUT);
	gpioWritePin(&D13, false);
	
    while(1)
    {
		if(gbCartridge_isPressent(&slot) && gbCartridge_isChecksumValid(&slot)) {
			gpioWritePin(&D13, true);
			gbCartridge_getRomInfo(&slot, &romInfo);
			state = xcom_cartridge_detect(&romInfo);
			if(state < 0) {
				_delay_ms(500);
			} else {
				state = xcom_do_cartridgeAction(&slot, &romInfo, 10000);
				if(state < 0) {
					//Drain uart:
					_delay_ms(500);
					while(!(uart_getc() & UART_NO_DATA));
				}
			}
		} else {
			gpioWritePin(&D13, false);
		}
    }
	_delay_ms(500);
}