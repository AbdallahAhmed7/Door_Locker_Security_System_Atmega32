/*
 * DC_MOTOR.h
 *
 *      Author: ABD ALLAH
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"


typedef enum{

	STOP,CW,ACW

}DcMotor_State;


void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
