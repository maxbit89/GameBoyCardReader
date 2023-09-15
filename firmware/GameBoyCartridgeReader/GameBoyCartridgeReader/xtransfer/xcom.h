/*
 * xcom.h
 * 
 * This is the Specific Protocol for the GameBoy Cartridge Reader that runs on top of xtransfer
 *
 * Created: 21.01.2023 14:36:18
 *  Author: maxbit89
 */ 


#ifndef XCOM_H_
#define XCOM_H_

#include "xtransfer.h"
#include "../gbCartridge/gbCartridge.h"

#define XCOM_STATE_OK         0
#define XCOM_TYPE_GBROMINFO  10
#define XCOM_TYPE_ROM_READ   11
#define XCOM_TYPE_SAVE_READ  12
#define XCOM_TYPE_SAVE_WRITE 13
#define XCOM_TYPE_CARTRIDGE_OP_ERR -10

int32_t xcom_waitTransferComplete(uint32_t timeout_ms);
int32_t xcom_cartridge_detect(GB_RomInfo* romInfo);
int32_t xcom_do_cartridgeAction(GB_CartridgeSlot* slot, GB_RomInfo* romInfo, uint32_t timeout_ms);


#endif /* XCOM_H_ */