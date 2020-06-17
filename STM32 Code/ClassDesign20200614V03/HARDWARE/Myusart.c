

#include "Myusart.h"
#include "Timeout.h"
#include "string.h"	
//DMA1的通道4为USART1_TX;通道5为USART1_RX

u8  ReceiveBuff[MaxReceiveLen];	   //接收串口数据缓冲区
u16  ReceiveCount=0;             //本帧数据长度	
u8 AckPacked[]={0xAA,0x55,0x06,0x11,0x22,0x00,0xFF,0xFF,0xFF,0xCA,0x55,0xAA};
u8 StuNumber[10]={0};//存储收到的学号
u8 RequestReceive=0;	//请求成功标志
u8 AckReceive=0;	//收到应答标志


void MyusartInit(u32 bound)
{
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

 //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}
//Function OK

u8 ReceiveEnable=0;//状态机状态标志
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
			
		ReceiveBuff[ReceiveCount++] = Res;//存到缓存区
		if(ReceiveEnable != 2)	//状态：定时器未打开
		{
			ReceiveCount=0;
			switch (ReceiveEnable)
			{
				case 0://未收到AA
				{
					if(Res == 0xAA)
						ReceiveEnable = 1;
					break;
				}
				case 1://收到AA
				{
					if(Res == 0x55)
					{
						ReceiveEnable =2;
						ReceiveBuff[0]=0xAA,ReceiveBuff[1]=0x55;//确认收到头部，手动添加到缓存数组
						ReceiveCount=2;//下一个数据从RX1_Buffer[2]开始缓存
						TIM_Cmd(TIM2, ENABLE);  //使能TIMx	
					}
					else if(Res == 0xAA)	ReceiveEnable = 1;
					else ReceiveEnable = 0;
					break;
				}
				default:	break;
			}		 
		} 
	}
}
//Function OK


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
//Function OK

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
u8 FrameCheckIfSendAck(USART_TypeDef *USARTx,u8 *FrameReceive)
{
	u8 i = 0,datalen = 0,Parity = 0;
	u8 *point = FrameReceive;
	u8 IfAck=*(point+5);//功能码1，是否需要应答信号
	
	if( (*(point) != 0xAA) || (*(point+1) != 0x55))
		return 1;//数据包头错误
	
	datalen = *(point+2);//获取数据段长度（不包含功能码）
	if( (*(point+datalen+4) != 0x55) || (*(point+datalen+5) != 0xAA))
		return 2;//数据包尾错误
	
	Parity = datalen;
	for(i=3;i<datalen+3;i++)//计算校验和
		Parity =Parity ^ *(point+i);//按位异或运算
	if(Parity != *(point+datalen+3))
		return 3;//校验和错误
	
	if(IfAck)
		USART_SendLenData(USARTx,AckPacked,12);
	return 0;//校验成功
}
//Functiona OK





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
	
	while(--ResendTimes)
	{
		AckTimeoutEnable=1;//启动应答超时计数
		AckTimeoutCount=0;
		AckReceive=0;
		USART_SendLenData(USART1,sendstr,sendlen);//通过串口发送请求
		while(AckTimeoutCount==0)
		{
			if(AckReceive==1)//收到应答了
			{
				AckReceive=0;//清除标志
				break;
			}
		}
		if(AckTimeoutCount>1)//应答超时
		{
			AckTimeoutEnable=0;
			continue;
		}
	
		//收到应答才执行下面程序
		RequestTimeoutEnable=1;//启动请求超时计数
		RequestTimeoutCount=0;//从0开始计数
		RequestReceive=0;
		while(RequestTimeoutCount<5)
		{
			if(RequestReceive==1)//收到了请求
				break;
		}
		if(RequestReceive==1)//收到了请求,就不用重新请求了
		{
			RequestReceive=0;//清除标志
			return 0;
		}
	}
	return 1;//超时，为收到人脸识别的结果
}


//--------------------------函数说明--------------------------------------------//
//函数功能：	数据校验成功后调用该函数解析数据
//入口参数：	*DataBuff:需要解析的数据帧地址
//返回值：		无

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/16
//修改内容：初次完成基本功能
//----------------------------
void DataAnalyze(u8 *DataBuff)
{
	u8 i=0;
	u8 *point;
	u8 FunctionCode = *(point+6);//功能码
	point = DataBuff;
	switch(FunctionCode)
	{
		case 0xFF://应答信息ACK
		{	
			AckReceive = 1;//已经收到应答信号
			break;
		}
		case 0x01://人脸识别结果（学号）
		{
			for(i=0;i<10;i++)
				StuNumber[i] = *(point+7+i);//写入10位字符的学号
			RequestReceive = 1;//已经收到人脸识别结果
			break;
		}
		default:break;
	}	
}



//--------------------------函数说明--------------------------------------------//
//函数功能：	向树莓派上传体温信息
//入口参数：	*USARTx：选择用哪一个串口发送
//						Temp：温度值
//					ResendTimes：未收到应答的重发次数
//返回值：	返回0表示收到对方的应答，上传成功；返回其他值表示上传失败

//作者：Liao_tingchun		
//修改日期：2020/06/14
//修改内容：初次完成基本功能
//--------------------------------
u8 UpdataTemperature(USART_TypeDef *USARTx,u16 Temp,u8 ResendTimes)
{
	u8 sendlen = 0;
	u8 data[2] = {0};
	u8 sendstr[MaxSendLen] = {0};
	
	data[0]=(u8)Temp/256;
	data[0]=(u8)Temp%256;
	
	sendlen = DataPacking(2,Stm32ID,RaspbrryID,0x01,FaceRecognitionRequest,data,sendstr);
	
	while(--ResendTimes)
	{
		AckTimeoutEnable=1;//启动应答超时计数
		AckTimeoutCount=0;
		AckReceive=0;
		USART_SendLenData(USART1,sendstr,sendlen);//通过串口发送请求
		while(AckTimeoutCount==0)
		{
			if(AckReceive==1)//收到应答了
			{
				AckReceive=0;//清除标志
				break;
			}
		}
		if(AckTimeoutCount>1)//应答超时
		{
			AckTimeoutEnable=0;
			continue;
		}
	
		//收到应答才执行下面程序
		RequestTimeoutEnable=1;//启动请求超时计数
		RequestTimeoutCount=0;//从0开始计数
		RequestReceive=0;
		while(RequestTimeoutCount<5)
		{
			if(RequestReceive==1)//收到了请求
				break;
		}
		if(RequestReceive==1)//收到了请求,就不用重新请求了
		{
			RequestReceive=0;//清除标志
			return 0;
		}
	}
	return 1;//超时，体温信息上传失败
}




