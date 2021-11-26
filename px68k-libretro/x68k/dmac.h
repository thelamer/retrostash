#ifndef _winx68k_dmac
#define _winx68k_dmac

#include <stdint.h>
#include "common.h"

typedef struct
{
	uint8_t CSR;		// 00
	uint8_t CER;
	uint8_t dmy0[2];
	uint8_t DCR;		// 04
	uint8_t OCR;
	uint8_t SCR;
	uint8_t CCR;
	uint8_t dmy1[2];	// 08
	WORD MTC;
	DWORD MAR;		// 0C
	uint8_t dmy2[4];	// 10
	DWORD DAR;		// 14
	uint8_t dmy3[2];	// 18
	WORD BTC;
	DWORD BAR;		// 1C
	uint8_t dmy4[5];	// 20
	uint8_t NIV;
	uint8_t dmy5;
	uint8_t EIV;
	uint8_t dmy6;		// 28
	uint8_t MFC;
	uint8_t dmy7[3];
	uint8_t CPR;
	uint8_t dmy8[3];
	uint8_t DFC;
	uint8_t dmy9[7];
	uint8_t BFC;
	uint8_t dmya[5];
	uint8_t GCR;

} dmac_ch;

extern dmac_ch	DMA[4];

DWORD FASTCALL DMA_Int(uint8_t irq);
uint8_t FASTCALL DMA_Read(DWORD adr);
void FASTCALL DMA_Write(DWORD adr, uint8_t data);

int FASTCALL DMA_Exec(int ch);
void DMA_Init(void);
void DMA_SetReadyCB(int ch, int (*func)(void));

#endif //_winx68k_dmac
