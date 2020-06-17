



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
2020/06/13/16:06
	ɾ��֮ǰ��ֲ�����Ķ����û�õĽṹ�壬ȷ�������������������мܹ����½��˴���ͨ�ŵĺ����ļ�Myusart



	
*****************************************************************************************************/	

#include "stm32f10x.h"


#include "Myusart.h"
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
u8 RFID_DisplayUpdata(void);


#define Timeout 5		//����ʱʱ�䣬��λs
#define TemperatureLimit	2550		//���±�����ֵ

u16 Temperature;     //�¶�
char IDcard[10];		//RFID���ţ�ʵ��ֻ�У�λ
u8 RequestTime=Timeout*2;	//��������ʶ��ĳ�ʱʱ��ļ�������


int main(void)
{
	unsigned char status,i,j;
		u8 Exist=1;
//	u16 Temp = 0;//���ζ�ȡ���¶�ֵ

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



  while (1)
  {
	
		OLED_Clear();
		Digitaltube_Close();
		
		status = RFID_Search(IDcard);//һֱ����Ƿ��п�
		if(status != 0)
			continue;
		
		//��⵽����ˢ�������±�����ʾ����ʾ����ƥ�����Ϳ���ID��
		BUZZ=0;delay_ms(100);BUZZ=1;//ˢ����ʾ��
		Exist = RFID_DisplayUpdata();//������ʾRFID����ƥ����Ϣ
		if(Exist == 1)//δ¼��ÿ�����Ϣ
		{
			LED_Flashing(red,10);
			continue;
		}
		
		//RFID����ƥ��ɹ�����������ʶ��������������
		//=======SendAquare();�����
		Digitaltube_Wait();//�������ʾ��ܣ���ʾ���ڲ���
		RequestTime=Timeout*2;//װ�ظ��µĳ�ʱʱ�����
		while(--RequestTime)//��һ��ʱ�����ڵȴ�����ʶ��Ľ�����ڽ����ж������жϺ󽫸ñ�־��0
		{
			Temperature=HDC1080_Read_Temper();
			Temperature_DisplayUpdata(Temperature);//ʵʱ��ʾ�������¶�
			LedBlue=!LedBlue;//���ƽ�����˸����ʾ���ڵȴ�����ʶ��Ľ��
			delay_ms(500);
		}
		LedTurnOFF(blue);//�ر�����ʶ������״ָ̬ʾ��
		
		
		//�ж�����ʶ��Ľ��
		if(RequestTime==0);//��ʱ
		{
			RequestTime=Timeout*2;//װ����һ�γ�ʱʱ�����
			
			
			//continue;
		}
		
		
		//����ʶ��ɹ����ϴ�������Ϣ�����ұ���������ʾ��ʾ
		BUZZ=0;delay_ms(300);BUZZ=1;//����ʶ��ɹ���ʾ��
		//�������ϴ�������Ϣ��
		if(Temperature>TemperatureLimit)//���³����趨��ֵ��������Ӧ������ʾ
			BUZZ_Flashing(10);//��������5������������ʾ
		else
		{
			Motor_open();//����
			LED_Flashing(green,5);//�̵���˸,���Ѿ���ͨ��
			LedTurnON(green);//�̵��ڼ�ͨ��
			LED_Flashing(red,5);//�����˸,�����ż����ر�
			Motor_close();//����
		}
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


//����RFID��Ϣ��ʾ��������ƥ����
//����ֵ0��ƥ��ɹ�������ֵ1��ƥ��ʧ��
u8 RFID_DisplayUpdata(void)
{
	u8 state=1;
	OLED_DisplayInit();
	OLED_ShowString(60,2,IDcard);	//������ʵIC����

	//���濪ʼ���Ѿ�¼�����Ϣ��һ�ȶ�
	state=strncmp(IDcard,ID_liaotingchun,8);//��Ա1����͢��
	if(state==0)
	{
		OLED_ShowCHinese(54,0,LTC);
		OLED_ShowCHinese(72,0,LTC+1);
		OLED_ShowCHinese(90,0,LTC+2);
		return 0;
	}

	state=strncmp(IDcard,ID_zhongweimin,8);//��Ա2����ΰ��
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWM);
		OLED_ShowCHinese(72,0,ZWM+1);
		OLED_ShowCHinese(90,0,ZWM+2);
		return 0;
	}
	
	state=strncmp(IDcard,ID_zhengwujie,8);//��Ա3��֣���
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWJ);
		OLED_ShowCHinese(72,0,ZWJ+1);
		OLED_ShowCHinese(90,0,ZWJ+2);
		return 0;
	}

	state=strncmp(IDcard,ID_liaotingchun,8);//��Ա4��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HSY);
		OLED_ShowCHinese(72,0,HSY+1);
		OLED_ShowCHinese(90,0,HSY+2);
		return 0;
	}
	
	state=strncmp(IDcard,ID_maozimeng,8);//��Ա5��ë����
	if(state==0)
	{
		OLED_ShowCHinese(54,0,MZM);
		OLED_ShowCHinese(72,0,MZM+1);
		OLED_ShowCHinese(90,0,MZM+2);
		return 0;
	}
	
	state=strncmp(IDcard,ID_hulingqiao,8);//��Ա6��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HLQ);
		OLED_ShowCHinese(72,0,HLQ+1);
		OLED_ShowCHinese(90,0,HLQ+2);
		return 0;
	}
	
	state=strncmp(IDcard,ID_wangtianhui,8);//��Ա7��������
	if(state==0)
	{
		OLED_ShowCHinese(54,0,WTH);
		OLED_ShowCHinese(72,0,WTH+1);
		OLED_ShowCHinese(90,0,WTH+2);
		return 0;
	}	
	
	state=strncmp(IDcard,ID_zhoujiahui,8);//��Ա8���ܼλ�
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZJH);
		OLED_ShowCHinese(72,0,ZJH+1);
		OLED_ShowCHinese(90,0,ZJH+2);
		return 0;
	}
	
	state=strncmp(IDcard,ID_biance,8);//��Ա9���߲�
	if(state==0)
	{
		OLED_ShowCHinese(54,0,BC);
		OLED_ShowCHinese(72,0,BC+1);
		return 0;
	}

	
	
	state=strncmp(IDcard,"19189198",8);//��Ա10�����Գ�Ա
	if(state==0)
	{
		OLED_ShowString(60,0,"TextName");	//������ʵIC����
		return 0;
	}
	
	OLED_ShowString(48,0,"NameError!");	//��⵽δ¼��Ŀ��ţ���ʾ����
	return 1;
}

