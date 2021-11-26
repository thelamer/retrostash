#ifndef _winx68k_windraw_h
#define _winx68k_windraw_h

#include <stdint.h>

extern int winx, winy;
extern int winh, winw;
extern uint8_t Draw_ClrMenu;
extern WORD FrameCount;
extern WORD WinDraw_Pal16B, WinDraw_Pal16R, WinDraw_Pal16G;

extern	uint8_t	Draw_BitMask[800];
extern	uint8_t	Draw_TextBitMask[800];

extern	int	WindowX;
extern	int	WindowY;
extern	int	kbd_x, kbd_y, kbd_w, kbd_h;

void WinDraw_InitWindowSize(WORD width, WORD height);
int WinDraw_Init(void);
void WinDraw_Cleanup(void);
void WinDraw_Redraw(void);
void FASTCALL WinDraw_Draw(void);
void WinDraw_ShowMenu(int flag);
void WinDraw_DrawLine(void);
void WinDraw_ChangeSize(void);

void WinDraw_StartupScreen(void);
void WinDraw_CleanupScreen(void);

int WinDraw_MenuInit(void);
void WinDraw_DrawMenu(int menu_state, int mkey_pos, int mkey_y, int *mval_y);

extern struct menu_flist mfl;

void WinDraw_DrawMenufile(struct menu_flist *mfl);
void WinDraw_ClearMenuBuffer(void);
void WinDraw_reverse_key(int x, int y);

#endif //winx68k_windraw_h


