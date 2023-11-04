/*
 * HMI_ECU_MAIN.c
 *
 *      Author: ABD ALLAH
 */

#include "gpio.h"
#include "keypad.h"
#include "lcd.h"
#include "TIMER_1.h"
#include "UART.h"
#include "avr/io.h"
#include <util/delay.h>


void Create_Password(void);
void Main_Options(void);
void Change_Password(void);
void Open_Door(void);
void LCD_DOOR_UNLOCK(void);
void enter_password_again(void);
void Seconds_counter(void);

#define MC1_READY 0x10
#define MC2_READY 0x20

uint8 PASSWORD1[7], PASSWORD2[7];
uint8 pass_counter=0;
uint8 pass_matched_flag=0;
uint8 unmatched_counter=0;
volatile uint8 key=0;
uint8 Timer_Count=0;


int main(void)
{

SREG |= (1<<7);

LCD_init();

UART_ConfigType UART_config={DATA_BIT_8,DISABLED,STOP_BIT_1,BAUD_RATE_9600};
UART_init(& UART_config);

Timer1_ConfigType TIMER1_config={0,7813,F_CPU_1024,CTC};
Timer1_init(&TIMER1_config);
Timer1_setCallBack(Seconds_counter);


Create_Password();
Main_Options();

while(1){

}

return 0;
}




/**************************Function responsible for creating new password*********************************/
void Create_Password(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"plz enter pass:");
	LCD_moveCursor(1,0);

	pass_counter=0;
    while(pass_counter<5)
    {

		if(KEYPAD_getPressedKey() >= '0' && KEYPAD_getPressedKey()  <= '9'){

			PASSWORD1[pass_counter] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			pass_counter++;
		}
		_delay_ms(300);

     }

    while(KEYPAD_getPressedKey() != '='){}

   PASSWORD1[5]='#';
   PASSWORD1[6]='\0';


	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"plz r-enter the");
	LCD_displayStringRowColumn(1,0,"same pass:");

	pass_counter=0;
	   while(pass_counter<5)
	    {

			if(KEYPAD_getPressedKey() >= '0' && KEYPAD_getPressedKey()  <= '9'){

				PASSWORD2[pass_counter] = KEYPAD_getPressedKey();
				LCD_displayCharacter('*');
				pass_counter++;
			}
			_delay_ms(300);

	     }
	while(KEYPAD_getPressedKey() != '='){}
	   PASSWORD2[5]='#';
	   PASSWORD2[6]='\0';

	    UART_sendByte(MC1_READY);

		while(UART_recieveByte() != MC2_READY){}

		_delay_ms(20);
		UART_sendString(PASSWORD1);

		_delay_ms(20);
		UART_sendString(PASSWORD2);

		LCD_clearScreen();
		pass_matched_flag=UART_recieveByte();

		if(pass_matched_flag==0){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Un matched pass");
			_delay_ms(2000);
			Create_Password();
		}

}




/************************Function of main menu (open door,change password)*********************************/
void Main_Options(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	_delay_ms(300);
	key=KEYPAD_getPressedKey();
	_delay_ms(20);
	UART_sendByte(key);

if(key == '+'){

		Open_Door();

	}

else if(key == '-'){
		Change_Password();
		Main_Options();
	}


}




/**********************Function responsible for Checking the password to Open the door******************/
void Open_Door(void){

	        LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Enter Pass");
			LCD_moveCursor(1,0);

	        pass_counter=0;
	        while(pass_counter<5)
	        {
	    		if(KEYPAD_getPressedKey() >= '0' && KEYPAD_getPressedKey()  <= '9'){

	    			PASSWORD1[pass_counter] = KEYPAD_getPressedKey();
	    			LCD_displayCharacter('*');
	    			pass_counter++;
	    		}
	    		_delay_ms(300);
	         }
	    	while(KEYPAD_getPressedKey() != '='){}
	    	   PASSWORD1[5]='#';
	    	   PASSWORD1[6]='\0';

	        UART_sendByte(MC1_READY);
			while(UART_recieveByte() != MC2_READY){}

			_delay_ms(20);
			UART_sendString(PASSWORD1);

			pass_matched_flag=0;
			pass_matched_flag=UART_recieveByte();

if(pass_matched_flag==1){
	            unmatched_counter=0;
				LCD_clearScreen();
				Timer_Count=0;
				Timer1_setCallBack(LCD_DOOR_UNLOCK);

				while (Timer_Count<19){}
				LCD_clearScreen();
				Timer1_setCallBack(Seconds_counter);
				Timer_Count=0;
				Main_Options();
	}
else if(pass_matched_flag==0){
				unmatched_counter++;
				if(unmatched_counter >0 && unmatched_counter <3 )
				{enter_password_again();}

	}

}




