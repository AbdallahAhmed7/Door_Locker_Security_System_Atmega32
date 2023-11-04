/*
 * PWM_TIMER0.c
 *
 *      Author: ABD ALLAH
 */

#include "PWM_TIMER0.h"

#include "avr/io.h"

#include "gpio.h"


void PWM_Timer0_Start(uint8 duty_cycle)
{

	TCNT0=0;
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,LOGIC_HIGH);// SET PB3/OC0 AS OUTPUT FOR PWM SIGNAL
	OCR0=duty_cycle; //set compare value to the duty cycle

	TCCR0 |=(1<<CS01) ;
	TCCR0 |=(1<<CS00) ; //PRESCALAR=64
	TCCR0 |=(1<<COM01);//NON-INVERTING MODE
	TCCR0 |= (1<<WGM01) | (1<<WGM00) ; // fast pwm mode

}
