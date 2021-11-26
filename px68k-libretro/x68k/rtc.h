#ifndef _x68k_rtc
#define _x68k_rtc

#include <stdint.h>

void RTC_Init(void);
uint8_t FASTCALL RTC_Read(DWORD adr);
void FASTCALL RTC_Write(DWORD adr, uint8_t data);
void RTC_Timer(int clock);

#endif
