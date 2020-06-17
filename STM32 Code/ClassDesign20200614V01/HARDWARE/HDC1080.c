
#include "delay.h"
#include "usart.h"
#include "HDC1080.h"
#include <math.h>






void HDC1080_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SCL=1;
	IIC_SDA=1;
	
	HDC1080_Soft_Reset();
	HDC1080_Setting();	
}
//Function OK


void HDC1080_Soft_Reset(void)
{
    uint16_t tempcom = 0x8000;
    HDC1080_WriteBuffer(HDC1080_Read_Config, tempcom);
    delay_ms(20);//there should be waiting for more than 15 ms.
}
//Function OK



//配置设备功能，操作HDC1080_Read_Config寄存器
void HDC1080_Setting(void)
{
    uint16_t tempcom = 0;
	
    tempcom |= 1<<HDC1080_Mode ;//连续读取数据使能
    // 温度与湿度都为14bit,默认为0
    HDC1080_WriteBuffer(HDC1080_Read_Config, tempcom);
}
//Function OK


//向传感器的指定寄存器地址写入数据（u16类型数据）
void HDC1080_WriteBuffer(u8 RegisterAddr,u16 DataBuffer)
{
	u8 Data_H = (u8)DataBuffer>>8;
	u8 Data_L = (u8)DataBuffer&0x00FF;
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss & 0xfe);//写操作，最低位为0
	IIC_Wait_Ack();
	IIC_Send_Byte(RegisterAddr);//发送寄存器地址
	IIC_Wait_Ack();
	IIC_Send_Byte(Data_H);IIC_Wait_Ack();//写入高8位
	IIC_Send_Byte(Data_L);IIC_Wait_Ack();//写入低8位
	IIC_Stop();
}
//Function OK


//向传感器的指定寄存器地址读取数据（u16类型数据）
void HDC1080_ReadBuffer(u8 RegisterAddr,u8 *DataBuffer,u8 len)
{
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss & 0xfe);//写操作，最低位为0,要先写地址
	IIC_Wait_Ack();
	IIC_Send_Byte(RegisterAddr);//发送寄存器地址
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss | 0x01);//读操作，最低位为1
	IIC_Wait_Ack();
	while(len--)
	{
		*DataBuffer = IIC_Read_Byte(0);
		DataBuffer++;
		if(len == 0)
			IIC_Ack();
		else
			IIC_NAck();
	}
	IIC_Stop();
}
//Function OK



//读取温度值
u16 HDC1080_Read_Temper(void)
{
    uint8_t buffer[2];
		u16 spring;
		HDC1080_ReadBuffer(HDC1080_Read_Temperature,buffer,2);
		spring=	(u16)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);//Temper*100
		return spring; 
}
//Function OK


//读取湿度值
u16 HDC1080_Read_Humidi(void)
{
    uint8_t buffer[2];
	
    HDC1080_ReadBuffer(HDC1080_Read_Humidity,buffer,2);
    return (u16)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);//Humidi*100
}
//Function OK




//==========================================================下面为IIC通信的基础函数=========================================//

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(4);
 	IIC_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL = 0;//钳住IIC总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;//发送IIC总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA = 1;delay_us(1);	   
	IIC_SCL = 1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL = 0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}












