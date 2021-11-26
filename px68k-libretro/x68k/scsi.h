#ifndef _winx68k_scsi
#define _winx68k_scsi

#include <stdint.h>
#include "common.h"

extern	uint8_t	SCSIIPL[0x2000];

void SCSI_Init(void);
void SCSI_Cleanup(void);

uint8_t FASTCALL SCSI_Read(DWORD adr);
void FASTCALL SCSI_Write(DWORD adr, uint8_t data);

#endif

