
#ifndef __HDC1080_H__
#define __HDC1080_H__


#include "sys.h"

//HDC1080
/////////计算公式///////////////////////////////////////////
//temper=[(d15:d0)/2^16]*165c-40c
//relative=[(d15:d0)/2^16]*100%


//============================IIC通信相关====================================//
#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=0x80000000;}
#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=0x30000000;}//PB15
#define IIC_SCL    PBout(14) //SCL
#define IIC_SDA    PBout(15) //SDA	 
#define READ_SDA   PBin(15)  //输入SDA 


//============HDC1080寄存器说明==========//
#define HDC1080_Device_Adderss      0x80		//IIC总线地址
#define HDC1080_Read_Temperature    0x00
#define HDC1080_Read_Humidity       0x01
#define HDC1080_Read_Config         0x02			//这个寄存器配置设备功能并返回状态。只用到低16位

//====================HDC1080_Read_Config寄存器位操作（下标）===========================//
#define HDC1080_Rst                 15//0x8000软复位
#define HDC1080_Enht                13//0x2000//使能加热
#define HDC1080_Mode                12//0x1000//工作模式-为0时分开来读，为1时可以连续读温度在前
#define HDC1080_Trest               10//0x0000  0为温度14bit 1为11bit
#define HDC1080_Hres                8//0x0000 14 11 7bit 温度


//=======================HDC1080传感器相关函数======================================//
void HDC1080_Init(void);
void HDC1080_Setting(void);
void HDC1080_Soft_Reset(void);
void HDC1080_WriteBuffer(u8 RegisterAddr,u16 DataBuffer);
void HDC1080_ReadBuffer(u8 RegisterAddr,u8 *DataBuffer,u8 len);
u16 HDC1080_Read_Temper(void);
u16 HDC1080_Read_Humidi(void);



//==================IIC通信相关函数=====================//
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


#endif

