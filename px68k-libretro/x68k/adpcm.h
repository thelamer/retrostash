#ifndef _winx68k_adpcm_h
#define _winx68k_adpcm_h

#include <stdint.h>

extern uint8_t ADPCM_Clock;
extern DWORD ADPCM_ClockRate;

void FASTCALL ADPCM_PreUpdate(DWORD clock);
void FASTCALL ADPCM_Update(int16_t *buffer, DWORD length, int rate, uint8_t *pbsp, uint8_t *pbep);

void FASTCALL ADPCM_Write(DWORD adr, uint8_t data);
uint8_t FASTCALL ADPCM_Read(DWORD adr);

void ADPCM_SetVolume(uint8_t vol);
void ADPCM_SetPan(int n);
void ADPCM_SetClock(int n);

void ADPCM_Init(DWORD samplerate);
int ADPCM_IsReady(void);

#endif
