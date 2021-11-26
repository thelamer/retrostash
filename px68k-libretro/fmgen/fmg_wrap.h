#ifndef _win68_opm_fmgen
#define _win68_opm_fmgen

#include <stdint.h>

int OPM_Init(int clock, int rate);
void OPM_Cleanup(void);
void OPM_Reset(void);
void OPM_Update(int16_t *buffer, int length, int rate, uint8_t *pbsp, uint8_t *pbep);
void FASTCALL OPM_Write(DWORD r, uint8_t v);
uint8_t FASTCALL OPM_Read(WORD a);
void FASTCALL OPM_Timer(DWORD step);
void OPM_SetVolume(uint8_t vol);
void OPM_SetRate(int clock, int rate);
void OPM_RomeoOut(unsigned int delay);

int M288_Init(int clock, int rate, const char* path);
void M288_Cleanup(void);
void M288_Reset(void);
void M288_Update(int16_t *buffer, int length);
void FASTCALL M288_Write(DWORD r, uint8_t v);
uint8_t FASTCALL M288_Read(WORD a);
void FASTCALL M288_Timer(DWORD step);
void M288_SetVolume(uint8_t vol);
void M288_SetRate(int clock, int rate);
void M288_RomeoOut(unsigned int delay);

#endif //_win68_opm_fmgen
