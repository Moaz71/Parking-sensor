#include "STD_TYPES.h"
#include "ErrType.h"
#include "BIT_MATH.h"

#include "TIMERS_interface.h"
#include "TIMERS_reg.h"
#include "TIMERS_cfg.h"
#include "TIMERS_prv.h"

static void (*Timer_pvCallBackFunc[8])(void)= {NULL};  /* 8 source of interrupt in timers */

//****************************Timer 0,2 Initialization function***************************
/**
 *
 * @param Copy_TimerSelect
 * @param Timer0_2_Cfg_Obj
 * @return Local_u8ErrorState
 */
uint8 Timer0_2_voidInit(Timer_Select_t Copy_TimerSelect, const Timer0_2_Config_t* Timer0_2_Cfg_Obj)
{
	uint8 Local_u8ErrorState = OK;

	if(Timer0_2_Cfg_Obj != NULL)
	{
		if(Copy_TimerSelect == Timer0)
		{
			//*********************setting wave generation mode *******************
			switch(Timer0_2_Cfg_Obj->WF_Gen_Mode)
			{
			case T0_2Normal 	   :CLR_BIT(TCCR0, TCCR0_WGM00);CLR_BIT(TCCR0, TCCR0_WGM01); break;
			case T0_2CTC 		   :CLR_BIT(TCCR0, TCCR0_WGM00);SET_BIT(TCCR0, TCCR0_WGM01); break;
			case T0_2Fast_PWM 	   :SET_BIT(TCCR0, TCCR0_WGM00);SET_BIT(TCCR0, TCCR0_WGM01); break;
			case T0_2Phase_Correct :SET_BIT(TCCR0, TCCR0_WGM00);CLR_BIT(TCCR0, TCCR0_WGM01); break;
			default : Local_u8ErrorState = NOK; break;
			}

			//***********************setting compare out match event ********************
			switch(Timer0_2_Cfg_Obj->OC_Mode)
			{
			case Timer_Disconnected :CLR_BIT(TCCR0, TCCR0_COM00);CLR_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_Toggle 		:SET_BIT(TCCR0, TCCR0_COM00);CLR_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_Clear 		:CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_SET 			:SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
			default : Local_u8ErrorState = NOK; break;
			}

			//*************************setting Prescaler*****************************
			switch(Timer0_2_Cfg_Obj->Clk_Prescaler)
			{
			case Timer_Stopped :CLR_BIT(TCCR0, TCCR0_CS00);CLR_BIT(TCCR0, TCCR0_CS01);CLR_BIT(TCCR0, TCCR0_CS02); break;
			case System_Clk    :SET_BIT(TCCR0, TCCR0_CS00);CLR_BIT(TCCR0, TCCR0_CS01);CLR_BIT(TCCR0, TCCR0_CS02); break;
			case ClkDivBy_8    :CLR_BIT(TCCR0, TCCR0_CS00);SET_BIT(TCCR0, TCCR0_CS01);CLR_BIT(TCCR0, TCCR0_CS02); break;
			case ClkDivBy_64   :SET_BIT(TCCR0, TCCR0_CS00);SET_BIT(TCCR0, TCCR0_CS01);CLR_BIT(TCCR0, TCCR0_CS02); break;
			case ClkDivBy_256  :CLR_BIT(TCCR0, TCCR0_CS00);CLR_BIT(TCCR0, TCCR0_CS01);SET_BIT(TCCR0, TCCR0_CS02); break;
			case ClkDivBy_1024 :SET_BIT(TCCR0, TCCR0_CS00);CLR_BIT(TCCR0, TCCR0_CS01);SET_BIT(TCCR0, TCCR0_CS02); break;
			case EXT_ClkSourse_FallingEdge:CLR_BIT(TCCR0, TCCR0_CS00);SET_BIT(TCCR0, TCCR0_CS01);SET_BIT(TCCR0, TCCR0_CS02); break;
			case EXT_ClkSourse_RisingEdge :SET_BIT(TCCR0, TCCR0_CS00);SET_BIT(TCCR0, TCCR0_CS01);SET_BIT(TCCR0, TCCR0_CS02); break;
			default : Local_u8ErrorState = NOK; break;
			}
		}
		else if(Copy_TimerSelect == Timer2)
		{
			//*********************setting wave generation mode *******************
			switch(Timer0_2_Cfg_Obj->WF_Gen_Mode)
			{
			case T0_2Normal 	   :CLR_BIT(TCCR2, TCCR2_WGM20);CLR_BIT(TCCR2, TCCR2_WGM21); break;
			case T0_2CTC 		   :CLR_BIT(TCCR2, TCCR2_WGM20);SET_BIT(TCCR2, TCCR2_WGM21); break;
			case T0_2Fast_PWM 	   :SET_BIT(TCCR2, TCCR2_WGM20);SET_BIT(TCCR2, TCCR2_WGM21); break;
			case T0_2Phase_Correct :SET_BIT(TCCR2, TCCR2_WGM20);CLR_BIT(TCCR2, TCCR2_WGM21); break;
			default : Local_u8ErrorState = NOK; break;
			}

			//***********************setting compare out match event ********************
			switch(Timer0_2_Cfg_Obj->OC_Mode)
			{
			case Timer_Disconnected :CLR_BIT(TCCR2, TCCR2_COM20);CLR_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_Toggle 		:SET_BIT(TCCR2, TCCR2_COM20);CLR_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_Clear 		:CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_SET 			:SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
			default : Local_u8ErrorState = NOK; break;
			}

			//*************************setting Prescaler*****************************
			switch(Timer0_2_Cfg_Obj->Clk_Prescaler)
			{
			case Timer_Stopped :CLR_BIT(TCCR2, TCCR2_CS20);CLR_BIT(TCCR2, TCCR2_CS21);CLR_BIT(TCCR2, TCCR2_CS22); break;
			case System_Clk    :SET_BIT(TCCR2, TCCR2_CS20);CLR_BIT(TCCR2, TCCR2_CS21);CLR_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_8    :CLR_BIT(TCCR2, TCCR2_CS20);SET_BIT(TCCR2, TCCR2_CS21);CLR_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_32   :SET_BIT(TCCR2, TCCR2_CS20);SET_BIT(TCCR2, TCCR2_CS21);CLR_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_64   :CLR_BIT(TCCR2, TCCR2_CS20);CLR_BIT(TCCR2, TCCR2_CS21);SET_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_128  :SET_BIT(TCCR2, TCCR2_CS20);CLR_BIT(TCCR2, TCCR2_CS21);SET_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_256  :CLR_BIT(TCCR2, TCCR2_CS20);SET_BIT(TCCR2, TCCR2_CS21);SET_BIT(TCCR2, TCCR2_CS22); break;
			case ClkDivBy_1024 :SET_BIT(TCCR2, TCCR2_CS20);SET_BIT(TCCR2, TCCR2_CS21);SET_BIT(TCCR2, TCCR2_CS22); break;
			default : Local_u8ErrorState = NOK; break;
			}
		}
		else
		{
			Local_u8ErrorState = NOK;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}



	return Local_u8ErrorState;

}

//****************************Timer 1 Initialization function***************************
/**
 *
 * @param Timer1_Cfg_Obj
 * @return Local_u8ErrorState
 */
uint8 Timer1_voidInit(const Timer1_Config_t* Timer1_Cfg_Obj)
{
	uint8 Local_u8ErrorState = OK;

	if(Timer1_Cfg_Obj != NULL)
	{
		//*********************setting wave generation mode *******************
		switch(Timer1_Cfg_Obj->Wf_Gen_Mode)
		{
		case T1_Normal :CLR_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseCorrect_8bit :SET_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseCorrect_9bit :CLR_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseCorrect_10bit :SET_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_CTC_OCR1A :CLR_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_Fast_PWM_8bit :SET_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_Fast_PWM_9bit :CLR_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_Fast_PWM_10bit :SET_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);CLR_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseFrqCorrect_ICR1 :CLR_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseFrqCorrect_OCR1A :SET_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseCorrect_ICR1 :CLR_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_PWM_PhaseCorrect_OCR1A :SET_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);CLR_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_CTC_ICR1 :CLR_BIT(TCCR1A,TCCR1A_WGM10);CLR_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_Fast_PWM_ICR1 :CLR_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		case T1_Fast_PWM_OCR1A :SET_BIT(TCCR1A,TCCR1A_WGM10);SET_BIT(TCCR1A,TCCR1A_WGM11);SET_BIT(TCCR1B,TCCR1B_WGM12);SET_BIT(TCCR1B,TCCR1B_WGM13); break;
		}

		//***********************setting compare out match event ********************
		switch(Timer1_Cfg_Obj->OC_Mode)
		{
		case Timer_Disconnected :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);CLR_BIT(TCCR1A,TCCR1A_COM1A1);CLR_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_Toggle 		:SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);CLR_BIT(TCCR1A,TCCR1A_COM1A1);CLR_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_Clear 		:CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SET 			:SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		default : Local_u8ErrorState = NOK; break;
		}

		//*************************setting Prescaler*****************************
		switch(Timer1_Cfg_Obj->Clk_Prescaler)
		{
		case Timer_Stopped :CLR_BIT(TCCR1B, TCCR1B_CS10);CLR_BIT(TCCR1B, TCCR1B_CS11);CLR_BIT(TCCR1B, TCCR1B_CS12); break;
		case System_Clk    :SET_BIT(TCCR1B, TCCR1B_CS10);CLR_BIT(TCCR1B, TCCR1B_CS11);CLR_BIT(TCCR1B, TCCR1B_CS12); break;
		case ClkDivBy_8    :CLR_BIT(TCCR1B, TCCR1B_CS10);SET_BIT(TCCR1B, TCCR1B_CS11);CLR_BIT(TCCR1B, TCCR1B_CS12); break;
		case ClkDivBy_64   :SET_BIT(TCCR1B, TCCR1B_CS10);SET_BIT(TCCR1B, TCCR1B_CS11);CLR_BIT(TCCR1B, TCCR1B_CS12); break;
		case ClkDivBy_256  :CLR_BIT(TCCR1B, TCCR1B_CS10);CLR_BIT(TCCR1B, TCCR1B_CS11);SET_BIT(TCCR1B, TCCR1B_CS12); break;
		case ClkDivBy_1024 :SET_BIT(TCCR1B, TCCR1B_CS10);CLR_BIT(TCCR1B, TCCR1B_CS11);SET_BIT(TCCR1B, TCCR1B_CS12); break;
		case EXT_ClkSourse_FallingEdge:CLR_BIT(TCCR1B, TCCR1B_CS10);SET_BIT(TCCR1B, TCCR1B_CS11);SET_BIT(TCCR1B, TCCR1B_CS12); break;
		case EXT_ClkSourse_RisingEdge :SET_BIT(TCCR1B, TCCR1B_CS10);SET_BIT(TCCR1B, TCCR1B_CS11);SET_BIT(TCCR1B, TCCR1B_CS12); break;
		default : Local_u8ErrorState = NOK; break;
		}

	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}

