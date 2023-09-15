/*
 * hcf4099be.h
 *
 * Created: 06.01.2023 20:58:33
 *  Author: maxbit89
 */ 


#ifndef HCF4099BE_H_
#define HCF4099BE_H_

#define HCF4099BE_MAX_INSTANCES 2

#include "../gpio.h"

typedef struct {
	uint8_t active;
	struct {
		Gpio_Pin* data;
		Gpio_Pin* writede;
		Gpio_Pin* read;
		Gpio_Pin* a0;
		Gpio_Pin* a1;
		Gpio_Pin* a2;
	} latchIO;
} Hcf4099be_Handle;

extern 	Gpio_Handle gpio_hcf4099be;

uint8_t hcf4099be_create(Gpio_Pin* data, Gpio_Pin* writede, Gpio_Pin* read, Gpio_Pin* a0, Gpio_Pin* a1, Gpio_Pin* a2);

#endif /* HCF4099BE_H_ */