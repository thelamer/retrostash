#ifndef _winx68k_scc
#define _winx68k_scc

#include <stdint.h>
#include "common.h"

void SCC_IntCheck(void);
void SCC_Init(void);
uint8_t FASTCALL SCC_Read(DWORD adr);
void FASTCALL SCC_Write(DWORD adr, uint8_t data);

extern signed char MouseX;
extern signed char MouseY;
extern uint8_t MouseSt;

#endif
