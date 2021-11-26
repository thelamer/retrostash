#ifndef _winx68k_sram
#define _winx68k_sram

#include "common.h"

extern	uint8_t	SRAM[0x4000];

void SRAM_Init(void);
void SRAM_Cleanup(void);
void SRAM_VirusCheck(void);

uint8_t FASTCALL SRAM_Read(DWORD adr);
void FASTCALL SRAM_Write(DWORD adr, uint8_t data);

#endif