//-----------------------------------------------------------------------------------------
//							Timers setting compare match event function
//-----------------------------------------------------------------------------------------
/**
 *
 * @param Copy_TimerSelect
 * @param Copy_u8CompMode
 * @return Local_u8Errorstate
 */
uint8 Timer_u8SetCompMode(Timer_Select_t Copy_TimerSelect, Timers_CompOutputMode_t Copy_u8CompMode)
{
	uint8 Local_u8ErrorState = OK;

	if(Copy_TimerSelect == Timer0)
	{
		//***********************setting compare out match event ********************
		switch(Copy_u8CompMode)
		{
		case Timer_Disconnected :CLR_BIT(TCCR0, TCCR0_COM00);CLR_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_Toggle 		:SET_BIT(TCCR0, TCCR0_COM00);CLR_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_Clear 		:CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_SET 			:SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR0, TCCR0_COM00);SET_BIT(TCCR0, TCCR0_COM01); break;
		default : Local_u8ErrorState = NOK; break;
		}
	}
	else if(Copy_TimerSelect == Timer1)
	{
		//***********************setting compare out match event ********************
		switch(Copy_u8CompMode)
		{
		case Timer_Disconnected :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);CLR_BIT(TCCR1A,TCCR1A_COM1A1);CLR_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_Toggle 		:SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);CLR_BIT(TCCR1A,TCCR1A_COM1A1);CLR_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_Clear 		:CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SET 			:SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR1A,TCCR1A_COM1A0);CLR_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR1A,TCCR1A_COM1A0);SET_BIT(TCCR1A,TCCR1A_COM1B0);SET_BIT(TCCR1A,TCCR1A_COM1A1);SET_BIT(TCCR1A,TCCR1A_COM1B1); break;
		default : Local_u8ErrorState = NOK; break;
		}
	}
	else if(Copy_TimerSelect == Timer2)
	{
		//***********************setting compare out match event ********************
		switch(Copy_u8CompMode)
		{
		case Timer_Disconnected :CLR_BIT(TCCR2, TCCR2_COM20);CLR_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_Toggle 		:SET_BIT(TCCR2, TCCR2_COM20);CLR_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_Clear 		:CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_SET 			:SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_ClearOnComp_SetOnTop :CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_SetOnComp_ClearOnTop :SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_ClearOnUpCounting_SetOnDownCounting :CLR_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		case Timer_SetOnUpCounting_ClearOnDownCounting :SET_BIT(TCCR2, TCCR2_COM20);SET_BIT(TCCR2, TCCR2_COM21); break;
		default : Local_u8ErrorState = NOK; break;
		}

	}
	else

	{
		Local_u8ErrorState = NOK;
	}
	return Local_u8ErrorState;
}

