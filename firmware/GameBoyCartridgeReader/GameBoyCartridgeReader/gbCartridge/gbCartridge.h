/*
 * gbCartridge.h
 *
 * Created: 07.01.2023 07:55:20
 *  Author: maxbit89
 */ 


#ifndef GBCARTRIDGE_H_
#define GBCARTRIDGE_H_

#include "../gpio/gpio.h"

typedef struct {
	Gpio_Port addr_low;
	Gpio_Port addr_high;
	Gpio_Pin *d0;
	Gpio_Pin *d1;
	Gpio_Pin *d2;
	Gpio_Pin *d3;
	Gpio_Pin *d4;
	Gpio_Pin *d5;
	Gpio_Pin *d6;
	Gpio_Pin *d7;
	Gpio_Pin *write;
	Gpio_Pin *read;
} GB_CartridgeSlot;

typedef enum {
	GB_ROM_MBC_NOT_SUPPORTED = -1,
	GB_ROM_MBC_NONE = 0,
	GB_ROM_MBC_1 = 1,
	GB_ROM_MBC_2 = 2,
	GB_ROM_MBC_3 = 3,
	GB_ROM_MBC_5 = 5
} GB_MBC_Type;

typedef struct __attribute__((__packed__)) {
	uint8_t romName[0x10];
	GB_MBC_Type mbcType;
	uint8_t cartridgeType;
	uint8_t isGBC; //Has Special GameBoy Color Features
	uint8_t isSGB; //Has Special Super Gameboy Features
	uint8_t isRTC; //Has RTC Support;
	uint8_t isRumble; //Has Rumble Support;
	uint8_t isBatteryBufferd; //Has Battery Buffered Memory
	uint8_t romVersion;
	uint8_t romSize;
	uint8_t ramSize;
	uint16_t romBanks;
	uint16_t ramBanks;
	uint32_t romDumpSize;
	uint32_t ramDumpSize;
} GB_RomInfo;

Gpio_Status gbCartridge_init(GB_CartridgeSlot *gbSlot);
Gpio_Status gbCartridge_read(const GB_CartridgeSlot* gbSlot, uint16_t addr, uint8_t* value);
Gpio_Status gbCartridge_readBlob(const GB_CartridgeSlot* gbSlot, uint16_t offset, uint16_t len, uint8_t* buffer);
uint8_t gbCartridge_isPressent(const GB_CartridgeSlot* slot);
Gpio_Status gbCartridge_getRomInfo(const GB_CartridgeSlot* slot, GB_RomInfo* romInfo);
void gbCartridge_printRomInfo(const GB_RomInfo* romInfo);
uint8_t gbCartridge_isChecksumValid(const GB_CartridgeSlot* slot);
void gbCartridge_switchRomBank(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint8_t bank);
void gbCartridge_switchRamBank(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint8_t bank);
#endif /* GBCARTRIDGE_H_ */