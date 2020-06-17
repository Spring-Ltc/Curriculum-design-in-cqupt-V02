#include "IIC.h"
#include "delay.h"
#include "usart.h"
//#include "HDC1080.h"
#include <math.h>
//------------------------CCS811+HDC1080

 void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	SCL_H;
	SDA_H;
}
void delay_1us(u8 x)//������ʱ,iic_40K
{
	delay_us(50);
//	u8 i=20;
//	x=i*x*15;
//	while(x--);
}
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
u8 I2C_Start(void)
{
	
	  SDA_OUT();
		SDA_H; 
		delay_1us(5);	//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		SCL_H;
		delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		//if(!SDA_read) return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA_L;   //SCL���ڸߵ�ƽ��ʱ��SDA����
		delay_1us(5);
	  //if(SDA_read) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
		SDA_OUT();
		SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
		SDA_OUT();
    if(1==i)
			SDA_H;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
			SDA_L;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	  SCL_H;                    //����ʱ����
    delay_1us(5);                 //��ʱ
    SCL_L ;                  //����ʱ����
    delay_1us(5);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	uint16_t i=0;
	SDA_IN();
	SDA_H;delay_us(1);	        //�ͷ�SDA
	SCL_H;delay_us(1);         //SCL���߽��в���
	while(SDA_read)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==500)//��ʱ����ѭ��
		break;
		
	}
	if(SDA_read)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL_L; 
		return RESET;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
  delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SCL_L;
	delay_1us(5); //��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SDA_OUT();
	return SET;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
	u8 i;
	SDA_OUT();  
	SCL_L;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
		if(dat&0x80)//SDA׼��
		SDA_H;  
		else 
		SDA_L;
    SCL_H;                //����ʱ�ӣ����ӻ�����
    delay_1us(5);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL_L;                //����ʱ�ӣ���SDA׼��
    delay_1us(5); 		  //��ʱ����IICʱ��Ƶ��
		dat <<= 1;          //�Ƴ����ݵ����λ  
  }
	delay_1us(10);
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
		SDA_IN();
    SDA_H;//�ͷ�SDA�����ӻ�ʹ��
    delay_1us(5);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		  dat <<= 1;
			
      SCL_H;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA_read) //������    
		   dat |=0x01;      
       delay_1us(5);     //��ʱ����IICʱ��Ƶ��		
       SCL_L;           //����ʱ���ߣ�������յ�������
       delay_1us(5);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();   //����ֹͣ�ź�
		
		return SET;
}

u8 Single_MWriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop();return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop();return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop();return RESET;}
 
	while(length)
	{
		I2C_SendByte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(!I2C_WaitAck()){I2C_Stop(); return RESET;}           //Ӧ��
		length--;
	}
	//	I2C_SendByte(*data);       //�ڲ��Ĵ������ݣ�
 	//	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		I2C_Stop();   //����ֹͣ�ź�		
		return SET;
}

//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop();return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
		if(!I2C_WaitAck()){I2C_Stop()	;return RESET;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop();return RESET;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2C_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return SET;
}



//void CS_EN()
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOAʱ��

//  //GPIOD13��ʼ������
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
//	
//	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
//}

//void ON_CS()
//{
//	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
//}

//void OFF_CS()
//{
//	GPIO_SetBits(GPIOD,GPIO_Pin_13);
//}


//-----------------------------------------------------------------//
//HDC1080

u8 HDC1080_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
    if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}  
	
		
    I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
    I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop();return RESET;}
		
    while ( len-- )
    {
    	I2C_SendByte(*buffer);
			if(!I2C_WaitAck()){I2C_Stop();return RESET;}
    	buffer++;
    }
    I2C_Stop();
		return SET;
}

void HDC1080_Soft_Reset(void)
{
    u8 temp[2];
    temp[0] = 0x80; 
    temp[1] = 0x00;
    HDC1080_Write_Buffer(HDC1080_Read_Config, temp, 2);
    delay_ms(20);//there should be waiting for more than 15 ms.
}

void HDC1080_Setting(void)
{
    uint16_t tempcom = 0;
    uint8_t temp[2];
	
    tempcom |= 1<<HDC1080_Mode ;//������ȡ����ʹ��
    // �¶���ʪ�ȶ�Ϊ14bit
    temp[0] = (uint8_t)(tempcom >> 8); 
    temp[1] = (uint8_t)tempcom;
    HDC1080_Write_Buffer(0x02, temp, 2);
}



void HDC1080_Init(void)
{
//		u8 temp = 0;
    HDC1080_Soft_Reset();
    HDC1080_Setting();
}