//-----------------------------------------------------------------------------------------
//							Timers Enable&Disable interrupt function
//-----------------------------------------------------------------------------------------

uint8 Timer_u8EnableInt(Timer_IntSrc_t Copy_IntSrc)
{
	uint8 Local_u8ErrorState = OK;

	switch(Copy_IntSrc)
	{
	case TIMER0_OVF_INT: SET_BIT(TIMSK, TIMSK_TOIE0);  break;
	case TIMER0_OC_INT : SET_BIT(TIMSK, TIMSK_OCIE0);  break;
	case TIMER1_OVF_INT: SET_BIT(TIMSK, TIMSK_TOIE1);  break;
	case TIMER1_OCA_INT: SET_BIT(TIMSK, TIMSK_OCIE1A); break;
	case TIMER1_OCB_INT: SET_BIT(TIMSK, TIMSK_OCIE1B); break;
	case TIMER1_CAPT_INT:SET_BIT(TIMSK, TIMSK_TICIE1); break;
	case TIMER2_OVF_INT: SET_BIT(TIMSK, TIMSK_TOIE2);  break;
	case TIMER2_OC_INT : SET_BIT(TIMSK, TIMSK_OCIE2);  break;
	default : Local_u8ErrorState = NOK;
	}

	return Local_u8ErrorState;
}

