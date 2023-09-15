/*
 * template.c
 *
 * Created: 07.01.2023 00:33:34
 *  Author: maxbit89
 */ 

#include "template.h"

//pinMux API
static Gpio_Status _gpio_template_init(uint8_t gpioId, uint8_t pinId, Gpio_Pin_Mode);

//read API
static Gpio_Status _gpio_template_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value);
static Gpio_Status _gpio_template_readU8(uint8_t gpioId, uint8_t* value);
static Gpio_Status _gpio_template_readU16(uint8_t gpioId, uint16_t* value);

//write API
static Gpio_Status _gpio_template_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value);
static Gpio_Status _gpio_template_writeU8(uint8_t gpioId, uint8_t value);
static Gpio_Status _gpio_template_writeU16(uint8_t gpioId, uint16_t value);

Gpio_Handle gpio_template = {
	//pinMux API
	.driver.init = _gpio_template_init,
	
	//read API
	.driver.readPin = _gpio_template_readPin,
	.driver.readU8  = _gpio_template_readU8,
	.driver.readU16 = _gpio_template_readU16,
	
	//write API
	.driver.writePin = _gpio_template_writePin,
	.driver.writeU8  = _gpio_template_writeU8,
	.driver.writeU16 = _gpio_template_writeU16
};

//pinMux API
static Gpio_Status _gpio_template_init(uint8_t gpioId, uint8_t pinId, Gpio_Pin_Mode pinMode) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}

//read API
static Gpio_Status _gpio_template_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
static Gpio_Status _gpio_template_readU8(uint8_t gpioId, uint8_t* value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
static Gpio_Status _gpio_template_readU16(uint8_t gpioId, uint16_t* value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}

//write API
static Gpio_Status _gpio_template_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
static Gpio_Status _gpio_template_writeU8(uint8_t gpioId, uint8_t value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
static Gpio_Status _gpio_template_writeU16(uint8_t gpioId, uint16_t value) {
	//Template dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}