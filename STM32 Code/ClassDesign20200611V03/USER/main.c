



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
2020/06/07/22:17
	�������⣺RFIC��⵽�Ŀ�����16������������ڣ�OLEDֻ����ʾ�ַ��ͺ��֣���Ҫ���ת��������ʾ
2020/06/10/22:24
	���IC���ŵ��ַ�ת����OLED��ʾ������ж��˲��ֳ�Ա�Ŀ��ţ�����OK
2020/06/11/10:00
	�޸�������RFID������ʾ��ƥ��ķ�ʽ�����ú궨���ȫ�ֱ����ķ�ʽ�������������³�Ա
2020/06/11/10:24
	����������ʾ�¶ȣ���С���㣬��ʾ����0.01��
	���������⣬��������һ��ʱ����������ʾȫ�����������Ǵ��ڴ�ӡ�������ʪ�ȶ�������RFIDҲ��������ȡ������������Ӳ���ϵ�����
2020/06/11/11:44
	��RFID��ع���д����һ�����ܺ�������������
	
	
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
#include "string.h"


///////////////////////////��¼���Ա����Ƶ��ID�ź�������������/////////////////////////
char ID_liaotingchun[8]="0F23660A";
#define LTC 0
char ID_zhongweimin[8]="00000000";//��δ¼��
#define ZWM 3
char ID_zhengwujie[8]="031202D3";
#define ZWJ 6 
char ID_haoshuyi[8]="00000000";//��δ¼��
#define HSY 9
char ID_maozimeng[8]="0F237704";
#define MZM 12
char ID_hulingqiao[8]="84F724CA";
#define HLQ 15
char ID_wangtianhui[8]="20D72F68";
#define WTH 18
char ID_zhoujiahui[8]="F0169A83";
#define ZJH 21
char ID_biance[8]="C5F2241C";
#define BC 24
///////////////////////////////////////////////////////








//�Զ��庯������
void OLED_DisplayInit(void);
void RFID_DisplayUpdata(void);




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

	char MyText[10];
int main(void)
{
	unsigned char status,i,j;

	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	uart_init(115200);//����1��USB����
	
	//��ģ��ĳ�ʼ��
	RGBledInit();//LED��
	BuzzInit();//������
	RC522_Init();//RFID��Ƶ��
	OLED_Init();	//OLEDҺ����ʾ
	MAX7219_Init();//�������ʾ
	MotorInit();//��������Ž���
	HDC1080_Init();//�����ʪ��

	
	OLED_DisplayInit();//
	Digitaltube_Close();
	
	printf("Start \r\n");



//	
  while (1)
  {
		delay_ms(500);

		LedTurnON(i%3+1);delay_ms(500);LedTurnOFF(i%3+1);
		
		i++;
		
		status = RFID_Search(MyText);
		if(status != 0)
			continue;
		

		RFID_DisplayUpdata();
		Digitaltube_Wait();
		BUZZ=0;delay_ms(100);BUZZ=1;
		HDC.Temperature=HDC1080_Read_Temper();
		Temperature_DisplayUpdata(HDC.Temperature);
		delay_ms(1500);delay_ms(1500);//����ˢ��ʱ����
		Digitaltube_Close();
  }
}



//OLED��ʾ��ʼ��
void OLED_DisplayInit(void)
{
		
		OLED_Clear();
		OLED_ShowCHinese(0,2,30);//��
		OLED_ShowCHinese(16,2,31);//��
		OLED_ShowString(32,2,"ID: ");

		OLED_ShowCHinese(0,0,26);//��
		OLED_ShowCHinese(18,0,27);//��
		OLED_ShowString(36,0,": ");

}



void RFID_DisplayUpdata(void)
{
	u8 state=1;
	OLED_DisplayInit();
	OLED_ShowString(60,2,MyText);	//������ʵIC����

	//���濪ʼ���Ѿ�¼�����Ϣ��һ�ȶ�
	state=strncmp(MyText,ID_liaotingchun,8);//��Ա1����͢��
	if(state==0)
	{
		OLED_ShowCHinese(54,0,LTC);
		OLED_ShowCHinese(72,0,LTC+1);
		OLED_ShowCHinese(90,0,LTC+2);
		return;
	}

	state=strncmp(MyText,ID_zhongweimin,8);//��Ա2����ΰ��
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWM);
		OLED_ShowCHinese(72,0,ZWM+1);
		OLED_ShowCHinese(90,0,ZWM+2);
		return;
	}
	
	state=strncmp(MyText,ID_zhengwujie,8);//��Ա3��֣���
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWJ);
		OLED_ShowCHinese(72,0,ZWJ+1);
		OLED_ShowCHinese(90,0,ZWJ+2);
		return;
	}

	state=strncmp(MyText,ID_liaotingchun,8);//��Ա4��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HSY);
		OLED_ShowCHinese(72,0,HSY+1);
		OLED_ShowCHinese(90,0,HSY+2);
		return;
	}
	
	state=strncmp(MyText,ID_maozimeng,8);//��Ա5��ë����
	if(state==0)
	{
		OLED_ShowCHinese(54,0,MZM);
		OLED_ShowCHinese(72,0,MZM+1);
		OLED_ShowCHinese(90,0,MZM+2);
		return;
	}
	
	state=strncmp(MyText,ID_hulingqiao,8);//��Ա6��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HLQ);
		OLED_ShowCHinese(72,0,HLQ+1);
		OLED_ShowCHinese(90,0,HLQ+2);
		return;
	}
	
	state=strncmp(MyText,ID_wangtianhui,8);//��Ա7��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,WTH);
		OLED_ShowCHinese(72,0,WTH+1);
		OLED_ShowCHinese(90,0,WTH+2);
		return;
	}	
	
	state=strncmp(MyText,ID_zhoujiahui,8);//��Ա8���ܼλ�
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZJH);
		OLED_ShowCHinese(72,0,ZJH+1);
		OLED_ShowCHinese(90,0,ZJH+2);
		return;
	}
	
	state=strncmp(MyText,ID_biance,8);//��Ա9���߲�
	if(state==0)
	{
		OLED_ShowCHinese(54,0,BC);
		OLED_ShowCHinese(72,0,BC+1);
		return;
	}

	
	
	state=strncmp(MyText,"19189198",8);//��Ա10�����Գ�Ա
	if(state==0)
	{
		OLED_ShowString(60,0,"TextName");	//������ʵIC����
		return;
	}
	
	OLED_ShowString(48,0,"NameError!");	//��⵽δ¼��Ŀ��ţ���ʾ����
}

