#ifndef __PLATFORM_INTERFACE_H
#define __PLATFORM_INTERFACE_H

#include <stdint.h>

void Send(uint8_t *pData, uint32_t size);
void Delay(uint32_t msec);

#endif
