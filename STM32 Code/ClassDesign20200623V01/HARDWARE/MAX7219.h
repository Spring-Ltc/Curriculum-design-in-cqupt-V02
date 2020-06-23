
#ifndef __MAX7219_H__
#define __MAX7219_H__


#include "sys.h"

#define MAX7219_CS		  PBout(10)
#define MAX7219_DIN 		PBout(11)
#define MAX7219_CLK 		PCout(13)



//MAX7219�ڲ����ƼĴ�����ַ����
#define Mode 0x09		//����ģʽ
#define Bright 0x01		//����		������Ӳ���������ȡ�
#define Size 0x0b		//����ܸ���
#define Down 0x0c		//�ض�ģʽ
#define Text 0x0f		//���Կ���

extern u8 number[10];
//�ַ����붨��
#define guan 0x00		//�ضϲ���ʾ
#define heng 0x01		//�м���ʾ��ܼ��
//����������ܱ���


void Write7219(u8 Adress,u8 Date);	//��7219д�����ݣ�������ַ������		
void MAX7219_Init(void);	//��ʼ������7219оƬ

void Digitaltube_Close(void);
void Digitaltube_Wait(void);
void Temperature_DisplayUpdata(u16 Temp);













#endif


