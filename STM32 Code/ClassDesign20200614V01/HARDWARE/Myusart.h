
#ifndef __Myusart_H__
#define __Myusart_H__


#include "sys.h"


//ͨ���豸����
#define Stm32ID 0x11	//STM32�豸
#define RaspbrryID 0x22		//��ݮ���豸

//���ݰ����ܱ���
#define FaceRecognitionRequest	0x00	//����ʶ������
#define FaceRecognitionResponse  0x02		//�·�����ʶ����
#define TemperatureUpload  0x01		//������Ϣ�ϴ�
#define LinkACK  0xFF		//ͨ������Ӧ���ź�



#define MaxSendLen  50		//���ڵ��η��͵�����ֽ���
#define MaxReceiveLen  50		//���ڵ��ν��յ�����ֽ���



void ClearArray(char *array,u8 len);
void ClearArray_u8(u8 *array,u8 len);

void USART_SendLenData(USART_TypeDef *USARTx,u8 *DataBuff,u16 Len);
void USART_SendString(USART_TypeDef *USARTx,char *str);



u8 DataPacking(u8 len, u8 localID,u8 destinationID,u8 IfAck,u8 FunctionCode,u8 *Data,u8 *Packed);



#endif


