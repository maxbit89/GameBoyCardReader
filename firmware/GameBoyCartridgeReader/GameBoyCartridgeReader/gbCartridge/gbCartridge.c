/*
 * gbCartridge.c
 *
 * Created: 07.01.2023 07:55:02
 *  Author: maxbit89
 */ 

#include <string.h>
#include <util/delay.h>
#include "../uart/uart.h"
#include "gbCartridge.h"

static const uint8_t nintendoLogo[] = {
	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
	0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
	0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

Gpio_Status gbCartridge_init(GB_CartridgeSlot *gbSlot) {
	Gpio_Status status;
	if((status = gpioInit(gbSlot->d0, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d1, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d2, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d3, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d4, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d5, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d6, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d7, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	
	if((status = gpioInit(gbSlot->write, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->read, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	
	if((status = gpioWritePin(gbSlot->write, true)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, true)) != GPIO_STATUS_OK) return status;
	return status;
}

Gpio_Status gbCartridge_write(const GB_CartridgeSlot* gbSlot, uint16_t addr, uint8_t value) {
	Gpio_Status status;
	uint8_t addr_low = (uint8_t)(addr & 0xff);
	uint8_t addr_high = (uint8_t)((addr & 0xff00) >> 8);
	
	if((status = gpioWritePin(gbSlot->write, true)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, true)) != GPIO_STATUS_OK) return status;

	_delay_us(800);

	if((status = gpioWriteU8(gbSlot->addr_low.handle, gbSlot->addr_low.gpioId, addr_low)) != GPIO_STATUS_OK) return status;
	if((status = gpioWriteU8(gbSlot->addr_high.handle, gbSlot->addr_high.gpioId, addr_high)) != GPIO_STATUS_OK) return status;

	_delay_us(60);

	//Set Databus to Input:
	if((status = gpioInit(gbSlot->d0, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d1, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d2, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d3, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d4, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d5, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d6, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d7, GPIO_PIN_MODE_OUTPUT)) != GPIO_STATUS_OK) return status;

	if((status = gpioWritePin(gbSlot->d0, value & 0x01)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d1, value & 0x02)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d2, value & 0x04)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d3, value & 0x08)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d4, value & 0x10)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d5, value & 0x20)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d6, value & 0x48)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->d7, value & 0x80)) != GPIO_STATUS_OK) return status;
	
	if((status = gpioWritePin(gbSlot->write, false)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, true)) != GPIO_STATUS_OK) return status;

	_delay_us(60);
	
	if((status = gpioWritePin(gbSlot->write, true)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, true)) != GPIO_STATUS_OK) return status;

	return status;
}

