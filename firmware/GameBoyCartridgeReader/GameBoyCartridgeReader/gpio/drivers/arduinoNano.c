/*
 * arduinoNano.c
 *
 * Created: 06.01.2023 20:48:05
 *  Author: maxbit89
 */ 

#include <avr/io.h>
#include "arduinoNano.h"
#include "../../uart/uart.h"

#ifdef GPIO_ARDUINO_ENABLE_ARUDINO_PIN_DECLARATION
//Arduino Helper Defines used as multi arg to gpio.c functions
Gpio_Pin D0  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_0};
Gpio_Pin D1  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_1};
Gpio_Pin D2  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_2};
Gpio_Pin D3  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_3};
Gpio_Pin D4  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_4};
Gpio_Pin D5  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_5};
Gpio_Pin D6  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_6};
Gpio_Pin D7  = {{&gpio_arduino, GPIO_ARDUINO_PORT_D}, GPIO_ARDUINO_PIN_7};
Gpio_Pin D8  = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_0};
Gpio_Pin D9  = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_1};
Gpio_Pin D10 = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_2};
Gpio_Pin D11 = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_3};
Gpio_Pin D12 = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_4};
Gpio_Pin D13 = {{&gpio_arduino, GPIO_ARDUINO_PORT_B}, GPIO_ARDUINO_PIN_5};

Gpio_Pin A0 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_0};
Gpio_Pin A1 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_1};
Gpio_Pin A2 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_2};
Gpio_Pin A3 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_3};
Gpio_Pin A4 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_4};
Gpio_Pin A5 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_5};
Gpio_Pin A6 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_6};
Gpio_Pin A7 = {{&gpio_arduino, GPIO_ARDUINO_PORT_C}, GPIO_ARDUINO_PIN_7};
#endif

typedef struct {
	uint8_t* port;
	uint8_t* portIn;
	uint8_t* ddr;
} GpioRegs;

static Gpio_Status cfgPort(GpioRegs* regs, uint8_t pinId, Gpio_Pin_Mode mode);
static Gpio_Status getRegsFromGpioId(uint8_t gpioId, GpioRegs* regs);

//pinMux API
static Gpio_Status _gpio_arduino_init(uint8_t gpioId, uint8_t pinId, Gpio_Pin_Mode);

//read API
static Gpio_Status _gpio_arduino_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value);
static Gpio_Status _gpio_arduino_readU8(uint8_t gpioId, uint8_t* value);

//write API
static Gpio_Status _gpio_arduino_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value);
static Gpio_Status _gpio_arduino_writeU8(uint8_t gpioId, uint8_t value);

Gpio_Handle gpio_arduino = {
	//pinMux API
	.driver.init = _gpio_arduino_init,
		
	//read API
	.driver.readPin = _gpio_arduino_readPin,
	.driver.readU8  = _gpio_arduino_readU8,
	.driver.readU16 = GPIO_METHOD_NOTSUPPORTED,
			
	//write API
	.driver.writePin = _gpio_arduino_writePin,
	.driver.writeU8  = _gpio_arduino_writeU8,
	.driver.writeU16 = GPIO_METHOD_NOTSUPPORTED
};

static Gpio_Status cfgPort(GpioRegs* regs, uint8_t pinId, Gpio_Pin_Mode mode) {
	Gpio_Status status = GPIO_STATUS_INVALID_MODE;
	switch(mode) {
		case GPIO_PIN_MODE_INPUT_PULLUP:
			*regs->port |= (1<<pinId); //Set Pin Value to 1
			//Drop through
		case GPIO_PIN_MODE_INPUT:
			*regs->ddr &= ~(1<<pinId); //Set Pin Direction to Input
			status = GPIO_STATUS_OK;
			break;
		case GPIO_PIN_MODE_OUTPUT:
			*regs->port &= ~(1<<pinId); //Set Pin Value to 0
			*regs->ddr |=  (1<<pinId);  //Set Pin Direction to Output
			status = GPIO_STATUS_OK;
			break;
	}
	return status;
}

static Gpio_Status getRegsFromGpioId(uint8_t gpioId, GpioRegs* regs) {
		switch(gpioId) {
			case GPIO_ARDUINO_PORT_B:
				regs->port = (uint8_t*)&PORTB;
				regs->portIn = (uint8_t*)&PINB;
				regs->ddr = (uint8_t*)&DDRB;
				break;
			case GPIO_ARDUINO_PORT_C:
				regs->port = (uint8_t*)&PORTC;
				regs->portIn = (uint8_t*)&PINC;
				regs->ddr = (uint8_t*)&DDRC;
				break;
			case GPIO_ARDUINO_PORT_D:
				regs->port = (uint8_t*)&PORTD;
				regs->portIn = (uint8_t*)&PIND;
				regs->ddr = (uint8_t*)&DDRD;
				break;
			default:
				return GPIO_STATUS_INVALID_ID;
		}
		return GPIO_STATUS_OK;
}

//pinMux API
static Gpio_Status _gpio_arduino_init(uint8_t gpioId, uint8_t pinId, Gpio_Pin_Mode pinMode) {
	GpioRegs regs;
	Gpio_Status status = getRegsFromGpioId(gpioId, &regs);
	if(status == GPIO_STATUS_OK) {
		status = cfgPort(&regs, pinId, pinMode);
	}
	return status;
}
	
//read API
static Gpio_Status _gpio_arduino_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value) {
	GpioRegs regs;
	Gpio_Status status = getRegsFromGpioId(gpioId, &regs);
	if(status == GPIO_STATUS_OK) {
		*value = (*regs.portIn) & (1<<pinId) ? true : false;
	}
	return status;
}
static Gpio_Status _gpio_arduino_readU8(uint8_t gpioId, uint8_t* value) {
	GpioRegs regs;
	Gpio_Status status = getRegsFromGpioId(gpioId, &regs);
	if(status == GPIO_STATUS_OK) {
		*value = *regs.portIn;
	}
	return status;
}
	
//write API
static Gpio_Status _gpio_arduino_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value) {
	GpioRegs regs;
	Gpio_Status status = getRegsFromGpioId(gpioId, &regs);
	if(status == GPIO_STATUS_OK) {
		if(value) {
			*regs.port |= (1<<pinId);
		} else {
			*regs.port &= ~(1<<pinId);
		}
	}
	return status;
}
static Gpio_Status _gpio_arduino_writeU8(uint8_t gpioId, uint8_t value) {
	GpioRegs regs;
	Gpio_Status status = getRegsFromGpioId(gpioId, &regs);
	if(status == GPIO_STATUS_OK) {
		*regs.port = value;
	}
	return status;
}