/*
 * hcf40999be.c
 *
 * Created: 06.01.2023 20:58:14
 *  Author: maxbit89
 */ 

#include "hcf4099be.h"
#include <util/delay.h>

//read API
/* TODO
static Gpio_Status _gpio_hcf4099be_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value);
static Gpio_Status _gpio_hcf4099be_readU8(uint8_t gpioId, uint8_t* value);
*/

//write API
/* TODO
static Gpio_Status _gpio_hcf4099be_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value);
*/
static Gpio_Status _gpio_hcf4099be_writeU8(uint8_t gpioId, uint8_t value);

Gpio_Handle gpio_hcf4099be = {
	//pinMux API
	.driver.init = NULL,
	
	//read API
	.driver.readPin = NULL,
	.driver.readU8  = NULL,
	.driver.readU16 = NULL,
	
	//write API
	.driver.writePin = NULL,
	.driver.writeU8  = _gpio_hcf4099be_writeU8,
	.driver.writeU16 = NULL
};

Hcf4099be_Handle hcf4099be_handles[HCF4099BE_MAX_INSTANCES] = {0};
	
static uint8_t newInstance() {
	uint8_t n;
	for(n=0;n<sizeof(hcf4099be_handles)/sizeof(Hcf4099be_Handle);n++) {
		if(hcf4099be_handles[n].active == 0) {
			hcf4099be_handles[n].active = true;
			return n;
		}
	}
	return 0xff;
}

//Returns gpioId
uint8_t hcf4099be_create(Gpio_Pin* data, Gpio_Pin* writede, Gpio_Pin* read, Gpio_Pin* a0, Gpio_Pin* a1, Gpio_Pin* a2) {
	uint8_t gpioId = newInstance();
	if(gpioId < 0xff) {
		hcf4099be_handles[gpioId].latchIO.data = data;
		hcf4099be_handles[gpioId].latchIO.writede = writede;
		hcf4099be_handles[gpioId].latchIO.read = read;
		hcf4099be_handles[gpioId].latchIO.a0 = a0;
		hcf4099be_handles[gpioId].latchIO.a1 = a1;
		hcf4099be_handles[gpioId].latchIO.a2 = a2;
		
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.data, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.writede, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.read, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.a0, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.a1, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioInit(hcf4099be_handles[gpioId].latchIO.a2, GPIO_PIN_MODE_OUTPUT))) goto hcf4099be_create_error;
		
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.data, false))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.writede, true))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.read, false))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.a0, false))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.a1, false))) goto hcf4099be_create_error;
		if(GPIO_STATUS_OK != (gpioWritePin(hcf4099be_handles[gpioId].latchIO.a2, false))) goto hcf4099be_create_error;
	}
	return gpioId;
hcf4099be_create_error:
	hcf4099be_handles[gpioId].active = false;
	return 0xff;
}

//read API
/*
static Gpio_Status _gpio_hcf4099be_readPin(uint8_t gpioId, uint8_t pinId, uint8_t* value) {
	//hcf4099be dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
static Gpio_Status _gpio_hcf4099be_readU8(uint8_t gpioId, uint8_t* value) {
	//hcf4099be dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
*/

//write API
/*
static Gpio_Status _gpio_hcf4099be_writePin(uint8_t gpioId, uint8_t pinId, uint8_t value) {
	//hcf4099be dummy
	return GPIO_STATUS_METHOD_NOT_SUPPORTED;
}
*/
static Gpio_Status _gpio_hcf4099be_writeU8(uint8_t gpioId, uint8_t value) {
	Gpio_Status status = GPIO_STATUS_OK;
	int n;
	int addr = 0;
	if(gpioId > sizeof(hcf4099be_handles)/sizeof(Hcf4099be_Handle)) {
		return GPIO_STATUS_INVALID_ID;
	}
	for (n = 1; n < 0xff; n = n << 1) {
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.a0, addr & 0x01)) != GPIO_STATUS_OK) return status;
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.a1, addr & 0x02)) != GPIO_STATUS_OK) return status;
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.a2, addr & 0x04)) != GPIO_STATUS_OK) return status;
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.data, value & n)) != GPIO_STATUS_OK) return status;
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.writede, false)) != GPIO_STATUS_OK) return status;
		_delay_ms(1);
		if((status = gpioWritePin(hcf4099be_handles[gpioId].latchIO.writede, true)) != GPIO_STATUS_OK) return status;
		addr++;
	}
	return status;
}