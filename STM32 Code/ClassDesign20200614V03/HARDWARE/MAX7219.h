
#ifndef __MAX7219_H__
#define __MAX7219_H__


#include "sys.h"

#define MAX7219_CS		  PBout(10)
#define MAX7219_DIN 		PBout(11)
#define MAX7219_CLK 		PCout(13)



//MAX7219内部控制寄存器地址定义
#define Mode 0x09		//解码模式
#define Bright 0x01		//亮度		【尝试硬件调节亮度】
#define Size 0x0b		//数码管个数
#define Down 0x0c		//关断模式
#define Text 0x0f		//测试控制

extern u8 number[10];
//字符编码定义
#define guan 0x00		//关断不显示
#define heng 0x01		//中间显示横杠间隔
//共阴极数码管编码


void Write7219(u8 Adress,u8 Date);	//向7219写入内容，包含地址和数据		
void MAX7219_Init(void);	//初始化两块7219芯片

void Digitaltube_Close(void);
void Digitaltube_Wait(void);
void Temperature_DisplayUpdata(u16 Temp);













#endif


