

#include "Myusart.h"





//---------------------����˵��--------------------------------------------//
//�������ܣ�	���һ�����飬ʵ����ʹ��0������鸲��ԭ��������
//��ڲ�����	*array:	��������
//						len:	��Ҫ��յ�ǰ���ٸ�����
//����ֵ����

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//-------------------------------
void ClearArray(char *array,u8 len)
{
	while(len)
	{
		len--;
		*(array+len) = 0;
	}
}

//���u8���͵�����,��ʵû��Ҫ��ֻ�ǲ��뿴��warning
void ClearArray_u8(u8 *array,u8 len)
{
	while(len)
	{
		len--;
		*(array+len) = 0;
	}
}


#define NewLine 0		//�����ַ�����������Ҫ�Զ���ӻ������

//---------------------����˵��--------------------------------------------//
//�������ܣ�	ָ�����ڷ���һ���ַ������ַ���Ĭ���� \0 ��Ϊ��������
//��ڲ�����	*USARTx��	�õ��Ĵ���
//						 *str:	Ҫ���͵��ַ���
//����ֵ����
//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//-------------------------------
void USART_SendString(USART_TypeDef *USARTx,char *str)
{
	u16 k=0;
	while((*(str+k)) != '\0')
	{
		USART_SendData(USARTx,*(str+k));
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
		k++;
	}
	#if NewLine
			USART_SendData(USARTx,'\r');
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,'\n');
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	#endif	
}
//Function OK



//---------------------����˵��--------------------------------------------//
//�������ܣ�	ָ�����ڷ���ָ�����ȵ�����
//��ڲ�����	*USARTx��	�õ��Ĵ���
//						 *DataBuff:	Ҫ���͵������׵�ַ
//						len��Ҫ���͵����ݳ���
//����ֵ����
//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//-------------------------------
void USART_SendLenData(USART_TypeDef *USARTx,u8 *DataBuff,u16 Len)
{
	u16 k=0;
	for(k=0;k<Len;k++)
	{
		USART_SendData(USARTx,*(DataBuff+k));
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	}
	#if NewLine
			USART_SendData(USARTx,'\r');
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,'\n');
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
	#endif	
}
//Function OK




























