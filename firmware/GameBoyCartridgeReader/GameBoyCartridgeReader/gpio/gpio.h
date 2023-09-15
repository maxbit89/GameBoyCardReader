/*
 * gpio.h
 *
 * Created: 06.01.2023 20:46:53
 *  Author: maxbit89
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define GPIO_METHOD_NOTSUPPORTED NULL

typedef enum {
	GPIO_PIN_MODE_INPUT        = 0,
	GPIO_PIN_MODE_INPUT_PULLUP = 1,
	GPIO_PIN_MODE_OUTPUT       = 2
} Gpio_Pin_Mode;

typedef enum {
	GPIO_STATUS_OK = 0,
	GPIO_STATUS_INVALID_ID = -1,
	GPIO_STATUS_INVALID_MODE = -2,
	GPIO_STATUS_METHOD_NOT_SUPPORTED = -3
} Gpio_Status;

typedef struct {
	//pinMux API
	Gpio_Status (*init)(uint8_t gpioId, uint8_t pinId, Gpio_Pin_Mode);
	
	//read API
	Gpio_Status (*readPin)(uint8_t gpioId, uint8_t pinId, uint8_t* value);
	Gpio_Status (*readU8)(uint8_t gpioId, uint8_t* value);
	Gpio_Status (*readU16)(uint8_t gpioId, uint16_t* value);
	
	//write API
	Gpio_Status (*writePin)(uint8_t gpioId, uint8_t pinId, uint8_t value);
	Gpio_Status (*writeU8)(uint8_t gpioId, uint8_t value);
	Gpio_Status (*writeU16)(uint8_t gpioId, uint16_t value);
} Gpio_Driver;

typedef struct Gpio_Handle {
	Gpio_Driver driver;
} Gpio_Handle;

typedef struct {
	Gpio_Handle* handle;
	uint8_t gpioId;
} Gpio_Port;

typedef struct Gpio_Pin {
	Gpio_Port port;
	uint8_t pinId;
} Gpio_Pin;

Gpio_Status gpioInit(Gpio_Pin *pin, Gpio_Pin_Mode pinMode);
Gpio_Status gpioReadPin(Gpio_Pin *pin, uint8_t* value);
Gpio_Status gpioReadU8(Gpio_Handle* handle, uint8_t gpioId, uint8_t* value);
Gpio_Status gpioReadU16(Gpio_Handle* handle, uint8_t gpioId, uint16_t* value);
Gpio_Status gpioWritePin(Gpio_Pin *pin, uint8_t value);
Gpio_Status gpioWriteU8(Gpio_Handle* handle, uint8_t gpioId, uint8_t value);
Gpio_Status gpioWriteU16(Gpio_Handle* handle, uint8_t gpioId, uint16_t value);

/*
 * Enabled Drivers:
 */
#include "drivers/arduinoNano.h"
#include "drivers/hcf4099be.h"

#endif /* GPIO_H_ */