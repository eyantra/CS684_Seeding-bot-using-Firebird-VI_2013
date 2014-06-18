/*
===============================================================================
 Name        : main.c
 Author      :
 Version     :
 Copyright   : Copyright (C)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// TODO: insert other include files here
#include "HardwareDefinition.h"
#include "FB6Lib.h"

#define THRESHOLD 100

// TODO: insert other definitions and declarations here
/******************Global Variables****************************/
extern uint8_t Ultrasonic[8];
/**************************************************************/

/******************Function Prototypes****************************/
void InitPeripherals(void);
void Delay1s(void);
void Delay100ms(void);
void LineSensor_ON(void);
void LineSensor_OFF(void);
/*****************************************************************/

/*===============================================================================
 Name        	 : InitPeripherals()
 Parameters		 : None
 Description 	 : This function initializes the peripherals of LPC1769 microcontroller
 	 	 	 	   and modules of Fire Bird VI robot as per the definitions in
 	 	 	 	   Hardwareprofile.h
 Preconditions	 : None
===============================================================================*/
void InitPeripherals(void)
{
	// delay to allow motor controller to initialize
	Delay100ms();

	UARTInit(2, 115200);
	//initilise motorcontroller
	InitMotorController();
	Stop();

	ResetI2C0();
	I2C0Init();
	ResetI2C1();
	I2C1Init();

	// delay to allow LCD to initialize
	Delay1s();
	InitLCD();


}

/*===============================================================================
 Name        	 : Delay();
 Parameters		 : None
 Description 	 : Generates delay of very small amount
 Preconditions	 : None
===============================================================================*/
void Delay(void)
{
	uint32_t i=0;
	for(i=0;i<100;i++);
}

/*===============================================================================
 Name        	 : Delay1s();
 Parameters		 : None
 Description 	 : Generates delay of approximately 1 Second
 Preconditions	 : None
===============================================================================*/
void Delay1s(void)
{
	volatile uint32_t i=0;
	volatile uint32_t k=0;
	volatile uint32_t j=0;
	for(k=0;k<110;k++)
	{
		for(i=0;i<60000;i++)
		{
			j++;
		}
	}
}

/*===============================================================================
 Name        	 : Delay100ms();
 Parameters		 : None
 Description 	 : Generates delay of approximately 100 milliseconds
 Preconditions	 : None
===============================================================================*/
void Delay100ms(void)
{
	volatile uint32_t i=0;
	volatile uint32_t k=0;
	volatile uint32_t j=0;
	for(k=0;k<11;k++)
	{
		for(i=0;i<60000;i++)
		{
			j++;
		}
	}
}

/*===============================================================================
 Name        	 : LineSensor_ON();
 Parameters		 : None
 Description 	 : Turns ON line sensor transmitter LED
 Preconditions	 : None
===============================================================================*/
void LineSensor_ON(void)
{
	WLTrigger=1;
	WLTriggerState = 1;		//ON
	WLTriggerUpdate();
}

/*===============================================================================
 Name        	 : LineSensor_OFF();
 Parameters		 : None
 Description 	 : Turns OFF line sensor transmitter LED
 Preconditions	 : None
===============================================================================*/
void LineSensor_OFF(void)
{
	WLTrigger=1;
	WLTriggerState = 0;		//OFF
	WLTriggerUpdate();
}


/*===============================================================================
 Name        	 : UltrasonicTrigger();
 Parameters		 : None
 Description 	 : This function starts ultrasonic sensor chaining
 Preconditions	 : None
===============================================================================*/
void UltrasonicTrigger(void)
{
	ULTrigger=1;
	ULTriggerState = 1;		//Start Chaining
	ULTriggerUpdate();
	ULTrigger=0;
}

int main(void) {

	// TODO: insert code here
	//uint32_t flag=0;
	//uint32_t y=10;
	uint32_t x=4;
	SystemInit();			/*Inits PLL and Peripheral Clocks*/	//Core Clock(CCLK) = 120MHz, All Peripheral Clock = 1/4 * CCLK
	SystemClockUpdate();	/* SystemClockUpdate() updates the SystemFrequency variable */
	InitPeripherals();
	Delay1s();
	UltrasonicTrigger();

	//LCDSetCursorPosition(1,1);

	//LCD_WriteStr(20,(uint8_t *)" OBSTACLE ");

	// Enter an infinite loop
    SetMode(0);
    DisableSafety();
    ClearEncoderCounts();
	while(1)
	{
		  while(Ultrasonic[7] > ((Ultrasonic[0]+1)*1.5)+5)
		  {
			  Get_Ultrasonic_Data(Ultrasonic);
			  Left_Velocity = 100;
			  Right_Velocity =100;
			  Move(Left_Velocity,Right_Velocity);
                  }

	                Stop();
     	   	        Delay1s();
			//USED TO UPDATE PAN SERVO POSITION 25 DEGREE.
			UpdateServoPos(25,PAN);//FOR FIRST TIME DIGGING
			Delay1s();
			//USED TO UPDATE PAN SERVO POSITION 25 TO 100 DEGREE.
			UpdateServoPos(110,PAN);
			Delay1s();
			//USED TO UPDATE AUX SERVO POSITION 95 DEGREE.
			UpdateServoPos(95,AUX);
			Delay1s();
			//USED TO UPDATE AUX SERVO POSITION 95 TO 45 DEGREE.
			UpdateServoPos(40,AUX);
			Delay1s();
			Delay1s();

		  	UpdateServoPos(25,PAN);
		  	Delay1s();		//FOR SECOND TIME DIGGING
		  	UpdateServoPos(110,PAN);
		  	Delay1s();
		  	Delay1s();
		  	Delay1s();

		while(GetLeftMotorCount() < 30*x)
		{

		        Left_Velocity = 100;
			    Right_Velocity =100;
			    Move(Left_Velocity,Right_Velocity);
                }

                ClearEncoderCounts();
	}


		return 0 ;}
