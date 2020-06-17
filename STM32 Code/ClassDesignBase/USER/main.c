



/********************�����ʵ��ѧ2017��08051704���6ѧ�ڿγ���Ƶײ���루�����飩************************************

MCU�ͺţ�STM32F103C8T6
���ߣ�Liao_Tingchun�����䣺2093181896@qq.com
�޸���־��
2020/06/02/
	RC522������ֲ��ɣ��Ѿ�����ģ����ԣ�������ʹ�ã���δ���ж�д����
2020/06/03/17:33	
	OLED������ֲ��ɣ���error��warning������δ����ģ�����
2020/06/07/09:55
	PCB���Ӳ��ԣ�RGB�Ʋ�����ȫOK��������������·������ѹ����������ȫOK��
	��ֲ�Ĳ��Գ���RFID��OLED���������У���ȷ��PCB�����⣬�����ƴ���
2020/06/07/10:10
	�������⣺OLED��RC522����ͬʱ��ʼ��




*****************************************************************************************************/	

#include "stm32f10x.h"

#include "LEDandBuzz.h"
#include "RC522.h"
#include "IIC.h"	//HDC1080
#include "OLED.h"
#include "delay.h"
#include "usart.h"

unsigned char g_ucTempbuf[20];


u8 BUF[12];
u8 Information[10];
u8 temp=0x5a;
u8 MeasureMode,Status,Error_ID;


typedef struct {
u16 eco2;
u16 tvoc;
u8 status;
u8 error_id;
u16 raw_data;
} ccs811_measurement_t;
ccs811_measurement_t  CCS;




typedef struct {
u16 Humidity ;       //ʪ��
u16 Temperature;     //�¶�
u16 Device_ID;
} hdc1080_measurement_t;
hdc1080_measurement_t HDC;


int main(void)
{
	unsigned char status,i;
	//unsigned int temp;
	
	
	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	uart_init(115200);//����1��USB����
	
	RGBledInit();
	BuzzInit();

	
	
	
	
	RC522_Init();
	
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	; 
	
	
//	I2C_GPIO_Config();
//		Single_ReadI2C(CCS811_Add,0x20,Information,1);  //Read CCS's information  ,ID,0x81
//		printf("Information=%0x\r\n",Information[0]);
//    Single_ReadI2C(CCS811_Add,0x23,&Information[1],2);	//FW_Boot_Version  
//		Single_ReadI2C(CCS811_Add,0x24,&Information[3],2); 	//FW_App_Version
//	Single_ReadI2C(CCS811_Add,0x00,&Status,1);
//    printf("Status1111111111=%d\r\n",Status);	//Firstly the status register is read and the APP_VALID flag is checked.
//		if(Status&0x10)	 Single_MWriteI2C_byte(CCS811_Add,0xF4,&temp,0);	//Used to transition the CCS811 state from boot to application mode, a write with no data is required.
//		Single_ReadI2C(CCS811_Add,0x00,&Status,1);
//		Single_ReadI2C(CCS811_Add,0x01,&MeasureMode,1);
//    Single_WriteI2C_byte(CCS811_Add,0x01,0x10); //Write Measure Mode Register,sensor measurement every second,no interrupt
//   // OFF_CS(); 
		delay_ms(100);
	
	
//	HDC1080_Init();
	
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	


	printf("Start \r\n");
  while (1)
  {
		delay_ms(1000);
		//BUZZ = !BUZZ;
		//BUZZ=1;

//		LedTurnON(i%3+1);delay_ms(1000);LedTurnOFF(i%3+1);
//		i++;
		
//		OLED_Clear();
//		OLED_ShowCHinese(0,0,0);//��
//		OLED_ShowCHinese(18,0,1);//��
//		OLED_ShowCHinese(36,0,2);//԰
//		OLED_ShowCHinese(54,0,3);//��
//		OLED_ShowCHinese(72,0,4);//��
//		OLED_ShowCHinese(90,0,5);//��
//		OLED_ShowCHinese(108,0,6);//��
//		OLED_ShowString(0,3,"1.3' OLED TEST");		
		
//		 HDC.Device_ID=HDC1080_Read_DeviceID();
//		 HDC.Temperature=HDC1080_Read_Temper();
//		 HDC.Humidity=HDC1080_Read_Humidi();
//		 printf("Temperature=%d  Humidity=%d \r\n",HDC.Temperature,HDC.Humidity);
//		 printf("HDC1080Device_ID=%0xH\r\n", HDC.Device_ID);
//		 
		
		
  

		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
		 if (status != MI_OK)
		 {    
				 PcdReset();
				 PcdAntennaOff(); 
				 PcdAntennaOn(); 
				 continue;
		 }		     
		 printf("��������:");
	  for(i=0;i<2;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
		}	
		//��Ѱ������ӡ�������������ϡ�
		
		
		
		 status = PcdAnticoll(g_ucTempbuf);//����ײ
		 if(status != MI_OK)
		 {    
				continue;   
		 }
		 //������ײ��ϡ�  
		 
		 
		////////����Ϊ�����ն˴�ӡ��������////////////////////////
	
		printf("�����кţ�");	//�����ն���ʾ,
		for(i=0;i<4;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
		}
		//����ӡ����������к���ϡ�	
		

  }
}