uint8 Timer_u8DisableInt(Timer_IntSrc_t Copy_IntSrc)
{
	uint8 Local_u8ErrorState = OK;

	switch(Copy_IntSrc)
		{
		case TIMER0_OVF_INT: CLR_BIT(TIMSK, TIMSK_TOIE0);  break;
		case TIMER0_OC_INT : CLR_BIT(TIMSK, TIMSK_OCIE0);  break;
		case TIMER1_OVF_INT: CLR_BIT(TIMSK, TIMSK_TOIE1);  break;
		case TIMER1_OCA_INT: CLR_BIT(TIMSK, TIMSK_OCIE1A); break;
		case TIMER1_OCB_INT: CLR_BIT(TIMSK, TIMSK_OCIE1B); break;
		case TIMER1_CAPT_INT:CLR_BIT(TIMSK, TIMSK_TICIE1); break;
		case TIMER2_OVF_INT: CLR_BIT(TIMSK, TIMSK_TOIE2);  break;
		case TIMER2_OC_INT : CLR_BIT(TIMSK, TIMSK_OCIE2);  break;
		default : Local_u8ErrorState = NOK;
		}

	return Local_u8ErrorState;
}
/**
 *
 * @param Copu_u8IntSource
 * @param Copy_pvCallBackFunc
 * @return Local_u8ErrorState
 */
