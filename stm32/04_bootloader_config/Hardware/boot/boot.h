#ifndef __BOOT_H__
#define __BOOT_H__

#include <string.h>
#include "main.h"
#include "usart.h"

typedef void (*pFunction)(void);

void boot_init(void);

__asm void MSR_MSP_set(uint32_t address);
void LOAD_A_Reset_Handler(uint32_t address);
void Reset_peripheral(void);

#endif /* __BOOT_H__ */
