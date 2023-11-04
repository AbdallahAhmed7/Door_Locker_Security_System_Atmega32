/*
 * TIMER_1.c
 *
 *      Author: ABD ALLAH
*/

#include "TIMER_1.h"
#include "avr/io.h"
#include <avr/interrupt.h>


static volatile void (*CALL_BACK_PTR)(void)=NULL_PTR;

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	//SET INITIAL VALUE IN TCNT1
	TCNT1=Config_Ptr->INITIAL_VALUE;

	//SET MODE (NORMAL OR CTC)
	TCCR1A =(TCCR1A & 0xFC) | (Config_Ptr->MODE & 0x03);
	TCCR1B =(TCCR1B & 0xE7) | ((Config_Ptr->MODE >> 2) << 3);

	//SET PRESCALER
	TCCR1B =(TCCR1B & 0xF8) | (Config_Ptr->PRESCALER);

	if(Config_Ptr->MODE == 4){
	//SET COMPARE VALUE (IN CTC MODE ONLY)
	OCR1A=Config_Ptr->COMPARE_VALUE;
	//Output Compare A Match Interrupt Enable
	TIMSK |= (1<<OCIE1A);
	}

	else if (Config_Ptr->MODE == 0){
	//Overflow Interrupt Enable
	TIMSK |= (1<<TOIE1);
	}
}

void Timer1_deInit(void)
{
	//DISABLE TOIE1 AND OCIE1A
	TIMSK &= ~(1<<OCIE1A) &~ (1<<TOIE1);
}


void Timer1_setCallBack(void(*a_ptr)(void))
{
	CALL_BACK_PTR=a_ptr;
}



ISR(TIMER1_COMPA_vect){

	if(CALL_BACK_PTR != NULL_PTR)
	{
		//Call the Call Back function
		(*CALL_BACK_PTR)();
	}


}


ISR(TIMER1_OVF_vect){

	if(CALL_BACK_PTR != NULL_PTR)
	{
		//Call the Call Back function
		(*CALL_BACK_PTR)();
	}

}

