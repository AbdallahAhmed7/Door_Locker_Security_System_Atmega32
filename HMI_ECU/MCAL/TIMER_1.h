/*
 * TIMER_1.h
 *
 *      Author: ABD ALLAH
 */

#ifndef TIMER_1_H_
#define TIMER_1_H_

#include "std_types.h"


typedef enum
{

NO_CLOCK,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,F_CPU_EXTERNAL_FALLING,F_CPU_EXTERNAL_RAISING

}Timer1_Prescaler;

typedef enum
{

NORMAL,CTC=4

}Timer1_Mode;


typedef struct
{
uint16 INITIAL_VALUE;
uint16 COMPARE_VALUE;
Timer1_Prescaler PRESCALER;
Timer1_Mode MODE;

}Timer1_ConfigType;


void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_1_H_ */