Gpio_Status gbCartridge_read(const GB_CartridgeSlot* gbSlot, uint16_t addr, uint8_t* value) {
	Gpio_Status status;
	uint8_t pinValue;
	uint8_t addr_low = (uint8_t)(addr & 0xff);
	uint8_t addr_high = (uint8_t)((addr & 0xff00) >> 8);
	
	if((status = gpioWritePin(gbSlot->write, true)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, true)) != GPIO_STATUS_OK) return status;

	_delay_us(800);

	if((status = gpioWriteU8(gbSlot->addr_low.handle, gbSlot->addr_low.gpioId, addr_low)) != GPIO_STATUS_OK) return status;
	if((status = gpioWriteU8(gbSlot->addr_high.handle, gbSlot->addr_high.gpioId, addr_high)) != GPIO_STATUS_OK) return status;

	_delay_us(60);

	if((status = gpioWritePin(gbSlot->write, true)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, false)) != GPIO_STATUS_OK) return status;

	_delay_us(60);

	//Set Databus to Input:
	if((status = gpioInit(gbSlot->d0, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d1, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d2, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d3, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d4, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d5, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d6, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;
	if((status = gpioInit(gbSlot->d7, GPIO_PIN_MODE_INPUT)) != GPIO_STATUS_OK) return status;

	if((status = gpioReadPin(gbSlot->d0, &pinValue)) != GPIO_STATUS_OK) return status;
	*value = (pinValue<< 0);
	if((status = gpioReadPin(gbSlot->d1, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 1);
	if((status = gpioReadPin(gbSlot->d2, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 2);
	if((status = gpioReadPin(gbSlot->d3, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 3);
	if((status = gpioReadPin(gbSlot->d4, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 4);
	if((status = gpioReadPin(gbSlot->d5, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 5);
	if((status = gpioReadPin(gbSlot->d6, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 6);
	if((status = gpioReadPin(gbSlot->d7, &pinValue)) != GPIO_STATUS_OK) return status;
	*value |= (pinValue<< 7);

	if((status = gpioWritePin(gbSlot->write, false)) != GPIO_STATUS_OK) return status;
	if((status = gpioWritePin(gbSlot->read, false)) != GPIO_STATUS_OK) return status;

	return status;
}

Gpio_Status gbCartridge_readBlob(const GB_CartridgeSlot* gbSlot, uint16_t offset, uint16_t len, uint8_t* data) {
	Gpio_Status status = GPIO_STATUS_OK;
	uint8_t value = 0xff;
	uint16_t addr;
	len--;
	/*
	sprintf(buffer, "\n%04x: ", offset);
	uart_puts(buffer);
	*/
	for (addr = offset; addr <= offset + len; addr++) {
		if((status = gbCartridge_read(gbSlot, addr, &value)) != GPIO_STATUS_OK) return status;
		if (data != NULL) {
			data[addr - offset] = value;
		}
				/*
		if ((addr) % 16 == 0) {
			sprintf(buffer, "\n%04x: ", addr);
			uart_puts(buffer);
		}
		sprintf(buffer, "%02x ", value);
		uart_puts(buffer);
		*/
	}
	//uart_puts("\r\n");
	return status;
}

uint8_t gbCartridge_isPressent(const GB_CartridgeSlot* slot) {
	Gpio_Status status;
	size_t checkSize = sizeof(nintendoLogo);
	//size_t checkSize = 1;
	uint8_t nintendoLogoIn[checkSize];
	if((status = gbCartridge_readBlob(slot, 0x104, checkSize, nintendoLogoIn)) != GPIO_STATUS_OK) {
		 sprintf(buffer, "readBloc Failed: %i\r\n", status);
		 uart_puts(buffer);
	}
	return memcmp(nintendoLogo, nintendoLogoIn, checkSize) == 0;
}

uint8_t gbCartridge_isChecksumValid(const GB_CartridgeSlot* slot) {
	int checksum = 0;
	uint8_t data;

	for (int addr = 0x134; addr < 0x14E; addr++) {
		if(gbCartridge_read(slot, addr, &data) != GPIO_STATUS_OK) return false;
		checksum += data;
	}

	return ((checksum + 25) & 0xFF) == 0;
}

Gpio_Status gbCartridge_getRomInfo(const GB_CartridgeSlot* slot, GB_RomInfo* romInfo) {
	Gpio_Status status = GPIO_STATUS_OK;
	uint8_t in;
	if((status = gbCartridge_readBlob(slot, 0x0134, sizeof(romInfo->romName)-1, romInfo->romName)) != GPIO_STATUS_OK) return status;
	romInfo->romName[sizeof(romInfo->romName)-1] = '\0';
	if((status = gbCartridge_read(slot, 0x143, &in)) != GPIO_STATUS_OK) return status;
	romInfo->isGBC = (in == 0x80) || (in == 0xC0);
	if((status = gbCartridge_read(slot, 0x146, &in)) != GPIO_STATUS_OK) return status;
	romInfo->isSGB = (in == 0x03);
	if((status = gbCartridge_read(slot, 0x149, &(romInfo->ramSize))) != GPIO_STATUS_OK) return status;
	if((status = gbCartridge_read(slot, 0x14C, &(romInfo->romVersion))) != GPIO_STATUS_OK) return status;
	switch (romInfo->romSize)
	{
		case 0x00:
			romInfo->romBanks = 2;
			break;
		case 0x01:
			romInfo->romBanks = 4;
			break;
		case 0x02:
			romInfo->romBanks = 8;
			break;
		case 0x03:
			romInfo->romBanks = 16;
			break;
		case 0x04:
			romInfo->romBanks = 32;
			break;
		case 0x05:
			romInfo->romBanks = 64;
			break;
		case 0x06:
			romInfo->romBanks = 128;
			break;
		case 0x07:
			romInfo->romBanks = 256;
			break;
		default:
			romInfo->romBanks = 2;
	}
	romInfo->romDumpSize = 0x4000*romInfo->romBanks;
	switch (romInfo->ramSize)
	{
		case 0x00:
			romInfo->ramBanks = 0;
			break;
		case 0x01:
			romInfo->ramBanks = 1;
			break;
		case 0x02:
			romInfo->ramBanks = 1;
			break;
		case 0x03:
			romInfo->ramBanks = 4;
			break;
		case 0x04:
			romInfo->ramBanks = 16;
			break;
		default:
			romInfo->ramBanks = 0;
	}
	romInfo->romDumpSize = 0x4000*romInfo->romBanks;
	
	if((status = gbCartridge_read(slot, 0x0147, &(romInfo->cartridgeType))) != GPIO_STATUS_OK) return status;
	switch (romInfo->cartridgeType)
	{
		case 0x00:
		// NO MBC
		case 0x08:
		// ROM
		// SRAM
		case 0x09:
		// ROM
		// SRAM
		// BATT
		romInfo->mbcType = GB_ROM_MBC_NONE;
		break;
		case 0x01:
		// MBC1
		case 0x02:
		// MBC1
		// SRAM
		case 0x03:
		// MBC1
		// SRAM
		// BATT
		case 0xFF:
		// Hack to accept HuC1 as a MBC1
		romInfo->mbcType = GB_ROM_MBC_1;
		break;
		case 0x05:
		// MBC2
		// SRAM
		case 0x06:
		// MBC2
		// SRAM
		// BATT
		romInfo->mbcType = GB_ROM_MBC_2;
		break;
		case 0x0F:
		// MBC3
		// TIMER
		// BATT
		case 0x10:
		// MBC3
		// TIMER
		// BATT
		// SRAM
		case 0x11:
		// MBC3
		case 0x12:
		// MBC3
		// SRAM
		case 0x13:
		// MBC3
		// BATT
		// SRAM
		case 0xFC:
		// Game Boy Camera
		romInfo->mbcType = GB_ROM_MBC_3;
		break;
		case 0x19:
		// MBC5
		case 0x1A:
		// MBC5
		// SRAM
		case 0x1B:
		// MBC5
		// BATT
		// SRAM
		case 0x1C:
		// RUMBLE
		case 0x1D:
		// RUMBLE
		// SRAM
		case 0x1E:
		// RUMBLE
		// BATT
		// SRAM
		romInfo->mbcType = GB_ROM_MBC_5;
		break;
		case 0x0B:
		// MMMO1
		case 0x0C:
		// MMM01
		// SRAM
		case 0x0D:
		// MMM01
		// SRAM
		// BATT
		case 0x15:
		// MBC4
		case 0x16:
		// MBC4
		// SRAM
		case 0x17:
		// MBC4
		// SRAM
		// BATT
		case 0x22:
		// MBC7
		// BATT
		// SRAM
		case 0x55:
		// GG
		case 0x56:
		// GS3
		case 0xFD:
		// TAMA 5
		case 0xFE:
		// HuC3
		romInfo->mbcType = GB_ROM_MBC_NOT_SUPPORTED;
		break;
		default:
		romInfo->mbcType = GB_ROM_MBC_NOT_SUPPORTED;
	}
	
	switch (romInfo->cartridgeType)
	{
		case 0x03:
		case 0x06:
		case 0x09:
		case 0x0D:
		case 0x0F:
		case 0x10:
		case 0x13:
		case 0x17:
		case 0x1B:
		case 0x1E:
		case 0x22:
		case 0xFD:
		case 0xFF:
		romInfo->isBatteryBufferd = true;
		break;
		default:
		romInfo->isBatteryBufferd = false;
	}

	switch (romInfo->cartridgeType)
	{
		case 0x0F:
		case 0x10:
		romInfo->isRTC = true;
		break;
		default:
		romInfo->isRTC = false;
	}

	switch (romInfo->cartridgeType)
	{
		case 0x1C:
		case 0x1D:
		case 0x1E:
		romInfo->isRumble = true;
		break;
		default:
		romInfo->isRumble = false;
	}
	
	return status;
}

const char* gbCartridge_mbcTypeToStr(GB_MBC_Type type) {
	switch(type) {
		case GB_ROM_MBC_NONE:
			return "MBC_NONE";
		case GB_ROM_MBC_1:
			return "MBC_1";
		case GB_ROM_MBC_2:
			return "MBC_2";
		case GB_ROM_MBC_3:
			return "MBC_3";
		case GB_ROM_MBC_5:
			return "MBC_4";
		default:
			return "MBC_NOT_SUPPORTED";
	}
	return "MBC_NOT_SUPPORTED";
}

void gbCartridge_printRomInfo(const GB_RomInfo* romInfo) {
	//Print Title
	sprintf(buffer, "Game Title: %s\r\n", romInfo->romName);
	uart_puts(buffer);
	//Print Card Type
	sprintf(buffer, " CARTRIDGE_TYPE: %02x\r\n", romInfo->cartridgeType);
	uart_puts(buffer);
	//Print Memory Controller Type
	sprintf(buffer, " MBC_TYPE: %s\r\n", gbCartridge_mbcTypeToStr(romInfo->mbcType));
	uart_puts(buffer);
	//Print Rom Size
	sprintf(buffer, " ROM_SIZE: %d\r\n", romInfo->romSize);
	uart_puts(buffer);
	//Print Rom Banks
	sprintf(buffer, " ROM_BANKS: %d\r\n", romInfo->romBanks);
	uart_puts(buffer);
	//Print Ram Size
	sprintf(buffer, " RAM_SIZE: %d\r\n", romInfo->ramSize);
	uart_puts(buffer);
	//Print Ram Banks
	sprintf(buffer, " RAM_BANKS: %d\r\n", romInfo->ramBanks);
	uart_puts(buffer);
}

void gbCartridge_switchRomBank(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint8_t bank) {
	switch (romInfo->mbcType) {
		case GB_ROM_MBC_1:
		case GB_ROM_MBC_2:
		case GB_ROM_MBC_3:
		case GB_ROM_MBC_5:
			gbCartridge_write(slot, 0x2100, bank);
			break;
	}
}

void gbCartridge_switchRamBank(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint8_t bank) {
	if (romInfo->ramBanks > 1) {
		switch (romInfo->mbcType) {
			case GB_ROM_MBC_1:
			case GB_ROM_MBC_3:
			case GB_ROM_MBC_5:
				gbCartridge_write(slot, 0x4000, bank);
				break;
		}
	}
}