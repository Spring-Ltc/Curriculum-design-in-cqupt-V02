

#include "LEDandBuzz.h"


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


//��������ʼ����PA0���͵�ƽ�򿪣�
void BuzzInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);//Ĭ������ߵ�ƽ�رշ�����
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
