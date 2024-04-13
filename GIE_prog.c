#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GIE_reg.h"
#include "GIE_interface.h"

void GIE_voidEnableGlobal(void)
{
	//SET_BIT(SREG, SREG_I);   /*read&modifiy&wright so it will take 3 CLK cycle at least*/
	__asm volatile("SEI");     /*
								*take only one CLK cycle
								*Inline assembly:set I-Bit
								*/
	
}

void GIE_voidDisableGlobal(void)
{
	//CLR_BIT(SREG, SREG_I);
	__asm volatile("CLI");


}
