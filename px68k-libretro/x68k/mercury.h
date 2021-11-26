#ifndef _winx68k_mercury_h
#define _winx68k_mercury_h

#include <stdint.h>

extern uint8_t Mcry_LRTiming;

void FASTCALL Mcry_Update(int16_t *buffer, DWORD length);
void FASTCALL Mcry_PreUpdate(DWORD clock);

void FASTCALL Mcry_Write(DWORD adr, uint8_t data);
uint8_t FASTCALL Mcry_Read(DWORD adr);

void Mcry_SetClock(void);
void Mcry_SetVolume(uint8_t vol);

void Mcry_Init(DWORD samplerate, const char* path);
void Mcry_Cleanup(void);
int Mcry_IsReady(void);

void FASTCALL Mcry_Int(void);

#endif
