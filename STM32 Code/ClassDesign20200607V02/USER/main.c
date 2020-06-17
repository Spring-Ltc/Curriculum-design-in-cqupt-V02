



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
	�������⣺OLED��RC522����ͬʱ��ʼ����20200607/10��23������⣬OLED�����³�ʼ����PB8�ܽţ�
2020/06/07/11:36
	MAX7219��������ܲ�����ȫOK��֮ǰ��·��������⣬MAX7219�����ѹ��Χ��4-5.5V��,�߼��ߵ�ƽ����3.3V
	���ʹ��5V��Դ����һ
	��150���ĵ��轵�͹����ѹ���Ӷ������߼��ߵ�ƽ��ѹ��
	���γ�����֮ǰ51�ĳ�����ֲ����������STM32��Ƶ��51��ܶ౶��оƬ�н���ʱ��ͱ���ʱ�䣬�ٴ��д���ʱҪ����ʱ
2020/06/07/14:59
	L298N�����������������ֲ��ɣ����ܺ���������ȫOK��ע���ʼ����ʱ��IO��ƽ���ߣ���Ȼ����һֱ�򿪵��µ�Դ����
	���ڵ��ת�ٵ���ʱ����speed��С����Ϊ2
2020/06/07/20:25
	HDC1080��ʪ�ȴ�������ֲ��ɣ���ʪ�ȶ�ȡ������ȫOK
2020/0607/22:17
	�������⣺RFIC��⵽�Ŀ�����16������������ڣ�OLEDֻ����ʾ�ַ��ͺ��֣���Ҫ���ת��������ʾ

*****************************************************************************************************/	

#include "stm32f10x.h"

#include "Motor.h"
#include "MAX7219.h"
#include "LEDandBuzz.h"
#include "RC522.h"
#include "HDC1080.h"	//HDC1080
#include "OLED.h"
#include "delay.h"
#include "usart.h"



void OLED_DisplayInit(void);





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
	u16 MyText;
	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	uart_init(115200);//����1��USB����
	
	//��ģ��ĳ�ʼ��
	RGBledInit();
	BuzzInit();
	RC522_Init();
	OLED_Init();		
	MAX7219_Init();
	MotorInit();
	HDC1080_Init();

	
	OLED_DisplayInit();
	
	
	printf("Start \r\n");
	Write7219(1,number[1]);
	Write7219(2,number[2]);
	Write7219(3,number[3]);Write7219(4,number[4]);


//	
  while (1)
  {
		delay_ms(500);
		
		 HDC.Temperature=HDC1080_Read_Temper();
		 HDC.Humidity=HDC1080_Read_Humidi();
//		 printf("Temperature=%d  Humidity=%d \r\n",HDC.Temperature,HDC.Humidity);
		
		LedTurnON(i%3+1);delay_ms(500);LedTurnOFF(i%3+1);
		
		i++;
		

		
  

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
		printf("\t\t");
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
		printf("\r\n");
		//����ӡ����������к���ϡ�	
		//OLED_ShowString(0,48,g_ucTempbuf);
		OLED_DisplayInit();
		BUZZ=0;delay_ms(100);BUZZ=1;delay_ms(1500);delay_ms(1500);//����ˢ��ʱ����
  }
}



//OLED��ʾ��ʼ��
void OLED_DisplayInit(void)
{
		OLED_ShowCHinese(0,0,26);//��
		OLED_ShowCHinese(18,0,27);//��
		OLED_ShowString(36,0,": ");
	
		OLED_ShowCHinese(0,2,30);//��
		OLED_ShowCHinese(16,2,31);//��
		OLED_ShowString(32,2,"ID: ");
	OLED_ShowString(0,4,g_ucTempbuf);
}

