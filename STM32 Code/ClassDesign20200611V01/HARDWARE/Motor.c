


#include "Motor.h"
#include "delay.h"



#define speed  2			//控制电机转速

u8 MotorState=close;	//记录当前电机的状态
u16 Motor_Step=500;		//步进电机脉冲数，控制开关角度


void MotorInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);
}
//END of Function



//开门
void Motor_open(void)
{
	u16 step=Motor_Step;
	if(MotorState == open)//本来就是打开的
		return;
	else
	{
		while(--step)
		{
			Motor_IN1=0;Motor_IN2=1;Motor_IN3=1;Motor_IN4=1;delay_ms(speed);
			Motor_IN1=1;Motor_IN2=0;Motor_IN3=1;Motor_IN4=1;delay_ms(speed);
			Motor_IN1=1;Motor_IN2=1;Motor_IN3=0;Motor_IN4=1;delay_ms(speed);
			Motor_IN1=1;Motor_IN2=1;Motor_IN3=1;Motor_IN4=0;delay_ms(speed);
		}
		MotorState = open;	
	}
}
//END of Function



//关门
void Motor_close(void)
{
	u16 step = Motor_Step;
	if(MotorState == close)//本来就是关闭的
		return;
	else
	{
		while(--step)
		{
			Motor_IN1=1;Motor_IN2=1;Motor_IN3=1;Motor_IN4=0;delay_ms(speed);
			Motor_IN1=1;Motor_IN2=1;Motor_IN3=0;Motor_IN4=1;delay_ms(speed);
			Motor_IN1=1;Motor_IN2=0;Motor_IN3=1;Motor_IN4=1;delay_ms(speed);
			Motor_IN1=0;Motor_IN2=1;Motor_IN3=1;Motor_IN4=1;delay_ms(speed);
		}
		MotorState = close;
	}
}
//END of Function

