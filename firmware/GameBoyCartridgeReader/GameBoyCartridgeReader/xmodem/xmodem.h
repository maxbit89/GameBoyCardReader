/*
 * xmodem.h
 *
 * Created: 14.01.2023 09:34:59
 *  Author: maxbit89
 */ 


#ifndef XMODEM_H_
#define XMODEM_H_
#include <stdint.h>

typedef int (*XModem_OnRecvFragment)(uint8_t *buffer, size_t offset, size_t len);
typedef int (*XModem_OnTransmitFragment)(uint8_t *buffer, size_t offset, size_t len);

int xmodemReceive(size_t maxlen, XModem_OnRecvFragment onRecvFragment);
int xmodemTransmit(size_t srcsz, XModem_OnTransmitFragment onTransmitFragment);

#endif /* XMODEM_H_ */