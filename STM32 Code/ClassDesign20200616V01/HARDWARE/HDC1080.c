
#include "delay.h"
#include "usart.h"
#include "HDC1080.h"
#include <math.h>






void HDC1080_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SCL=1;
	IIC_SDA=1;
	
	HDC1080_Soft_Reset();
	HDC1080_Setting();	
}
//Function OK


void HDC1080_Soft_Reset(void)
{
    uint16_t tempcom = 0x8000;
    HDC1080_WriteBuffer(HDC1080_Read_Config, tempcom);
    delay_ms(20);//there should be waiting for more than 15 ms.
}
//Function OK



//�����豸���ܣ�����HDC1080_Read_Config�Ĵ���
void HDC1080_Setting(void)
{
    uint16_t tempcom = 0;
	
    tempcom |= 1<<HDC1080_Mode ;//������ȡ����ʹ��
    // �¶���ʪ�ȶ�Ϊ14bit,Ĭ��Ϊ0
    HDC1080_WriteBuffer(HDC1080_Read_Config, tempcom);
}
//Function OK


//�򴫸�����ָ���Ĵ�����ַд�����ݣ�u16�������ݣ�
void HDC1080_WriteBuffer(u8 RegisterAddr,u16 DataBuffer)
{
	u8 Data_H = (u8)DataBuffer>>8;
	u8 Data_L = (u8)DataBuffer&0x00FF;
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss & 0xfe);//д���������λΪ0
	IIC_Wait_Ack();
	IIC_Send_Byte(RegisterAddr);//���ͼĴ�����ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(Data_H);IIC_Wait_Ack();//д���8λ
	IIC_Send_Byte(Data_L);IIC_Wait_Ack();//д���8λ
	IIC_Stop();
}
//Function OK


//�򴫸�����ָ���Ĵ�����ַ��ȡ���ݣ�u16�������ݣ�
void HDC1080_ReadBuffer(u8 RegisterAddr,u8 *DataBuffer,u8 len)
{
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss & 0xfe);//д���������λΪ0,Ҫ��д��ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(RegisterAddr);//���ͼĴ�����ַ
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
	IIC_Start();
	IIC_Send_Byte(HDC1080_Device_Adderss | 0x01);//�����������λΪ1
	IIC_Wait_Ack();
	while(len--)
	{
		*DataBuffer = IIC_Read_Byte(0);
		DataBuffer++;
		if(len == 0)
			IIC_Ack();
		else
			IIC_NAck();
	}
	IIC_Stop();
}
//Function OK



//��ȡ�¶�ֵ
u16 HDC1080_Read_Temper(void)
{
    uint8_t buffer[2];
		u16 spring;
		HDC1080_ReadBuffer(HDC1080_Read_Temperature,buffer,2);
		spring=	(u16)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);//Temper*100
		return spring; 
}
//Function OK


//��ȡʪ��ֵ
u16 HDC1080_Read_Humidi(void)
{
    uint8_t buffer[2];
	
    HDC1080_ReadBuffer(HDC1080_Read_Humidity,buffer,2);
    return (u16)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);//Humidi*100
}
//Function OK




//==========================================================����ΪIICͨ�ŵĻ�������=========================================//

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(4);
 	IIC_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL = 0;//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL = 0;
	IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;//����IIC���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA = 1;delay_us(1);	   
	IIC_SCL = 1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}












