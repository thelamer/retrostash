#ifndef _winx68k_ioc
#define _winx68k_ioc

#include <stdint.h>
#include "common.h"

extern	uint8_t	IOC_IntStat;
extern	uint8_t	IOC_IntVect;

void IOC_Init(void);
uint8_t FASTCALL IOC_Read(DWORD adr);
void FASTCALL IOC_Write(DWORD adr, uint8_t data);

#endif
