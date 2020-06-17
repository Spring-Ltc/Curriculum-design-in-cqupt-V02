
#ifndef __HDC1080_H__
#define __HDC1080_H__


#include "sys.h"

//HDC1080
/////////���㹫ʽ///////////////////////////////////////////
//temper=[(d15:d0)/2^16]*165c-40c
//relative=[(d15:d0)/2^16]*100%


//============================IICͨ�����====================================//
#define SDA_IN()  {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=0x80000000;}
#define SDA_OUT() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=0x30000000;}//PB15
#define IIC_SCL    PBout(14) //SCL
#define IIC_SDA    PBout(15) //SDA	 
#define READ_SDA   PBin(15)  //����SDA 


//============HDC1080�Ĵ���˵��==========//
#define HDC1080_Device_Adderss      0x80		//IIC���ߵ�ַ
#define HDC1080_Read_Temperature    0x00
#define HDC1080_Read_Humidity       0x01
#define HDC1080_Read_Config         0x02			//����Ĵ��������豸���ܲ�����״̬��ֻ�õ���16λ

//====================HDC1080_Read_Config�Ĵ���λ�������±꣩===========================//
#define HDC1080_Rst                 15//0x8000��λ
#define HDC1080_Enht                13//0x2000//ʹ�ܼ���
#define HDC1080_Mode                12//0x1000//����ģʽ-Ϊ0ʱ�ֿ�������Ϊ1ʱ�����������¶���ǰ
#define HDC1080_Trest               10//0x0000  0Ϊ�¶�14bit 1Ϊ11bit
#define HDC1080_Hres                8//0x0000 14 11 7bit �¶�


//=======================HDC1080��������غ���======================================//
void HDC1080_Init(void);
void HDC1080_Setting(void);
void HDC1080_Soft_Reset(void);
void HDC1080_WriteBuffer(u8 RegisterAddr,u16 DataBuffer);
void HDC1080_ReadBuffer(u8 RegisterAddr,u8 *DataBuffer,u8 len);
u16 HDC1080_Read_Temper(void);
u16 HDC1080_Read_Humidi(void);



//==================IICͨ����غ���=====================//
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


#endif

