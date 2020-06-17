
#ifndef __Myusart_H__
#define __Myusart_H__


#include "sys.h"


void ClearArray(char *array,u8 len);
void ClearArray_u8(u8 *array,u8 len);

void USART_SendLenData(USART_TypeDef *USARTx,u8 *DataBuff,u16 Len);
void USART_SendString(USART_TypeDef *USARTx,char *str);


#endif


