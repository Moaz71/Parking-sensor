#ifndef SERVICE_H
#define SERVICE_H


#define CPU_FREQUENCY				16u
#define PREESCALER					8u
#define COMP_VAL					2000u
#define PERIODIC 					0u
#define ONCE						1u

#define FALLING_EDGE				0u
#define RISING_EDGE 				1u



void PWM_Set(uint32 Copy_u32PeriodTime, uint32 Copy_u32OnTime);

void Schedule_ms(uint32 Copy_u32Time_ms, uint8 Copy_u8PeriodicOrOnce, void(*ScheduleFunc)(void));

void PWM_Get(uint32 *Copy_u32PeriodTime, uint32 *Copy_u32OnTime);


static void SchedulerHandler(void);
static void ICU_HW(void);





#endif
