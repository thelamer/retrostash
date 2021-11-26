#ifndef _winx68k_sysport
#define _winx68k_sysport

#include <stdint.h>
#include "common.h"

extern	uint8_t	SysPort[7];

void SysPort_Init(void);
uint8_t FASTCALL SysPort_Read(DWORD adr);
void FASTCALL SysPort_Write(DWORD adr, uint8_t data);

#endif
