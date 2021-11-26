#ifndef _winx68k_midi
#define _winx68k_midi

#include <stdint.h>
#include "common.h"

void MIDI_Init(void);
void MIDI_Cleanup(void);
void MIDI_Reset(void);
uint8_t FASTCALL MIDI_Read(DWORD adr);
void FASTCALL MIDI_Write(DWORD adr, uint8_t data);
void MIDI_SetModule(void);
void FASTCALL MIDI_Timer(DWORD clk);
int MIDI_SetMimpiMap(char *filename);
int MIDI_EnableMimpiDef(int enable);
void MIDI_DelayOut(unsigned int delay);

#endif
