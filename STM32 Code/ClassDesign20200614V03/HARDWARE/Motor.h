
#ifndef __Motor_H__
#define __Motor_H__


#include "sys.h"

#define open 	1
#define close 0


extern u8 MotorState;//当前电机的状态


#define Motor_IN1		PAout(6)
#define Motor_IN2		PAout(8)
#define Motor_IN3		PAout(11)
#define Motor_IN4		PAout(12)


void MotorInit(void);
void Motor_open(void);
void Motor_close(void);




#endif

