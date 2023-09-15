/*
 * xtransfer.h
 *
 * Created: 21.01.2023 13:44:34
 *  Author: maxbit89
 */ 


#ifndef XTRANSFER_H_
#define XTRANSFER_H_

#include <stdio.h>

#define XTRANSFER_STATE_OK 0
#define XTRANSFER_STATE_ERR_RECV_HEADER (-1)
#define XTRANSFER_STATE_ERR_RECV_DATA   (-2)
#define XTRANSFER_STATE_ERR_TIMOUT      (-3)
#define XTRANSFER_STATE_ERR_INVALID_ARG (-4)

typedef struct __attribute__((__packed__)) {
	uint8_t ioId[2];
	int32_t type;
	uint32_t size;
	uint8_t xId[2];
} XTransfer_Header;

void xtransfer_transmitHeader(int type, uint32_t size);
void xtransfer_transmitData(uint8_t *data, uint32_t size);
int32_t xtransfer_receiveHeader(XTransfer_Header *header, uint32_t timeoutMs);

#endif /* XTRANSFER_H_ */