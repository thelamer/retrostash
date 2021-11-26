#ifndef _winx68k_crtc
#define _winx68k_crtc

#include <stdint.h>
#include "common.h"

#define	VSYNC_HIGH	180310L
#define	VSYNC_NORM	162707L

extern	uint8_t	CRTC_Regs[48];
extern	uint8_t	CRTC_Mode;
extern	WORD	CRTC_VSTART, CRTC_VEND;
extern	WORD	CRTC_HSTART, CRTC_HEND;
extern	DWORD	TextDotX, TextDotY;
extern	DWORD	TextScrollX, TextScrollY;
extern	uint8_t	VCReg0[2];
extern	uint8_t	VCReg1[2];
extern	uint8_t	VCReg2[2];
extern	WORD	CRTC_IntLine;
extern	uint8_t	CRTC_FastClr;
extern	uint8_t	CRTC_DispScan;
extern	DWORD	CRTC_FastClrLine;
extern	WORD	CRTC_FastClrMask;
extern	uint8_t	CRTC_VStep;
extern  int		HSYNC_CLK;

extern	DWORD	GrphScrollX[];
extern	DWORD	GrphScrollY[];

void CRTC_Init(void);

void CRTC_RasterCopy(void);

uint8_t FASTCALL CRTC_Read(DWORD adr);
void FASTCALL CRTC_Write(DWORD adr, uint8_t data);

uint8_t FASTCALL VCtrl_Read(DWORD adr);
void FASTCALL VCtrl_Write(DWORD adr, uint8_t data);

#endif
