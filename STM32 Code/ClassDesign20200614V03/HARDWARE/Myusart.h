
#ifndef __Myusart_H__
#define __Myusart_H__


#include "sys.h"


//通信设备编码
#define Stm32ID 0x11	//STM32设备
#define RaspbrryID 0x22		//树莓派设备

//数据包功能编码
#define FaceRecognitionRequest	0x00	//人脸识别请求
#define FaceRecognitionResponse  0x02		//下发人脸识别结果
#define TemperatureUpload  0x01		//体温信息上传
#define LinkACK  0xFF		//通信链接应答信号



#define MaxSendLen  50		//串口单次发送的最大字节数
#define MaxReceiveLen  50		//串口单次接收的最大字节数


extern u8  ReceiveBuff[MaxReceiveLen];	   //接收串口数据缓冲区
extern u16  ReceiveCount;             //本帧数据长度	
extern u8 ReceiveEnable;
extern u8 RequestReceive;	//请求成功标志
extern u8 AckReceive;	//收到应答标志


void MyusartInit(u32 bound);






void ClearArray(char *array,u8 len);
void ClearArray_u8(u8 *array,u8 len);

void USART_SendLenData(USART_TypeDef *USARTx,u8 *DataBuff,u16 Len);
void USART_SendString(USART_TypeDef *USARTx,char *str);



u8 DataPacking(u8 len, u8 localID,u8 destinationID,u8 IfAck,u8 FunctionCode,u8 *Data,u8 *Packed);
u8 FrameCheckIfSendAck(USART_TypeDef *USARTx,u8 *FrameReceive);
u8 SendFaceRecognitionRequest(USART_TypeDef *USARTx,u8 ResendTimes);
u8 UpdataTemperature(USART_TypeDef *USARTx,u16 Temp,u8 ResendTimes);
void DataAnalyze(u8 *DataBuff);



#endif