u8 HDC1080_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{	
//    uint8_t temp = 0;
	
		if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}  
		
    I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
    I2C_SendByte(addr);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		 
    I2C_Stop();
//    
//    while( pin_SCL_READ())
//    {
//        Delay_us(20);
//	temp++;
//	if ( temp >= 100 )
//	{
//	    break;
//	}
//    }
		delay_ms(1);
    I2C_Start();
		delay_ms(10);
    I2C_SendByte(HDC1080_Device_Adderss|0x01);    //read
     if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		 
    while ( len-- )
    {
    	*buffer = I2C_RecvByte();
    	buffer++;
    	if ( len == 0 )
    	   I2C_SendACK(1);
    	else
          I2C_SendACK(0);   	
    	
    }
    I2C_Stop();
		return SET;
}
/*******************************************************************************
  * @brief  HDC1080_Read_MBuffer becareful between triger and read there is a wait.
  * @param  uint8_t addr is point register
  * @param  uint8_t *buffer is the need to read data point
  * @param  uint8_t len is the read data length
  * @retval void
 *******************************************************************************/
u8 HDC1080_Read_MBuffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
   // uint8_t temp = 0;
	
   
		if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}  
		
     I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}

    
    I2C_SendByte(0X00);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();
    
//    while( pin_SCL_READ())
//    {
//        Delay_us(20);
//	temp++;
//	if ( temp >= 100 )
//	{
//	    break;
//	}
//    }
    delay_ms(10);      // after triger should wait at least 6.5ms
		if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop();return RESET;}  
		delay_ms(10);         //14λ��ת��ʱ�䣬�����
    I2C_SendByte(HDC1080_Device_Adderss|0x01);    //read
		if(!I2C_WaitAck()){I2C_Stop();return RESET;}
	
    while ( len-- )
    {
    	*buffer = I2C_RecvByte();
    	buffer++;
    	if ( len == 0 )
    	     I2C_SendACK(1);
    	else
           I2C_SendACK(0);
    	
    }
    I2C_Stop();
		return SET;
}

u16 HDC1080_Read_Temper(void)
{
    uint8_t buffer[2];
	u16 spring=0;

    HDC1080_Read_MBuffer(0x00, buffer, 2);
    //return (((u16)buffer[0]<<8)|buffer[1]);
    spring=(buffer[0]<<8)|buffer[1];
	spring /= 65536.0;
	spring*=165;
	spring-=40;
	spring*=100;
		spring=	(u16)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);//Temper*100
	return spring; 
}
/*******************************************************************************
  * @brief  HDC1080_Read_Humidity 
  * @param  void
  * @retval int16_t Humidity value
 *******************************************************************************/
u16 HDC1080_Read_Humidi(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_MBuffer(HDC1080_Read_Humidity, buffer, 2);

   // return (buffer[0]<<8)|buffer[1];
    return (u16)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);//Humidi*100
}
/*******************************************************************************
  * @brief  HDC1080_Read_Configuration 
  * @param  void
  * @retval Config value
 *******************************************************************************/
