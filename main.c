#include "STD_TYPES.h"
#include <util/delay.h>

#include "PORT_inerface.h"
#include "TIMERS_interface.h"
#include "DIO_interface.h"

#include "CLCD_interface.h"
#include "ULTRASONIC_interface.h"






void main(void)
{
	uint8 Local_u8Flag= 0u;
	uint16 Local_u16Distance = 0u, Local_u16Temp =0u;

	/*Init Timer 0 FAST PWM used to Change the Frq of buzzer sound level*/
	Timer0_2_Config_t timer0 = {
			.WF_Gen_Mode =T0_2Fast_PWM,
			.OC_Mode = Timer_ClearOnComp_SetOnTop,
			.Clk_Prescaler= ClkDivBy_64
	};


	PORT_voidInit();
	CLCD_voidInit();
	Timer0_2_voidInit(Timer0, &timer0);

	/*Get the distance from ultraSonic*/
	Local_u16Distance= ULTRASONIC_u16GetDistance();
	Local_u16Temp = Local_u16Distance;



	while(1)
	{

		/*Get the distance from ultraSonic*/
		Local_u16Distance= ULTRASONIC_u16GetDistance();
		/*Check  if the new distance = the old distance*/
		if(Local_u16Distance == Local_u16Temp)
		{

			if((Local_u8Flag ==1u)||(Local_u8Flag ==0u))
			{
				CLCD_voidGoToXY(0, 0);
				CLCD_u8SendString("Distance =");
				CLCD_voidGoToXY(11, 0);
				CLCD_voidSendNumber(Local_u16Distance);
				CLCD_u8SendString("cm");
			}
			else if(Local_u8Flag ==2)
			{
				CLCD_voidGoToXY(3, 0);
				CLCD_u8SendString("Be Careful");
				CLCD_voidGoToXY(0, 1);

				CLCD_u8SendString(" Distance =");
				CLCD_voidGoToXY(11, 1);
				CLCD_voidSendNumber(Local_u16Distance);
				CLCD_u8SendString("cm");
			}
			else
			{
				CLCD_voidGoToXY(3, 0);
				CLCD_u8SendString("Watch OUT !!");
				CLCD_voidGoToXY(0, 1);

				CLCD_u8SendString(" Distance =");
				CLCD_voidGoToXY(11, 1);
				CLCD_voidSendNumber(Local_u16Distance);
				CLCD_u8SendString("cm");
			}
		}
		else
		{
			/*Clear the LCD screen*/
			CLCD_voidSendCmd(1);
			Local_u16Temp = Local_u16Distance;

		}

		/*check the distance range and set the suitable alarm.*/
		if((Local_u16Distance>=8u)&&(Local_u16Distance<11))
		{
			Local_u8Flag = 1;
			/*Set leds low before Start*/
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN0, DIO_u8PIN_LOW);
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN1, DIO_u8PIN_LOW);

			/*Set comp value to control The ON time And Buzz sound level*/
			//  Enable this in case you wanna use PWM to control Buzzer frequency
			//Timer0_voidCompareMatchValue(64);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);
			_delay_ms(250);
			//  Enable this in case you wanna use PWM to control Buzzer frequency
			//Timer0_voidCompareMatchValue(0);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN2);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);

			_delay_ms(280);

		}
		else if((Local_u16Distance>=5u)&&(Local_u16Distance<8))
		{
			Local_u8Flag = 2;

			/*Set leds low before Start*/
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN0, DIO_u8PIN_LOW);
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN2, DIO_u8PIN_LOW);

			/*Set comp value to control The ON time And Buzz sound level*/
			//Timer0_voidCompareMatchValue(128);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);

			_delay_ms(150);
			//Timer0_voidCompareMatchValue(0);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN1);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);

			_delay_ms(150);

		}
		else if((Local_u16Distance<5))
		{
			Local_u8Flag = 3;

			/*Set leds low before Start*/
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN2, DIO_u8PIN_LOW);
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN1, DIO_u8PIN_LOW);

			/*Set comp value to control The ON time And Buzz sound level*/
			//Timer0_voidCompareMatchValue(255);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);

			_delay_ms(50);
			//Timer0_voidCompareMatchValue(0);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN0);
			DIO_u8TogglePinValue(DIO_u8PORTC, DIO_u8PIN5);

			_delay_ms(50);

		}
		else
		{
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN0, DIO_u8PIN_LOW);
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN1, DIO_u8PIN_LOW);
			DIO_u8SetPinValue(DIO_u8PORTC, DIO_u8PIN2, DIO_u8PIN_LOW);


		}

	}
}

