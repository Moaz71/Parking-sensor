#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#include "ErrType.h"


#include "DIO_interface.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"

#if CLCD_u8BIT_MODE ==FOUR_BIT
static void voidSetHalfDataPort(uint8 Copy_u8Data)
{
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D4_PIN, GET_BIT(Copy_u8Data,0));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D5_PIN, GET_BIT(Copy_u8Data,1));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D6_PIN, GET_BIT(Copy_u8Data,2));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT, CLCD_u8D7_PIN, GET_BIT(Copy_u8Data,3));

}
#endif

static void voidSendEnablePulse(void)
{
	/*********** set enable high for 2ms then low******/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8E_PIN, DIO_u8PIN_LOW);

}


/**************Prepare LCD M.C to accept command****************/
void CLCD_voidSendCmd(uint8 Copy_u8Cmd)
{
	/*** set RS low for command****/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN,DIO_u8PIN_LOW);

	/*** set R/W low for Writing****/
#if CLCD_u8RW_CONN_STS ==DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/***send the command on Data Port*****/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Cmd);
	/*********** set enable high for 2ms then low******/
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/** The higher order 4 bits  DB4~DB7 is transferred first**/
	voidSetHalfDataPort(Copy_u8Cmd>>4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Cmd);
	voidSendEnablePulse();

#else
#error  wrong CLCD_u8BIT_MODE configuration option
#endif




}

void CLCD_voidSendData(uint8 Copy_u8Data)
{
	/*** set RS HIGH for Data****/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN,DIO_u8PIN_HIGH);

	/*** set R/W low for Writing****/
#if CLCD_u8RW_CONN_STS ==DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/***send the DATA on Data Port*****/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Data);
	/*********** set enable high for 2ms then low******/
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/** The higher order 4 bits  DB4~DB7 is transferred first**/
	voidSetHalfDataPort(Copy_u8Data>>4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Data);
	voidSendEnablePulse();

#else
#error  wrong CLCD_u8BIT_MODE configuration option
#endif

}

void CLCD_voidInit(void)
{
	/*** Wait for  more than 30ms after power on***/
	_delay_ms(40);

	/**** Function set command:2line front size 5*7 ***/
#if CLCD_u8BIT_MODE ==EIGHT_BIT
	CLCD_voidSendCmd(0b00111000);

#elif CLCD_u8BIT_MODE == FOUR_BIT
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b1000);
	voidSendEnablePulse();

#endif
	/***Display on/off : DIsplay on, cursor off, blink cursor off***/
	CLCD_voidSendCmd(0b00001100);

	/*** Clear Display*/
	CLCD_voidSendCmd(1);

}

uint8 CLCD_u8SendString(const char* Copy_pchString)
{
	uint8 Local_u8ErrorState=OK;


	if(Copy_pchString!=NULL)
	{
		uint8 Local_u8LoopIterator=0;
		while(Copy_pchString[Local_u8LoopIterator]!='\0')
		{
			CLCD_voidSendData(Copy_pchString[Local_u8LoopIterator]);
			Local_u8LoopIterator++;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}



	return Local_u8ErrorState;
}

void CLCD_voidSendNumber(sint32 Copy_s32Number)
{
	char Local_chNumber[10];   /*size is 10 bec. S32 is about 2bilon 10digit number**/
	uint8 Local_u8Counter1=0, Local_u8RightDigit;
	sint8 Local_u8Counter2;
	if(Copy_s32Number<0)
	{
		/** NUmber is -ve, make it positive, print -ve sign**/
		CLCD_voidSendData('-');
		Copy_s32Number*=-1;
	}
	else if(Copy_s32Number ==0)
	{
		CLCD_voidSendData('0');
		return;
	}
	while(Copy_s32Number!=0)
	{
		Local_u8RightDigit=(uint8)((uint32)Copy_s32Number%10); /*get the right most digit*/
		Copy_s32Number/=10;   /* remove the right most digit**/
		Local_chNumber[Local_u8Counter1]=Local_u8RightDigit+'0'; /* convert to ASCII**/

		Local_u8Counter1++;

	}
	for(Local_u8Counter2=(sint8)Local_u8Counter1 -1;Local_u8Counter2>=0;Local_u8Counter2--)
	{
		CLCD_voidSendData(Local_chNumber[(uint8)Local_u8Counter2]);
	}


}


void CLCD_voidGoToXY(uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8Address ;
	Local_u8Address =(0x40*Copy_u8YPos)+Copy_u8XPos;

	/* set bit 7 for set DDRAM Address Command****/
	SET_BIT(Local_u8Address,7);

	/**Execute Set DDRAM Address command****/
	CLCD_voidSendCmd(Local_u8Address);
}


uint8 CLCD_u8SendSpecialCharacter(uint8 Copy_u8LocationNum, uint8* Copy_pu8Pattern, uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_ErrorState= OK;
	if(Copy_pu8Pattern!=NULL)
	{
		uint8 Local_u8Counter;
		uint8 Local_u8CGRAMAddress =Copy_u8LocationNum*8;
		/* set bit 7 for set CGRAM Address Command****/
		SET_BIT(Local_u8CGRAMAddress,6);

		/**Execute Set DDRAM Address command****/
		CLCD_voidSendCmd(Local_u8CGRAMAddress);

		/**write the input pattern inside CGRAM **/
		for(Local_u8Counter=0u;Local_u8Counter<8u;Local_u8Counter++)
		{
			CLCD_voidSendData(Copy_pu8Pattern[Local_u8Counter]);
		}

		/**Go back to DDRAM**/
		CLCD_voidGoToXY(Copy_u8XPos,Copy_u8YPos);

		/**Display the special pattern inside CGRAM**/
		CLCD_voidSendData(Copy_u8LocationNum);



	}
	else
	{
		Local_ErrorState = NULL_PTR_ERR;
	}



	return Local_ErrorState;
}
