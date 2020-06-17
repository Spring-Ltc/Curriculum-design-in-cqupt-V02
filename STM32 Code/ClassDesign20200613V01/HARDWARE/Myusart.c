

#include "Myusart.h"





//---------------------函数说明--------------------------------------------//
//函数功能：	清空一个数组，实际上使用0填充数组覆盖原来的数据
//入口参数：	*array:	数组名称
//						len:	需要清空的前多少个数据
//返回值：无

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
//-------------------------------
void ClearArray(char *array,u8 len)
{
	while(len)
	{
		len--;
		*(array+len) = 0;
	}
}

//清空u8类型的数组,其实没必要，只是不想看到warning
void ClearArray_u8(u8 *array,u8 len)
{
	while(len)
	{
		len--;
		*(array+len) = 0;
	}
}


#define NewLine 0		//发送字符串或数据需要自动添加换行与否

//---------------------函数说明--------------------------------------------//
//函数功能：	指定串口发送一个字符串（字符串默认以 \0 作为结束符）
//入口参数：	*USARTx：	用到的串口
//						 *str:	要发送的字符串
//返回值：无
//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
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



//---------------------函数说明--------------------------------------------//
//函数功能：	指定串口发送指定长度的数据
//入口参数：	*USARTx：	用到的串口
//						 *DataBuff:	要发送的数据首地址
//						len：要发送的数据长度
//返回值：无
//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
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




























