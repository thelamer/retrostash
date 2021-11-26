#ifndef _winx68k_fdc
#define _winx68k_fdc

#include <stdint.h>
#include "common.h"

void FDC_Init(void);
uint8_t FASTCALL FDC_Read(DWORD adr);
void FASTCALL FDC_Write(DWORD adr, uint8_t data);
int16_t FDC_Flush(void);
void FDC_EPhaseEnd(void);
void FDC_SetForceReady(int n);
int FDC_IsDataReady(void);

#endif //_winx68k_fdc

