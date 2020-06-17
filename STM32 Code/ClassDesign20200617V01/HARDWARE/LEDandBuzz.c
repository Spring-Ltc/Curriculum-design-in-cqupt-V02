

#include "LEDandBuzz.h"
#include "delay.h"

void RGBledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;		//RGB_B
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}


//蜂鸣器初始化（PA0，低电平打开）
void BuzzInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);//默认输出高电平关闭蜂鸣器
}
	

void LedTurnON(u8 color)
{
	switch(color)
	{
		case red:
			LedRed=0;break;
		case green:
			LedGreen=0;break;
		case blue:
			LedBlue=0;break;
	}
}
void LedTurnOFF(u8 color)
{
	switch(color)
	{
		case red:
			LedRed=1;break;
		case green:
			LedGreen=1;break;
		case blue:
			LedBlue=1;break;
	}
}

//LED闪烁,颜色和次数
void LED_Flashing(u8 color,u8 times)
{
	times++;
	while(--times)
	{
		LedTurnON(color);delay_ms(150);
		LedTurnOFF(color);delay_ms(150);
	}
}


//蜂鸣器交替性鸣叫警示
void BUZZ_Flashing(u8 times)
{
	times++;
	while(--times)
	{
		BUZZ=0;delay_ms(50);
		BUZZ=1;delay_ms(50);
	}
}



