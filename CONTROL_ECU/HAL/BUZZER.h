/*
 * BUZZER.h
 *
 *      Author: ABD ALLAH
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "gpio.h"

#define BUZZER_OUTPUT_PORT PORTA_ID
#define BUZZER_OUTPUT_PIN PIN3_ID


void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);


#endif /* BUZZER_H_ */
