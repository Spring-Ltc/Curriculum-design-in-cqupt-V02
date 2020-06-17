

#include "MAX7219.h"
#include "delay.h"


u8 number[10]={ 0x7e,0x30,0x6d,0x79,0x33,
								0x5b,0x5f,0x70,0x7f,0x7b};  //数字编码0到9




//向7219写入内容,包含地址和数据
void Write7219 (u8 Adress,u8 Date)
{
   u8 i;
   u8 adr=Adress,dat=Date;
   MAX7219_CS=1;
   
   for(i=8;i>=1;i--)	//发送地址
   {
      MAX7219_CLK=0;delay_us(2);
			MAX7219_DIN = ((adr&0x80)==0x00?0:1);delay_us(2);
      adr=adr<<1;
      MAX7219_CLK=1;delay_us(2);
   }
   for(i=8;i>=1;i--)	//发送数据
   {
      MAX7219_CLK=0;delay_us(2);
      MAX7219_DIN = ((dat&0x80)==0x00?0:1);delay_us(2);
      dat=dat<<1;
      MAX7219_CLK=1;delay_us(2);
   }
   MAX7219_CS=0;delay_us(2);
   MAX7219_CS=1;
}
//END of Function


//初始化7219芯片
void MAX7219_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	
   Write7219 (Mode,0x00);	//选择 No Code 
   Write7219 (Bright,0x05);	//亮度调节
   Write7219 (Size,0x05);	//数码管位数，8位全选
   Write7219 (Down,0x01);	//正常工作模式0x01（省电模式为0x00）
   Write7219 (Text,0x00);	//显示测试
}
//END of Function












