/*
 * crc16.h
 *
 * Created: 14.01.2023 08:02:50
 *  Author: maxbit89
 */ 


#ifndef CRC16_H_
#define CRC16_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uint16_t crc16(const uint8_t *buf, size_t len);

#endif /* CRC16_H_ */