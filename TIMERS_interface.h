#ifndef TIMERS_INTERFACE_H
#define TIMERS_INTERFACE_H

/*********************************Timer0_2 *********************************/
typedef enum{
	T0_2Normal,
	T0_2CTC,
	T0_2Fast_PWM,
	T0_2Phase_Correct
}Timer0_2_WFGenMode_t;

/****************************Timers compare output event mode *********************/
typedef enum{
	Timer_Disconnected,
	Timer_Toggle      ,
	Timer_Clear       ,
	Timer_SET         ,
	Timer_ClearOnComp_SetOnTop,
	Timer_SetOnComp_ClearOnTop,
	Timer_ClearOnUpCounting_SetOnDownCounting,
	Timer_SetOnUpCounting_ClearOnDownCounting

}Timers_CompOutputMode_t;
/**********************************Timers prescaler *********************/
typedef enum{
	Timer_Stopped,
	System_Clk,
	ClkDivBy_8,
	ClkDivBy_32,
	ClkDivBy_64,
	ClkDivBy_128,
	ClkDivBy_256,
	ClkDivBy_1024,
	EXT_ClkSourse_FallingEdge,
	EXT_ClkSourse_RisingEdge
}Timer_Clk_t;

typedef struct{
	Timer0_2_WFGenMode_t WF_Gen_Mode;
	Timers_CompOutputMode_t OC_Mode;
	Timer_Clk_t Clk_Prescaler;

}Timer0_2_Config_t;

/*************************************Timer1 ****************************************/
typedef enum{
	T1_Normal,
	T1_PWM_PhaseCorrect_8bit,
	T1_PWM_PhaseCorrect_9bit,
	T1_PWM_PhaseCorrect_10bit,
	T1_CTC_OCR1A,
	T1_Fast_PWM_8bit,
	T1_Fast_PWM_9bit,
	T1_Fast_PWM_10bit,
	T1_PWM_PhaseFrqCorrect_ICR1,
	T1_PWM_PhaseFrqCorrect_OCR1A,
	T1_PWM_PhaseCorrect_ICR1,
	T1_PWM_PhaseCorrect_OCR1A,
	T1_CTC_ICR1,
	T1_Fast_PWM_ICR1,
	T1_Fast_PWM_OCR1A

}Timer1_WFGenMode_t;


typedef struct{
	Timer1_WFGenMode_t Wf_Gen_Mode;
	Timers_CompOutputMode_t OC_Mode;
	Timer_Clk_t Clk_Prescaler;
}Timer1_Config_t;

/*************************************Timers Interrupt Source *********************/
typedef enum{
	TIMER0_OVF_INT,
	TIMER0_OC_INT ,
	TIMER1_OVF_INT,
	TIMER1_OCA_INT ,
	TIMER1_OCB_INT ,
	TIMER1_CAPT_INT ,
	TIMER2_OVF_INT,
	TIMER2_OC_INT

}Timer_IntSrc_t;

/************************************Timer select*********************************/
typedef enum{
	Timer0,
	Timer1,
	Timer2
}Timer_Select_t;


/****************************Function Declaration**************************/
uint8 Timer0_2_voidInit(Timer_Select_t Copy_TimerSelect, const Timer0_2_Config_t* Timer0_2_Cfg_Obj);
uint8 Timer1_voidInit(const Timer1_Config_t* Timer1_Cfg_Obj);

uint8 Timer_u8SetCompMode(Timer_Select_t Copy_TimerSelect, Timers_CompOutputMode_t Copy_u8CompMode);

uint8 Timer_u8EnableInt(Timer_IntSrc_t Copy_IntSrc);
uint8 Timer_u8DisableInt(Timer_IntSrc_t Copy_IntSrc);

void Timer0_voidSetPerloadValue(uint8 Copy_u8PreloadVal);
void Timer2_voidSetPerloadValue(uint8 Copy_u8PreloadVal);

void Timer1_voidSetTopValue(uint16 Copy_u16TopVal);
void Timer1_voidCompareMatchValue(uint16 Copy_u16CompMatchVal);
void Timer1_voidICU_Init(void);
uint16 Timer1_u16GetTimervalue(void);
void Timer1_voidSetIcuEdgeSelect(uint8 Copy_u8EdgeSelect);



void Timer0_voidCompareMatchValue(uint8 Copy_u8CompMatchVal);
void Timer2_voidCompareMatchValue(uint8 Copy_u8CompMatchVal);






uint8 Timer_u8SetCallBack(Timer_IntSrc_t Copu_u8IntSource, void(*Copy_pvCallBackFunc)(void));

void Timer0_voidSetPerloadValue(uint8 Copy_u8PreloadVal);
void Timer0_voidCompareMatchValue(uint8 Copy_u8CompMatchVal);










#endif
