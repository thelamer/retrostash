#ifndef _winx68k_cdrom
#define _winx68k_cdrom

#include <stdint.h>
#include "common.h"

typedef struct {
	unsigned char reserved;
	unsigned char adr;
	unsigned char trackno;
	unsigned char reserved1;
	unsigned char addr[4];
} TOCENTRY;

typedef struct {
	unsigned char size[2];
	unsigned char first;
	unsigned char last;
	TOCENTRY track[100];
} TOC;

void CDROM_Init(void);
void CDROM_Cleanup(void);
uint8_t FASTCALL CDROM_Read(DWORD adr);
void FASTCALL CDROM_Write(DWORD adr, uint8_t data);

extern uint8_t CDROM_ASPIChecked;

#endif

