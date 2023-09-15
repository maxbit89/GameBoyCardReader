/*
 * xcom.c
 *
 * Created: 21.01.2023 14:36:08
 *  Author: maxbit89
 */ 

#include "xcom.h"
#include "../gbCartridge/gbCartridge.h"

#define XCOM_TIMEOUT 10000

int32_t xcom_cartridge_detect(GB_RomInfo* romInfo) {
	int state;
	xtransfer_transmitHeader(XCOM_TYPE_GBROMINFO, sizeof(GB_RomInfo));
	xtransfer_transmitData((uint8_t*)romInfo, sizeof(GB_RomInfo));
	/*
	state = xcom_waitTransferComplete(XCOM_TIMEOUT);
	if(state != XCOM_TYPE_GBROMINFO) {
		return state;
	}
	*/
	return XCOM_STATE_OK;
}

uint32_t xcom_getRomSize(GB_RomInfo* romInfo) {
	
}

static int32_t xcom_readRomBank(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint8_t bank) {
	uint8_t data;
	uint16_t addr = 0;
	uint16_t offset = 0;
	
	if(bank > 0) {
		offset = 0x4000;
		gbCartridge_switchRomBank(slot, romInfo, bank);
	}
	
	for(addr = 0; addr < 0x4000; addr++) {
		if(gbCartridge_read(slot, addr + offset, &data) != GPIO_STATUS_OK) {
			xtransfer_transmitHeader(XCOM_TYPE_CARTRIDGE_OP_ERR, 0);
			return XCOM_TYPE_CARTRIDGE_OP_ERR;
		}
		xtransfer_transmitData(&data, 1);
	}
	return XCOM_STATE_OK;
}

int32_t xcom_do_cartridgeAction(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint32_t timeout_ms) {
	int32_t state;
	XTransfer_Header header;
	uint8_t bank;
	if((state = xtransfer_receiveHeader(&header, timeout_ms)) < 0) return state;
	switch(header.type) {
		case XCOM_TYPE_ROM_READ:
			for(bank=0;bank<romInfo->romBanks;bank++) {
				if((state = xcom_readRomBank(slot, romInfo, bank)) != XCOM_STATE_OK) {
					return state;
				}
			}
			return XCOM_STATE_OK;
		break;
		case XCOM_TYPE_SAVE_READ:
			xtransfer_transmitHeader(XTRANSFER_STATE_ERR_INVALID_ARG, 0);
			return XTRANSFER_STATE_ERR_INVALID_ARG;
		break;
		case XCOM_TYPE_SAVE_WRITE:
			xtransfer_transmitHeader(XTRANSFER_STATE_ERR_INVALID_ARG, 0);
			return XTRANSFER_STATE_ERR_INVALID_ARG;
		break;
		default:
			xtransfer_transmitHeader(XTRANSFER_STATE_ERR_INVALID_ARG, 0);
			return XTRANSFER_STATE_ERR_INVALID_ARG;
	}
}

int32_t xcom_waitTransferComplete(uint32_t timeout_ms) {
	int state;
	XTransfer_Header header;
	if((state = xtransfer_receiveHeader(&header, timeout_ms)) < 0) return state;
	return header.type;
}