u16 HDC1080_Read_Conf(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(HDC1080_Read_Config, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_ManufacturerID 
  * @param  void
  * @retval ManufacturerID 
 *******************************************************************************/
u16 HDC1080_Read_ManufacturerID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xfe, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_DeviceID
  * @param  void
  * @retval DeviceID
 *******************************************************************************/
u16 HDC1080_Read_DeviceID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xff, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}


//-----------------------------------------------//
////-------------------MS5611-------------------------------

//void MS5611_RESET()
//{
//	u8 temp[2];
//    temp[0] = MS5611_Device_Adderss; 
//    temp[1] = MS5611_Reset_Cmd;
//    Single_MWriteI2C_byte(MS5611_Device_Adderss,MS5611_Reset_Cmd,temp,0);
//    delay_ms(20);//there should be waiting for more than 15 ms.
//}
//	

//void MS5611_Add_Gpio_Config()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE );  

//	 /* PC3-I2C_SCL��PC5-I2C_SDA*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOD, &GPIO_InitStructure); 
//	
//	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
//}

//void MS5611_Init()
//{ 
//	MS5611_Add_Gpio_Config();
//	delay_ms(10);
//	MS5611_RESET();
//}


//u8 MS5611_Prom_Read(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
//{
////		u8 temp[2];
////    temp[0] = MS5611_Device_Adderss; 
////    temp[1] = MS5611_Reset_cmd;
////	Single_MWriteI2C_byte(MS5611_Device_Adderss,0xA6,temp,0);
////	MS561_Single_ReadI2C(MS5611_Device_Adderss,);
//	 if(I2C_Start()==0)  //��ʼ�ź�
//		{I2C_Stop();return RESET;}          
//	
//	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
//		if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
//	
//	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
// 	if(!I2C_WaitAck()){I2C_Stop();return RESET;} 
//	
//	I2C_Stop();

//	if(I2C_Start()==0)  //��ʼ�ź�
//			{I2C_Stop();return RESET;}    
// 			
//	
//	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
// 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
//	
//	while(length-1)
//	{
//		*REG_data=I2C_RecvByte();       //�����Ĵ�������		
//		REG_data++;
//		I2C_SendACK(0);               //Ӧ��
//		length--;
//	}
//	*REG_data=I2C_RecvByte();
//	I2C_SendACK(1);     //����ֹͣ�����ź�
//	I2C_Stop();                    //ֹͣ�ź�
//	return SET;
//}


//u16 MS5611_Prom_Cx_Read(u8 Cx)
//{
//	u16 tmp;
//	uint8_t buffer[2];
//	tmp=MS5611_Prom_Read(MS5611_Device_Adderss,MS5611_PROM_RD_Cmd+Cx*2,buffer,2);
//	return ((buffer[0]<<8)|buffer[1]);
//}

//u32 MS5611_Dx_Read(u8 Dx)
//{
//	u8 rx_buffer[4];
//	u8 temp[2];
//	if(1==Dx)		
//	Single_MWriteI2C_byte(MS5611_Device_Adderss,MS5611_CMD_ADC_CONV+MS5611_CMD_ADC_D1+MS5611_CMD_ADC_4096,temp,0);
//	else if(2==Dx)
//	Single_MWriteI2C_byte(MS5611_Device_Adderss,MS5611_CMD_ADC_CONV+MS5611_CMD_ADC_D2+MS5611_CMD_ADC_4096,temp,0);
//		delay_ms(10);                            //conversion time is must!!!
//	MS5611_Prom_Read(MS5611_Device_Adderss,MS5611_CMD_ADC_READ,rx_buffer,3);     //the same as 
//	return ((rx_buffer[0]<<16)|(rx_buffer[1]<<8)|rx_buffer[2]);

//}







////�¶���ѹת����ʽ���е�����
//void MS5611_Temperature_Read()
//{
//		 //PROM    16bit
//		 MS5611.C0=MS5611_Prom_Cx_Read(0); 
//		 MS5611.C1=MS5611_Prom_Cx_Read(1);    
//		 MS5611.C2=MS5611_Prom_Cx_Read(2);
//		 MS5611.C3=MS5611_Prom_Cx_Read(3);
//		 MS5611.C4=MS5611_Prom_Cx_Read(4);
//		 MS5611.C5=MS5611_Prom_Cx_Read(5);
//		 MS5611.C6=MS5611_Prom_Cx_Read(6);
//		 MS5611.C7=MS5611_Prom_Cx_Read(7);
//		 
//		 //D1:presure   D2:temperature    //24bit
//		 MS5611.D1=MS5611_Dx_Read(1);
//		 MS5611.D2=MS5611_Dx_Read(2);
//		
//	//caculate
//	u32 dT;
//	u32 TEMP;

//	u32 OFF;
//	u32 SENS;
//	u32 Press;

//	u32 dT2;
//	u32 OFF2;
//	u32 SENS2;
//		dT = MS5611.D2-MS5611.C5*pow(2,8);
//		TEMP=2000+dT*MS5611.C6/pow(2,23);//Ϊ����߾��ȣ�����1000
//	
//		OFF=MS5611.C2*pow(2,16)+(MS5611.C4*dT)/pow(2,7);//������1000��
//		SENS=MS5611.C1*pow(2,15)+(MS5611.C3*dT)/pow(2,8);//����1000��
//	
//		if(TEMP<2000)//�¶Ȳ���
//		{
//			dT2=dT*dT/(pow(2,31));
//			OFF2=5*pow(TEMP-2000,2)/2; //OFF2ֻ����1000��
//			SENS2=5*pow(TEMP-2000,2)/4; //SENS2ֻ����1000��
//		}
//		else
//		{
//			dT2=0;
//			OFF2=0;
//			SENS2=0;
//		}
//	
//	
//		TEMP=TEMP-dT2;//������1000��
//		OFF=OFF-OFF2;
//		SENS=SENS-SENS2;
//	
//		Press=(MS5611.D1*SENS/pow(2,21)-OFF)/pow(2,15);//������1000��
//		printf("Temperature=%d,Pressure=%d\r\n",TEMP,Press);
//}















