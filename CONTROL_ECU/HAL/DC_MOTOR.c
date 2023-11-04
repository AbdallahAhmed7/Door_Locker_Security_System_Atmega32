/*
 * DC_MOTOR.c
 *
 *      Author: ABD ALLAH
 */

#include "DC_MOTOR.h"
#include "PWM_TIMER0.h"
#include "avr/io.h"
#include "gpio.h"


void DcMotor_Init(void)
{

GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT); // SET PB0 AS OUPUT  ( INPUT 1 FOR DC MOTOR )
GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT); // SET PB1 AS OUPUT  ( INPUT 2 FOR DC MOTOR )

GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW); // Stop the DC-Motor at the beginning
GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);


}


void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{

PORTB=(PORTB & 0xFC) | (state);

PWM_Timer0_Start( (uint8)(((uint16)speed*255)/(100)) );

}

