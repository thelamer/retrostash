#ifndef _WINX68K_MEMORY_H
#define _WINX68K_MEMORY_H

#include <stdint.h>
#include "../libretro/common.h"

#define	Memory_ReadB		cpu_readmem24
#define Memory_ReadW		cpu_readmem24_word
#define Memory_ReadD		cpu_readmem24_dword

#define	Memory_WriteB		cpu_writemem24
#define Memory_WriteW		cpu_writemem24_word
#define Memory_WriteD		cpu_writemem24_dword

extern	uint8_t*	IPL;
extern	uint8_t*	MEM;
extern	uint8_t*	OP_ROM;
extern	uint8_t*	FONT;
extern  uint8_t    SCSIIPL[0x2000];
extern  uint8_t    SRAM[0x4000];
extern  uint8_t    GVRAM[0x80000];
extern  uint8_t   TVRAM[0x80000];

extern	DWORD	BusErrFlag;
extern	DWORD	BusErrAdr;
extern	DWORD	MemByteAccess;

void Memory_ErrTrace(void);
void Memory_IntErr(int i);

void Memory_Init(void);
DWORD Memory_ReadB(DWORD adr);
DWORD Memory_ReadW(DWORD adr);
DWORD Memory_ReadD(DWORD adr);

uint8_t dma_readmem24(DWORD adr);
WORD dma_readmem24_word(DWORD adr);
DWORD dma_readmem24_dword(DWORD adr);

void Memory_WriteB(DWORD adr, DWORD data);
void Memory_WriteW(DWORD adr, DWORD data);
void Memory_WriteD(DWORD adr, DWORD data);

void dma_writemem24(DWORD adr, uint8_t data);
void dma_writemem24_word(DWORD adr, WORD data);
void dma_writemem24_dword(DWORD adr, DWORD data);

void cpu_setOPbase24(DWORD adr);

void Memory_SetSCSIMode(void);

#endif
