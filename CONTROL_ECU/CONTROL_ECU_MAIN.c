/*
 * CONTROL_ECU_MAIN.c
 *
 *      Author: ABD ALLAH
 */
#include "UART.h"
#include "TIMER_1.h"
#include "gpio.h"
#include "external_eeprom.h"
#include "I2C.h"
#include "BUZZER.h"
#include "DC_MOTOR.h"
#include "avr/io.h"
#include <util/delay.h>



void Create_Password(void);
void Main_Options(void);
void Change_Password(void);
void Open_Door(void);
void Motor_ON(void);
void receive_password_again(void);
void Seconds_counter(void);


#define MC1_READY 0x10
#define MC2_READY 0x20

uint8 PASSWORD1[7], PASSWORD2[7];
uint8 pass_matched_flag=0;
uint8 unmatched_counter=0;
volatile uint8 key=0;
uint8 Timer_Count=0;




int main(void)
{
	SREG |= (1<<7);

	Buzzer_init();

	DcMotor_Init();

	UART_ConfigType UART_config={DATA_BIT_8,DISABLED,STOP_BIT_1,BAUD_RATE_9600};
	UART_init(& UART_config);


	Timer1_ConfigType TIMER1_config={0,7813,F_CPU_1024,CTC};
	Timer1_init(&TIMER1_config);
	Timer1_setCallBack(Seconds_counter);


	Create_Password();
	Main_Options();


while(1)
{

}

return 0;
}



/**************************Function responsible for creating new password*********************************/
void Create_Password(void){

    while(UART_recieveByte() != MC1_READY){}
    _delay_ms(20);
    UART_sendByte(MC2_READY);

    UART_receiveString(PASSWORD1);

    UART_receiveString(PASSWORD2);



if(strcmp(PASSWORD1,PASSWORD2)==0){
	for(uint8 i=0; i<5;i++){
	EEPROM_writeByte((0x0010)+i, PASSWORD1[i]); //save password in eeprom
	}
	pass_matched_flag=1;
	_delay_ms(20);
	UART_sendByte(pass_matched_flag);
}

else{
    pass_matched_flag=0;
    _delay_ms(20);
    UART_sendByte(pass_matched_flag);
    Buzzer_on();
    _delay_ms(1000);
    Buzzer_off();
    Create_Password();
}

}


/************************Function of main menu (open door,change password)*********************************/
void Main_Options(void){

	key=UART_recieveByte();

if(key== '+'){
	Open_Door();
 }

else if (key== '-'){
    Change_Password();
}


}



/**********************Function responsible for Checking the password and Open the door******************/
void Open_Door(void){

	while(UART_recieveByte() != MC1_READY){}
	_delay_ms(20);
	UART_sendByte(MC2_READY);

	UART_receiveString(PASSWORD1);


	for(uint8 i=0; i<5;i++){
		EEPROM_readByte((0x0010)+i, PASSWORD2[i]);
	}

if(strcmp(PASSWORD1,PASSWORD2)==0){
		pass_matched_flag=1;
		unmatched_counter=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);

		Timer_Count=0;
		Timer1_setCallBack(Motor_ON);

		while (Timer_Count<19){}
		DcMotor_Rotate(STOP,0);
		Timer1_setCallBack(Seconds_counter);
		Timer_Count=0;
		Main_Options();
}
else{
		pass_matched_flag=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);
		unmatched_counter++;
		if(unmatched_counter >0 && unmatched_counter <3)
		{receive_password_again();}

}

}

/**************************Function responsible for changing old password*********************************/
void Change_Password(void){

	while(UART_recieveByte() != MC1_READY){}
	_delay_ms(20);
	UART_sendByte(MC2_READY);

	UART_receiveString(PASSWORD1);


	for(uint8 i=0; i<5;i++){
		EEPROM_readByte((0x0010)+i, PASSWORD2[i]);
	}

if(strcmp(PASSWORD1,PASSWORD2)==0){
		pass_matched_flag=1;
		unmatched_counter=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);

		Create_Password();
		Main_Options();
}
else{
		pass_matched_flag=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);
		unmatched_counter++;
		if(unmatched_counter >0 && unmatched_counter <3)
		{receive_password_again();}
}

}


/********************Function responsible to turn on motor to lock/unlock the door (CALL BACK FUNCTION)****/
void Motor_ON(void){

Timer_Count++;

/*rotates motor for 8-seconds CW and display a message on the screen “Door is Unlocking” */
if( Timer_Count >0 && Timer_Count <=8 ){
	DcMotor_Rotate(CW,100);
	}

/*hold the motor for 3-seconds.*/
else if( Timer_Count >8 && Timer_Count <=11 ){
	DcMotor_Rotate(STOP,0);
	}

/*rotates motor for 8-seconds A-CW and display a message on the screen “Door is Locking” */
else if( Timer_Count >11 && Timer_Count <=19 ){
	DcMotor_Rotate(ACW,100);
	}
else {
	DcMotor_Rotate(STOP,0);}

}


/**************************Function responsible if wrong password entered/ Activate the Alarm *************/
void receive_password_again(void){

	while(UART_recieveByte() != MC1_READY){}
	_delay_ms(20);
	UART_sendByte(MC2_READY);
	UART_receiveString(PASSWORD1);

	for(uint8 i=0; i<5;i++){
		EEPROM_readByte((0x0010)+i, PASSWORD2[i]);
	}

if(strcmp(PASSWORD1,PASSWORD2)==0){
		pass_matched_flag=1;
		unmatched_counter=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);

		Timer_Count=0;
		if(key== '+'){
		Timer1_setCallBack(Motor_ON);
		while (Timer_Count<19){}
		DcMotor_Rotate(STOP,0);
		Timer1_setCallBack(Seconds_counter);
		Timer_Count=0;
		Main_Options();
		}

		else if(key== '-'){
		Create_Password();
		Main_Options();
		}

 }

else{
		pass_matched_flag=0;
		_delay_ms(20);
		UART_sendByte(pass_matched_flag);
		unmatched_counter++;
		if(unmatched_counter >0 && unmatched_counter <3 )
		{
		 receive_password_again();
		}
		else if(unmatched_counter ==3)
		{
			unmatched_counter=0;
			Buzzer_on();
			Timer_Count=0;
			Timer1_setCallBack(Seconds_counter);
            while(Timer_Count <10){}
            Buzzer_off();
            Timer_Count=0;
            Main_Options();

		}
 }

}



/**************************Function responsible for counting seconds (CALL BACK FUNCTION)****************/
void Seconds_counter(void){
Timer_Count++;
}








