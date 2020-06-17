

#include "Myusart.h"
#include "Timeout.h"
#include "string.h"	
//DMA1��ͨ��4ΪUSART1_TX;ͨ��5ΪUSART1_RX

u8  ReceiveBuff[MaxReceiveLen];	   //���մ������ݻ�����
u16  ReceiveCount=0;             //��֡���ݳ���	
u8 AckPacked[]={0xAA,0x55,0x06,0x11,0x22,0x00,0xFF,0xFF,0xFF,0xCA,0x55,0xAA};
u8 StuNumber[10]={0};//�洢�յ���ѧ��
u8 RequestReceive=0;	//����ɹ���־
u8 AckReceive=0;	//�յ�Ӧ���־


void MyusartInit(u32 bound)
{
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

 //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}
//Function OK

u8 ReceiveEnable=0;//״̬��״̬��־
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
			
		ReceiveBuff[ReceiveCount++] = Res;//�浽������
		if(ReceiveEnable != 2)	//״̬����ʱ��δ��
		{
			ReceiveCount=0;
			switch (ReceiveEnable)
			{
				case 0://δ�յ�AA
				{
					if(Res == 0xAA)
						ReceiveEnable = 1;
					break;
				}
				case 1://�յ�AA
				{
					if(Res == 0x55)
					{
						ReceiveEnable =2;
						ReceiveBuff[0]=0xAA,ReceiveBuff[1]=0x55;//ȷ���յ�ͷ�����ֶ���ӵ���������
						ReceiveCount=2;//��һ�����ݴ�RX1_Buffer[2]��ʼ����
						TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx	
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
//Function OK

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





//----------------------------------����˵��--------------------------------------------//
//�������ܣ�	�����ݰ���Э������һ��֡
//��ڲ�����	len:���������ȣ��������ʶ����͹�����
//					localID�����ط����ߵı���
//					destinationID��Զ�̽����ߵı���
//					IfAck���Ƿ���ҪӦ���ź�
//					FunctionCode��������
//					*Data����Ҫ������������ĵ�ַ
//					*Packed�������洢�������ĵ�ַ
//����ֵ��	�������������֡�ĳ���
//--------------------------------��������---------------------------------------------------//
//����ReceiveBuffΪ{0x01,0x02,0x03,0x04,0x05},����Ϊ5�������溯����һ����ڲ���
//ִ�к���DataPacking(5,0x20,0x21,0x01,0x58,ReceiveBuff,packedbuff);
//packedbuff������Ϊ{AA 55 09 20 21 01 58 01 02 03 04 05 50 55 AA }
//------------------------------------------------------------------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/20
//�޸����ݣ�������ɻ�������
//----------------------------
u8 DataPacking(u8 len, u8 localID,u8 destinationID,u8 IfAck,u8 FunctionCode,u8 *Data,u8 *Packed)
{
	u8 parity=len+4;
	u16 i=0;
	*(Packed+i) = 0xAA;i++;//��Ӱ�ͷAA
	*(Packed+i) = 0x55;i++;//��Ӱ�ͷ55
	*(Packed+i) = len+4;i++;//��ӳ���len
	*(Packed+i) = localID;i++;//��ӱ���ʶ����
	*(Packed+i) = destinationID;i++;//���Զ�̽�����ʶ����
	*(Packed+i) = IfAck;i++;//��ӹ�����1���Ƿ���ҪӦ��
	*(Packed+i) = FunctionCode;i++;//��ӹ�����

	for(i=0;i<len;i++)	//�洢����	
		*(Packed+7+i) = *(Data+i);//��������
	for(i=3;i<7+len;i++)
		parity = parity ^ *(Packed+i);//����У���
	
	i=len+7;
	*(Packed+i) = parity;i++;//���У���
	*(Packed+i) = 0x55;i++;//��Ӱ�β55
	*(Packed+i) = 0xAA;i++;//��Ӱ�βAA
	return len+10;//���ش������������֡�ĳ���
}
//Function OK




//--------------------------����˵��--------------------------------------------//
//�������ܣ�	��ɶ�����֡��У�飬У��ɹ��Ļ��ڸ����յ��Ĺ������ж��Ƿ���Ӧ���ź�
//��ڲ�����	*USARTx�������Ҫ����Ӧ���źţ�ѡ������һ�����ڷ���
//						*FrameReceive:�յ��Ĵ�У�������֡��ַ
//����ֵ��	����0��ʾУ��ɹ�����������ֵ��ʾУ��ʧ��
//											��1:��ͷ����2:��β�򳤶ȴ���3:У��ʹ���
//--------------------------------��������-----------------------------------------//
//�豸�յ�	��	AA 55 09 01 20 01 57 01 02 03 04 05 7F 55 AA
//����Ӧ���룺	AA 55 06 20 01 00 72 09 7F 23 55 AA 
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/20
//�޸����ݣ�������ɻ�������
//--------------------------------
u8 FrameCheckIfSendAck(USART_TypeDef *USARTx,u8 *FrameReceive)
{
	u8 i = 0,datalen = 0,Parity = 0;
	u8 *point = FrameReceive;
	u8 IfAck=*(point+5);//������1���Ƿ���ҪӦ���ź�
	
	if( (*(point) != 0xAA) || (*(point+1) != 0x55))
		return 1;//���ݰ�ͷ����
	
	datalen = *(point+2);//��ȡ���ݶγ��ȣ������������룩
	if( (*(point+datalen+4) != 0x55) || (*(point+datalen+5) != 0xAA))
		return 2;//���ݰ�β����
	
	Parity = datalen;
	for(i=3;i<datalen+3;i++)//����У���
		Parity =Parity ^ *(point+i);//��λ�������
	if(Parity != *(point+datalen+3))
		return 3;//У��ʹ���
	
	if(IfAck)
		USART_SendLenData(USARTx,AckPacked,12);
	return 0;//У��ɹ�
}
//Functiona OK





//--------------------------����˵��--------------------------------------------//
//�������ܣ�	����ݮ�ɷ�������ʶ������
//��ڲ�����	*USARTx��ѡ������һ�����ڷ���
//					ResendTimes��δ�յ�Ӧ����ط�����
//����ֵ��	����0��ʾ�յ��Է���Ӧ�𣬷�������ɹ�����������ֵ��ʾ��������ʧ��

//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/06/14
//�޸����ݣ�������ɻ�������
//--------------------------------
u8 SendFaceRecognitionRequest(USART_TypeDef *USARTx,u8 ResendTimes)
{
	u8 sendlen = 0;
	u8 data[2] = {0xFF,0xFF};
	u8 sendstr[MaxSendLen] = {0};
	sendlen = DataPacking(2,Stm32ID,RaspbrryID,0x01,FaceRecognitionRequest,data,sendstr);
	
	while(--ResendTimes)
	{
		AckTimeoutEnable=1;//����Ӧ��ʱ����
		AckTimeoutCount=0;
		AckReceive=0;
		USART_SendLenData(USART1,sendstr,sendlen);//ͨ�����ڷ�������
		while(AckTimeoutCount==0)
		{
			if(AckReceive==1)//�յ�Ӧ����
			{
				AckReceive=0;//�����־
				break;
			}
		}
		if(AckTimeoutCount>1)//Ӧ��ʱ
		{
			AckTimeoutEnable=0;
			continue;
		}
	
		//�յ�Ӧ���ִ���������
		RequestTimeoutEnable=1;//��������ʱ����
		RequestTimeoutCount=0;//��0��ʼ����
		RequestReceive=0;
		while(RequestTimeoutCount<5)
		{
			if(RequestReceive==1)//�յ�������
				break;
		}
		if(RequestReceive==1)//�յ�������,�Ͳ�������������
		{
			RequestReceive=0;//�����־
			return 0;
		}
	}
	return 1;//��ʱ��Ϊ�յ�����ʶ��Ľ��
}


//--------------------------����˵��--------------------------------------------//
//�������ܣ�	����У��ɹ�����øú�����������
//��ڲ�����	*DataBuff:��Ҫ����������֡��ַ
//����ֵ��		��

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/16
//�޸����ݣ�������ɻ�������
//----------------------------
void DataAnalyze(u8 *DataBuff)
{
	u8 i=0;
	u8 *point;
	u8 FunctionCode = *(point+6);//������
	point = DataBuff;
	switch(FunctionCode)
	{
		case 0xFF://Ӧ����ϢACK
		{	
			AckReceive = 1;//�Ѿ��յ�Ӧ���ź�
			break;
		}
		case 0x01://����ʶ������ѧ�ţ�
		{
			for(i=0;i<10;i++)
				StuNumber[i] = *(point+7+i);//д��10λ�ַ���ѧ��
			RequestReceive = 1;//�Ѿ��յ�����ʶ����
			break;
		}
		default:break;
	}	
}



//--------------------------����˵��--------------------------------------------//
//�������ܣ�	����ݮ���ϴ�������Ϣ
//��ڲ�����	*USARTx��ѡ������һ�����ڷ���
//						Temp���¶�ֵ
//					ResendTimes��δ�յ�Ӧ����ط�����
//����ֵ��	����0��ʾ�յ��Է���Ӧ���ϴ��ɹ�����������ֵ��ʾ�ϴ�ʧ��

//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/06/14
//�޸����ݣ�������ɻ�������
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
		AckTimeoutEnable=1;//����Ӧ��ʱ����
		AckTimeoutCount=0;
		AckReceive=0;
		USART_SendLenData(USART1,sendstr,sendlen);//ͨ�����ڷ�������
		while(AckTimeoutCount==0)
		{
			if(AckReceive==1)//�յ�Ӧ����
			{
				AckReceive=0;//�����־
				break;
			}
		}
		if(AckTimeoutCount>1)//Ӧ��ʱ
		{
			AckTimeoutEnable=0;
			continue;
		}
	
		//�յ�Ӧ���ִ���������
		RequestTimeoutEnable=1;//��������ʱ����
		RequestTimeoutCount=0;//��0��ʼ����
		RequestReceive=0;
		while(RequestTimeoutCount<5)
		{
			if(RequestReceive==1)//�յ�������
				break;
		}
		if(RequestReceive==1)//�յ�������,�Ͳ�������������
		{
			RequestReceive=0;//�����־
			return 0;
		}
	}
	return 1;//��ʱ��������Ϣ�ϴ�ʧ��
}




