// ---------------------------------------------------------------------------------------
//  IRQH.C - IRQ Handler (架空のデバイスにょ)
// ---------------------------------------------------------------------------------------

#include "common.h"
#include "../m68000/m68000.h"
#include "irqh.h"

#if defined (HAVE_CYCLONE)
extern struct Cyclone m68k;
typedef signed int  FASTCALL C68K_INT_CALLBACK(signed int level);
#endif /* HAVE_CYCLONE */

	uint8_t	IRQH_IRQ[8];
	void	*IRQH_CallBack[8];

// -----------------------------------------------------------------------
//   初期化
// -----------------------------------------------------------------------
void IRQH_Init(void)
{
	memset(IRQH_IRQ, 0, 8);
}


// -----------------------------------------------------------------------
//   デフォルトのベクタを返す（これが起こったら変だお）
// -----------------------------------------------------------------------
DWORD FASTCALL IRQH_DefaultVector(uint8_t irq)
{
	IRQH_IRQCallBack(irq);
	return -1;
}


// -----------------------------------------------------------------------
//   他の割り込みのチェック
//   各デバイスのベクタを返すルーチンから呼ばれます
// -----------------------------------------------------------------------
void IRQH_IRQCallBack(uint8_t irq)
{
	IRQH_IRQ[irq&7] = 0;
int i;

#if defined (HAVE_CYCLONE)
	m68k.irq =0;
#elif defined (HAVE_C68K)
	C68k_Set_IRQ(&C68K, 0);
#endif /* HAVE_C68K */

	for (i=7; i>0; i--)
	{
	    if (IRQH_IRQ[i])
	    {
#if defined (HAVE_CYCLONE)
			m68k.irq = i;
#elif defined (HAVE_C68K)
			C68k_Set_IRQ(&C68K, i);
#endif /* HAVE_C68K */
		return;
	    }
	}
}

// -----------------------------------------------------------------------
//   割り込み発生
// -----------------------------------------------------------------------
void IRQH_Int(uint8_t irq, void* handler)
{
	int i;
	IRQH_IRQ[irq&7] = 1;
	if (handler==NULL)
	    IRQH_CallBack[irq&7] = &IRQH_DefaultVector;
	else
	    IRQH_CallBack[irq&7] = handler;
	for (i=7; i>0; i--)
	{
	    if (IRQH_IRQ[i])
	    {
#if defined (HAVE_CYCLONE)

	        m68k.irq = i;
#elif defined (HAVE_C68K)
	        C68k_Set_IRQ(&C68K, i);
#endif /* HAVE_C68K */
	        return;
	    }
	}
}

signed int  my_irqh_callback(signed int  level)
{
    int i;
    C68K_INT_CALLBACK *func = IRQH_CallBack[level&7];
    int vect = (func)(level&7);

    for (i=7; i>0; i--)
    {
		if (IRQH_IRQ[i])
		{
#if defined (HAVE_CYCLONE)
			m68k.irq = i;
#elif defined (HAVE_C68K)
	    	C68k_Set_IRQ(&C68K, i);
#endif /* HAVE_C68K */
			break;
		}
    }

    return (signed int )vect;
}
