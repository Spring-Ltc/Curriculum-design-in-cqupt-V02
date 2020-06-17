

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





//----------------------------------函数说明--------------------------------------------//
//函数功能：	将数据按照协议打包成一个帧
//入口参数：	len:数据区长度，不算身份识别码和功能码
//					localID：本地发送者的编码
//					destinationID：远程接收者的编码
//					IfAck：是否需要应答信号
//					FunctionCode：功能码
//					*Data：需要打包的数据区的地址
//					*Packed：用来存储打包结果的地址
//返回值：	打包后整个数据帧的长度
//--------------------------------函数测试---------------------------------------------------//
//假设ReceiveBuff为{0x01,0x02,0x03,0x04,0x05},长度为5，即下面函数第一个入口参数
//执行函数DataPacking(5,0x20,0x21,0x01,0x58,ReceiveBuff,packedbuff);
//packedbuff的内容为{AA 55 09 20 21 01 58 01 02 03 04 05 50 55 AA }
//------------------------------------------------------------------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/20
//修改内容：初次完成基本功能
//----------------------------
u8 DataPacking(u8 len, u8 localID,u8 destinationID,u8 IfAck,u8 FunctionCode,u8 *Data,u8 *Packed)
{
	u8 parity=len+4;
	u16 i=0;
	*(Packed+i) = 0xAA;i++;//添加包头AA
	*(Packed+i) = 0x55;i++;//添加包头55
	*(Packed+i) = len+4;i++;//添加长度len
	*(Packed+i) = localID;i++;//添加本地识别码
	*(Packed+i) = destinationID;i++;//添加远程接收者识别码
	*(Packed+i) = IfAck;i++;//添加功能码1，是否需要应答
	*(Packed+i) = FunctionCode;i++;//添加功能码

	for(i=0;i<len;i++)	//存储数据	
		*(Packed+7+i) = *(Data+i);//保存数据
	for(i=3;i<7+len;i++)
		parity = parity ^ *(Packed+i);//计算校验和
	
	i=len+7;
	*(Packed+i) = parity;i++;//添加校验和
	*(Packed+i) = 0x55;i++;//添加包尾55
	*(Packed+i) = 0xAA;i++;//添加包尾AA
	return len+10;//返回打包后整个数据帧的长度
}
//Function OK




//--------------------------函数说明--------------------------------------------//
//函数功能：	完成对数据帧的校验，校验成功的话在根据收到的功能码判断是否发送应答信号
//入口参数：	*USARTx：如果需要发送应答信号，选择用哪一个串口发送
//						*FrameReceive:收到的待校验的数据帧地址
//返回值：	返回0表示校验成功，返回其他值表示校验失败
//											（1:包头错误；2:包尾或长度错误；3:校验和错误）
//--------------------------------函数测试-----------------------------------------//
//设备收到	：	AA 55 09 01 20 01 57 01 02 03 04 05 7F 55 AA
//发送应答码：	AA 55 06 20 01 00 72 09 7F 23 55 AA 
//作者：Liao_tingchun		
//修改日期：2020/05/20
//修改内容：初次完成基本功能
//--------------------------------
//u8 FrameCheckIfSendAck(USART_TypeDef *USARTx,u8 *FrameReceive)
//{
//	u8 i = 0,datalen = 0,Parity = 0;
//	u8 *point = FrameReceive;
//	u8 IfAck=*(point+5);//功能码1，是否需要应答信号
//	
//	if( (*(point) != 0xAA) || (*(point+1) != 0x55))
//		return 1;//数据包头错误
//	
//	datalen = *(point+2);//获取数据段长度（不包含功能码）
//	if( (*(point+datalen+4) != 0x55) || (*(point+datalen+5) != 0xAA))
//		return 2;//数据包尾错误
//	
//	Parity = datalen;
//	for(i=3;i<datalen+3;i++)//计算校验和
//		Parity =Parity ^ *(point+i);//按位异或运算
//	if(Parity != *(point+datalen+3))
//		return 3;//校验和错误
//	
//	if(IfAck)
//	{
//		ClearHistoryAcked;//先清除原来应答数据包中的内容
//		AckPacking(MasterBoardID,FrameReceive);//生成最新的应答数据包
//		USART_SendLenData(USARTx,AckPacked,AckLen);
//	}
//	return 0;//校验成功
//}
////Functiona OK





//--------------------------函数说明--------------------------------------------//
//函数功能：	向树莓派发送人脸识别请求
//入口参数：	*USARTx：选择用哪一个串口发送
//					ResendTimes：未收到应答的重发次数
//返回值：	返回0表示收到对方的应答，发送请求成功；返回其他值表示发送请求失败

//作者：Liao_tingchun		
//修改日期：2020/06/14
//修改内容：初次完成基本功能
//--------------------------------
u8 SendFaceRecognitionRequest(USART_TypeDef *USARTx,u8 ResendTimes)
{
	u8 sendlen = 0;
	u8 data[2] = {0xFF,0xFF};
	u8 sendstr[MaxSendLen] = {0};
	sendlen = DataPacking(2,Stm32ID,RaspbrryID,0x01,FaceRecognitionRequest,data,sendstr);
	USART_SendLenData(USART1,sendstr,sendlen);
	
	
}


















