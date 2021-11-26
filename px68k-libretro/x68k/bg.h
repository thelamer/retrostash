#ifndef _winx68k_bg
#define _winx68k_bg

#include <stdint.h>
#include "common.h"

extern	uint8_t	BG_DrawWork0[1024*1024];
extern	uint8_t	BG_DrawWork1[1024*1024];
extern	DWORD	BG0ScrollX, BG0ScrollY;
extern	DWORD	BG1ScrollX, BG1ScrollY;
extern	DWORD	BG_AdrMask;
extern	uint8_t	BG_CHRSIZE;
extern	uint8_t	BG_Regs[0x12];
extern	WORD	BG_BG0TOP;
extern	WORD	BG_BG1TOP;
extern	long	BG_HAdjust;
extern	long	BG_VLINE;
extern	DWORD	VLINEBG;

extern	uint8_t	Sprite_DrawWork[1024*1024];
extern	WORD	BG_LineBuf[1600];

void BG_Init(void);

uint8_t FASTCALL BG_Read(DWORD adr);
void FASTCALL BG_Write(DWORD adr, uint8_t data);

void FASTCALL BG_DrawLine(int opaq, int gd);

#endif
