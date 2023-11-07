
"Door Locker Security System"

The project is implemented via two AVR atmega32 with 8 MHz internal clock frequency communicating through UART.



The project has two main components:

1- HMI ECU (Human Machine Interface): It is responsible for the interaction with the user to take inputs through keypad and display messages on the LCD.

2- Control ECU: It is responsible for all the processing and decisions in the system like password checking, opening the door using DC Motor and activating the system alarm.


------------------------------------------------------------------------------------------------------------------------------------------------------------------ 



Here is the main sequence of the system:

-CREATE A SYSTEM PASSWORD: The user enters a 5-digit password, and the system will check if the password is correctly entered two times.

.

-MAIN OPTIONS: There are two main options (OPEN DOOR , CHANGE PASSWORD) which the user will choose using the keypad.

Open door: The user should enter the password and the control ECU will compare the password with the one in EEPROM and open the door if they matched.

Change password: The user should enter the old password and then create new password if the password matched.

.

-ALARM SYSTEM: if the password entered by the user to open the door or to change password is entered Wrong three times the alarm will activate and shows ERROR on the LCD.

------------------------------------------------------------------------------------------------------------------------------------------------------------------


The project is designed and implemented based on the layered architecture model as follow:

.

For HMI_ECU:

-Application layer

-HAL: LCD, KEYPAD

-MCAL: GPIO , UART, TIMER_1

.

For CONTROL_ECU:

-Application layer

-HAL: EEPROM, BUZZER, DC_MOTOR

-MCAL: GPIO , UART, TIMER_1, PWM, I2C