/**************************Function responsible for changing old password*********************************/
void Change_Password(void){
	            LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Enter Old Pass");
				LCD_moveCursor(1,0);

		        pass_counter=0;
		        while(pass_counter<5)
		        {
		    		if(KEYPAD_getPressedKey() >= '0' && KEYPAD_getPressedKey()  <= '9'){

		    			PASSWORD1[pass_counter] = KEYPAD_getPressedKey();
		    			LCD_displayCharacter('*');
		    			pass_counter++;
		    		}
		    		_delay_ms(300);
		         }
		    	while(KEYPAD_getPressedKey() != '='){}
		    	   PASSWORD1[5]='#';
		    	   PASSWORD1[6]='\0';

		        UART_sendByte(MC1_READY);
				while(UART_recieveByte() != MC2_READY){}

				_delay_ms(20);
				UART_sendString(PASSWORD1);

				pass_matched_flag=0;
				pass_matched_flag=UART_recieveByte();



				if(pass_matched_flag==1){
					unmatched_counter=0;
					Create_Password();
					Main_Options();
				}
				else if(pass_matched_flag==0){

					unmatched_counter++;
					if(unmatched_counter >0 && unmatched_counter <3 )
					{enter_password_again();}

				}

}



/**************************Function responsible for displaying Door status on LCD (CALL BACK FUNCTION)*****/
void LCD_DOOR_UNLOCK(void){
Timer_Count++;

/*rotates motor for 8-seconds CW and display a message on the screen “Door is Unlocking” */
if( Timer_Count >0 && Timer_Count <=8 ){
	LCD_displayStringRowColumn(0,0,"DOOR UNLOCKING ");
	}

/*hold the motor for 3-seconds.*/
else if( Timer_Count >8 && Timer_Count <=11 ){
	LCD_displayStringRowColumn(0,0,"DOOR UNLOCKED  ");
	}

/*rotates motor for 8-seconds A-CW and display a message on the screen “Door is Locking” */
else if( Timer_Count >11 && Timer_Count <=19 ){
	LCD_displayStringRowColumn(0,0,"DOOR LOCKING   ");
	}


}




/**************************Function responsible if wrong password entered/ Display "ERROR" ***************/
void enter_password_again(void){

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Wrong Password");
	LCD_displayStringRowColumn(1,0,"TRY AGAIN!");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Enter Pass:");
	LCD_moveCursor(1,0);

	pass_counter=0;
    while(pass_counter<5)
    {

		if(KEYPAD_getPressedKey() >= '0' && KEYPAD_getPressedKey()  <= '9'){

			PASSWORD1[pass_counter] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			pass_counter++;
		}
		_delay_ms(300);

     }

    while(KEYPAD_getPressedKey() != '='){}
   PASSWORD1[5]='#';
   PASSWORD1[6]='\0';

   _delay_ms(20);
   UART_sendByte(MC1_READY);
   while(UART_recieveByte() != MC2_READY){}

	_delay_ms(20);
	UART_sendString(PASSWORD1);

	pass_matched_flag=0;
	pass_matched_flag=UART_recieveByte();

if(pass_matched_flag==1){
	    unmatched_counter=0;
		LCD_clearScreen();
		Timer_Count=0;

		if(key== '+'){
		Timer1_setCallBack(LCD_DOOR_UNLOCK);
		while (Timer_Count<19){}
		LCD_clearScreen();
		Timer1_setCallBack(Seconds_counter);
		Timer_Count=0;
		Main_Options();
		}

		else if(key== '-'){
		Create_Password();
		Main_Options();
		}

}
else if(pass_matched_flag==0){
	unmatched_counter++;


	if(unmatched_counter >0 && unmatched_counter <3 )
	{
		enter_password_again();
	}
	else if(unmatched_counter ==3){
		unmatched_counter=0;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"ERROR!!");
		Timer_Count=0;
		Timer1_setCallBack(Seconds_counter);
        while(Timer_Count <10){}
        LCD_clearScreen();
        Timer_Count=0;
        Main_Options();
	 }
}

}


/**************************Function responsible for counting seconds (CALL BACK FUNCTION)****************/
void Seconds_counter(void){
Timer_Count++;
}












