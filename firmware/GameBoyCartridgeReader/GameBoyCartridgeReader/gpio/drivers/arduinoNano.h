
/*
 * arduinoNano.h
 *
 * Created: 06.01.2023 20:47:51
 *  Author: maxbit89
 */ 


#ifndef ARDUINONANO_H_
#define ARDUINONANO_H_

#define GPIO_ARDUINO_ENABLE_ARUDINO_PIN_DECLARATION

#include "../gpio.h"

#define GPIO_ARDUINO_PORT_B (uint8_t) 0
#define GPIO_ARDUINO_PORT_C (uint8_t) 1
#define GPIO_ARDUINO_PORT_D (uint8_t) 2

#define GPIO_ARDUINO_PIN_0 (uint8_t) 0
#define GPIO_ARDUINO_PIN_1 (uint8_t) 1
#define GPIO_ARDUINO_PIN_2 (uint8_t) 2
#define GPIO_ARDUINO_PIN_3 (uint8_t) 3
#define GPIO_ARDUINO_PIN_4 (uint8_t) 4
#define GPIO_ARDUINO_PIN_5 (uint8_t) 5
#define GPIO_ARDUINO_PIN_6 (uint8_t) 6
#define GPIO_ARDUINO_PIN_7 (uint8_t) 7

extern Gpio_Handle gpio_arduino;

#ifdef GPIO_ARDUINO_ENABLE_ARUDINO_PIN_DECLARATION
	//Arduino Helper Defines used as multi arg to gpio.c functions
	extern Gpio_Pin D0;
	extern Gpio_Pin D1;
	extern Gpio_Pin D2;
	extern Gpio_Pin D3;
	extern Gpio_Pin D4;
	extern Gpio_Pin D5;
	extern Gpio_Pin D6;
	extern Gpio_Pin D7;
	extern Gpio_Pin D8;
	extern Gpio_Pin D9;
	extern Gpio_Pin D10;
	extern Gpio_Pin D11;
	extern Gpio_Pin D12;
	extern Gpio_Pin D13;

	extern Gpio_Pin A0;
	extern Gpio_Pin A1;
	extern Gpio_Pin A2;
	extern Gpio_Pin A3;
	extern Gpio_Pin A4;
	extern Gpio_Pin A5;
	extern Gpio_Pin A6;
	extern Gpio_Pin A7;
#endif


#endif /* ARDUINONANO_H_ */