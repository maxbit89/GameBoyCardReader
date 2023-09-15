/*
 * gpio.c
 *
 * Created: 06.01.2023 20:46:40
 *  Author: maxbit89
 */ 

#include "gpio.h"

Gpio_Status gpioInit(Gpio_Pin *pin, Gpio_Pin_Mode pinMode) {
	if (pin && pin->port.handle->driver.init) {
		return pin->port.handle->driver.init(pin->port.gpioId, pin->pinId, pinMode);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioReadPin(Gpio_Pin *pin, uint8_t* value) {
	if (pin && pin->port.handle->driver.readPin) {
		return pin->port.handle->driver.readPin(pin->port.gpioId, pin->pinId, value);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioReadU8(Gpio_Handle* handle, uint8_t gpioId, uint8_t* value) {
	if (handle->driver.readU8) {
		return handle->driver.readU8(gpioId, value);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioReadU16(Gpio_Handle* handle, uint8_t gpioId, uint16_t* value) {
	if (handle->driver.readU16) {
		return handle->driver.readU16(gpioId, value);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioWritePin(Gpio_Pin *pin, uint8_t value) {
	if (pin && pin->port.handle->driver.writePin) {
		return pin->port.handle->driver.writePin(pin->port.gpioId, pin->pinId, value ? true : false);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioWriteU8(Gpio_Handle* handle, uint8_t gpioId, uint8_t value) {
	if (handle->driver.writeU8) {
		return handle->driver.writeU8(gpioId, value);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
Gpio_Status gpioWriteU16(Gpio_Handle* handle, uint8_t gpioId, uint16_t value) {
	if (handle->driver.writeU16) {
		return handle->driver.writeU16(gpioId, value);
	}
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}