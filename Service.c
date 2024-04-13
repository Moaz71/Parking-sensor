#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ErrType.h"
#include "Service.h"

#include "CLCD_interface.h"

#include "TIMERS_interface.h"
#include "TIMERS_reg.h"
#include "GIE_interface.h"


static void (*Schedule_Func)(void) =NULL;
static uint32 Time = 0u;
static uint8 Periodicity =ONCE;

static uint16 u16PeriodTicks1=0u;
static uint16 u16PeriodTicks=0u;

static uint16 u16OnTicks=0u;


/**
 *
 * @param Copy_u32PeriodTime
 * @param Copy_u32OnTime
 */
void PWM_Set(uint32 Copy_u32PeriodTime, uint32 Copy_u32OnTime)
{

	uint32 Local_u32TopValue, Local_u32CompVal;

	/*Set Initial config for Timer 1*/
	Timer1_Config_t PWM_SET ={
			.Wf_Gen_Mode=T1_Fast_PWM_ICR1,
			.OC_Mode=Timer_ClearOnComp_SetOnTop,
			.Clk_Prescaler=ClkDivBy_8

	};

	Timer1_voidInit(&PWM_SET);

	/*Calculating Top value*/
	Local_u32TopValue= ((Copy_u32PeriodTime*1000u*CPU_FREQUENCY)/PREESCALER);
	Timer1_voidSetTopValue(Local_u32TopValue);

	/*calculating Comp value*/
	Local_u32CompVal= ((Copy_u32OnTime*1000u*CPU_FREQUENCY)/PREESCALER);
	Timer1_voidCompareMatchValue(Local_u32CompVal);

}

void Schedule_ms(uint32 Copy_u32Time_ms, uint8 Copy_u8PeriodicOrOnce, void(*ScheduleFunc)(void))
{

	/*Set Initial config for Timer 1*/
	/*Timer1_Config_t CTC_SET ={
			.Wf_Gen_Mode=T1_CTC_OCR1A,
			.OC_Mode=Timer_Toggle,
			.Clk_Prescaler=ClkDivBy_8

	};

	Timer1_voidInit(&CTC_SET);
	Timer_u8EnableInt(TIMER1_OCA_INT);*/

	Timer0_2_Config_t CTC_SET ={
			.WF_Gen_Mode=T0_2CTC,
			.OC_Mode=Timer_Disconnected,
			.Clk_Prescaler=ClkDivBy_64
	};
	Timer0_2_voidInit(Timer0,&CTC_SET);
	Timer_u8EnableInt(TIMER0_OC_INT);
	GIE_voidEnableGlobal();  // service shouldn't make En GIE it self u should make a precondition in comment "if u wanna use this service u should enable GIE"

	/*calculation for ctc mode which user can schedule 1ms at least Comp time = 2000"compare value" * 1/2 "prescaler/Cpu clock" */
	//Timer1_voidCompareMatchValue(2000);
	Timer0_voidCompareMatchValue(250);

	/*Convert Local Schedule_ms into global*/
	Schedule_Func =ScheduleFunc;

	/*Convert Local Time_ms into global*/
	Time=Copy_u32Time_ms;

	/*Convert Local Perodic into global*/

	Periodicity =Copy_u8PeriodicOrOnce;

	if(ScheduleFunc != NULL)
	{
		//Timer_u8SetCallBack(TIMER1_OCA_INT,&SchedulerHandler);
		Timer_u8SetCallBack(TIMER0_OC_INT,&SchedulerHandler);
	}
	else
	{
		//Error
	}


}

static void SchedulerHandler(void)
{
	static uint32 Local_u32Counter =0;
	//CLCD_voidInit();
	uint32 Local_u32CompCount =0u;
	//Local_u32CompCount=((2*Time*1000)/COMP_VAL);
	Local_u32CompCount=((Time*1000)/1000);

	//CLCD_voidSendNumber(Local_u32Counter);
	Local_u32Counter++;
	if(Local_u32Counter<Local_u32CompCount)
	{
		/*do nothing*/
		/*wait until reach desired time*/
	}
	else
	{
		Schedule_Func();
		if(Periodicity == PERIODIC)
		{
			Local_u32Counter = 0u;
		}
		else
		{
			//Timer_u8DisableInt(TIMER1_OCA_INT);
			Timer_u8DisableInt(TIMER0_OC_INT);

		}

	}
}


void PWM_Get(uint32 *Copy_u32PeriodTime, uint32 *Copy_u32OnTime)
{

	/*Initial timer 1*/
	Timer1_Config_t ICU_SET ={
			.Wf_Gen_Mode = T1_Normal,
			.OC_Mode = Timer_Disconnected,
			.Clk_Prescaler = ClkDivBy_64
	};

	Timer1_voidInit(&ICU_SET);
	Timer_u8SetCallBack(TIMER1_CAPT_INT, &ICU_HW);

	Timer1_voidICU_Init();
	Timer_u8EnableInt(TIMER1_CAPT_INT);
	GIE_voidEnableGlobal();

	while((u16PeriodTicks ==0)||(u16OnTicks==0));

	*Copy_u32OnTime = (u16OnTicks-u16PeriodTicks)*4;
	*Copy_u32PeriodTime = (u16PeriodTicks-u16PeriodTicks1)*4;

}

static void ICU_HW(void)
{
	volatile static uint8 Local_u8flag=0; /*volatile because it variable define in ISR */
	Local_u8flag++;


	if(Local_u8flag == 1)
	{
		u16PeriodTicks1=Timer1_u16GetTimervalue();
	}
	else if(Local_u8flag == 2)
	{
		/*Get timer reading -> period time number of ticks*/
		u16PeriodTicks=Timer1_u16GetTimervalue();

		//u16PeriodTicks -= u16PeriodTicks1;
		/*Set ICU trigger to falling edge*/
		Timer1_voidSetIcuEdgeSelect(FALLING_EDGE);


	}
	else
	{

		u16OnTicks=Timer1_u16GetTimervalue();
		//u16OnTicks-=u16PeriodTicks;
		//Timer_u8DisableInt(TIMER1_CAPT_INT);
	}

}