uint8 Timer_u8SetCallBack(Timer_IntSrc_t Copu_u8IntSource, void(*Copy_pvCallBackFunc)(void))
{
	uint8 Local_u8ErrorState = OK;

	if(Copy_pvCallBackFunc!=NULL)
	{
		Timer_pvCallBackFunc[Copu_u8IntSource]=Copy_pvCallBackFunc;
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}

void Timer0_voidSetPerloadValue(uint8 Copy_u8PreloadVal)
{
	TCNT0=Copy_u8PreloadVal;
}

void Timer0_voidCompareMatchValue(uint8 Copy_u8CompMatchVal)
{
	OCR0=Copy_u8CompMatchVal;
}

void Timer1_voidSetTopValue(uint16 Copy_u16TopVal)
{
	ICR1=Copy_u16TopVal;
}

void Timer1_voidCompareMatchValue(uint16 Copy_u16CompMatchVal)
{
	OCR1A=Copy_u16CompMatchVal;
	OCR1B=Copy_u16CompMatchVal;

}

void Timer1_voidICU_Init(void)
{
	/* Sitting input capture edge select to  rising edge */
		SET_BIT(TCCR1B, TCCR1B_ICES1);

		/*enable Input Capture interrupt*/
		SET_BIT(TIMSK, TIMSK_TICIE1);
}

void Timer1_voidSetIcuEdgeSelect(uint8 Copy_u8EdgeSelect)
{
	switch(Copy_u8EdgeSelect)
	{
	case FALLING_EDGE: CLR_BIT(TCCR1B, TCCR1B_ICES1); break;
	case RISING_EDGE: SET_BIT(TCCR1B, TCCR1B_ICES1); break;
	}
}

uint16 Timer1_u16GetTimervalue(void)
{
	return ICR1;
}

void Timer2_voidSetPerloadValue(uint8 Copy_u8PreloadVal)
{
	TCNT2=Copy_u8PreloadVal;
}

void Timer2_voidCompareMatchValue(uint8 Copy_u8CompMatchVal)
{
	OCR2=Copy_u8CompMatchVal;
}


//*******************************Timers ISR function*********************************************

/* Timer/Counter2 Compare Match ISR*/
void __vector_4(void) __attribute__((signal));

void __vector_4(void)
{

	if(Timer_pvCallBackFunc[TIMER2_OC_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER2_OC_INT]();
	}
	else
	{

	}
}

/* Timer/Counter2 Overflow ISR*/
void __vector_5(void) __attribute__((signal));

void __vector_5(void)
{

	if(Timer_pvCallBackFunc[TIMER2_OVF_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER2_OVF_INT]();
	}
	else
	{

	}
}

/* Timer/Counter1 Capture Event ISR*/
void __vector_6(void) __attribute__((signal));

void __vector_6(void)
{

	if(Timer_pvCallBackFunc[TIMER1_CAPT_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER1_CAPT_INT]();
	}
	else
	{

	}
}

/*Timer/Counter1 Compare Match A ISR*/
void __vector_7(void) __attribute__((signal));

void __vector_7(void)
{

	if(Timer_pvCallBackFunc[TIMER1_OCA_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER1_OCA_INT]();
	}
	else
	{

	}
}

/*Timer/Counter1 Compare Match B ISR*/
void __vector_8(void) __attribute__((signal));

void __vector_8(void)
{

	if(Timer_pvCallBackFunc[TIMER1_OCB_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER1_OCB_INT]();
	}
	else
	{

	}
}

/*Timer/Counter1 Overflow ISR*/
void __vector_9(void) __attribute__((signal));

void __vector_9(void)
{

	if(Timer_pvCallBackFunc[TIMER1_OVF_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER1_OVF_INT]();
	}
	else
	{

	}
}

/*Timer/Counter0 Compare Match*/
void __vector_10(void) __attribute__((signal));

void __vector_10(void)
{

	if(Timer_pvCallBackFunc[TIMER0_OC_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER0_OC_INT]();
	}
	else
	{

	}
}

/*Timer0 overflow ISR*/
void __vector_11(void) __attribute__((signal));

void __vector_11(void)
{

	if(Timer_pvCallBackFunc[TIMER0_OVF_INT]!=NULL)
	{
		Timer_pvCallBackFunc[TIMER0_OVF_INT]();
	}
	else
	{

	}
}

