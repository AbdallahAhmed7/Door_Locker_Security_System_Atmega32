/*
 * BUZZER.c
 *
 *      Author: ABD ALLAH
 */

#include "BUZZER.h"


void Buzzer_init(void){

	//Set Buzzer pin as output
GPIO_setupPinDirection(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN,LOGIC_HIGH);

    //Buzzer OFF
GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN,LOGIC_LOW);

}

void Buzzer_on(void){
    //Buzzer ON
GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN,LOGIC_HIGH);
}




void Buzzer_off(void){
    //Buzzer OFF
GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN,LOGIC_LOW);
}
