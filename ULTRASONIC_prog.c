#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>

#include "TIMERS_interface.h"
#include "DIO_interface.h"
//#include "Service.h"
#include "GIE_interface.h"

#include "ULTRASONIC_interface.h"
#include "ULTRASONIC_cfg.h"
#include "ULTRASONIC_prv.h"



static uint16 u16PeriodTicks1=0u;
static uint16 u16PeriodTicks=0u;


uint16 ULTRASONIC_u16GetDistance(void)
{
	uint16 Local_u16Distance =0u;
	uint32 Local_u16Time =0u;
	/*Set Trig pulse to older ultra sonic to get wave out*/
	DIO_u8SetPinValue(TRIG_PORT,TRIG_PIN,DIO_u8PIN_HIGH);
	_delay_us(20);
	DIO_u8SetPinValue(TRIG_PORT,TRIG_PIN,DIO_u8PIN_LOW);


	/*     */
	Echo_ICU(&Local_u16Time);
	/*340 --> sound speed m/s, Distance = Sound speed *((T1+T2)/2)*/
	Local_u16Distance = (uint16)((340u*100u*Local_u16Time)/(1000000u*2u));


	return Local_u16Distance;
}
static void Echo_ICU(uint32 *Copy_u32Time)
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

	while((u16PeriodTicks ==0));


	*Copy_u32Time = ((u16PeriodTicks-u16PeriodTicks1)*4);

}

static void ICU_HW(void)
{
	volatile static uint8 Local_u8flag=0; /*volatile because it variable define in ISR */
	Local_u8flag++;


	if(Local_u8flag == 1)
	{
		u16PeriodTicks1=Timer1_u16GetTimervalue();
		/*Set ICU trigger to falling edge*/
		Timer1_voidSetIcuEdgeSelect(FALLING_EDGE);
	}
	else if(Local_u8flag == 2)
	{
		/*Get timer reading -> period time number of ticks*/
		u16PeriodTicks=Timer1_u16GetTimervalue();
		Local_u8flag = 0u;
	}

}
