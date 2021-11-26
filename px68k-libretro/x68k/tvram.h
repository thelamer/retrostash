#ifndef _winx68k_tvram
#define _winx68k_tvram

#include <stdint.h>
#include "common.h"

extern	uint8_t	TVRAM[0x80000];
extern	uint8_t	TextDrawWork[1024*1024];
extern	uint8_t	TextDirtyLine[1024];
extern	uint8_t	Text_TrFlag[1024];

void TVRAM_SetAllDirty(void);

void TVRAM_Init(void);
void TVRAM_Cleanup(void);

uint8_t FASTCALL TVRAM_Read(DWORD adr);
void FASTCALL TVRAM_Write(DWORD adr, uint8_t data);
void FASTCALL TVRAM_RCUpdate(void);
void FASTCALL Text_DrawLine(int opaq);

#endif
