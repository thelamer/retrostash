#ifndef _winx68k_irqh
#define _winx68k_irqh

#include <stdint.h>
#include "common.h"

void IRQH_Init(void);
DWORD FASTCALL IRQH_DefaultVector(uint8_t irq);
void IRQH_IRQCallBack(uint8_t irq);
void IRQH_Int(uint8_t irq, void* handler);

#endif
