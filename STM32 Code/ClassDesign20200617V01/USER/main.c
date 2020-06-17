



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
2020/06/14/11:04
	�����֮ǰʵϰд��ͨ�Ŵ�����Զ��崮�ڷ��͵���غ�������֣�������ȷ����ͨ�ŵ����ݸ�ʽ��Ӧ�����
	�½���Timeout�ļ���׼������ʱ������ʱ���ա�
2020/06/14/11:28
	����˶�ʱ���ĳ�ʼ�����жϣ�������������ʱ��ʹ�ܺͼ�������
2020/06/14/14:58
	����1����Ϊ��ʱ���գ����յ���ͷ��ſ�ʼ���գ���������ʱ����TIM2������ʱ���Ｔ�������
	ͨ�����ڴ�ӡ�����֤û�����⡣
2020/06/14/16:44
	����ͽ��ա��ϴ����ִ��붼д���ˣ��������߼�Ҳд���ˣ�Ȼ�������ˣ��������������ˣ�����
	�������⣺�������ζ���֮���ʱ����̫С�ˣ����ԼӸ�1s��ʱ���Զ���
		�����δ��ںͶ�ʱ��������£�������С�Ӹ���ʱ2ms���ܶ�����
2020/06/16/20:16
	������OLED��ʾ��ˢ�µķ�ʽ
	���幦�ܻ�����ɣ�ͨ�����ڵ���ͨ��������������Ӧ���У��
2020/06/17/14:35
	�����������ݮ�ɵĵ��ԣ������¶��ϴ��Ĺ�����֮ǰ�����ˣ��¶�ֵ�ı����ʽ�����ˣ�
	�Ѿ��޸ģ�ͨ�����ڲ�����ϣ������ϻ����Һʹ��һ�����ϵͳ������

*****************************************************************************************************/	





#include "stm32f10x.h"

#include "Timeout.h"
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
u8 RFID_Updata(void);
void OLED_DisplayUpdata(u8 Name,char *IDcard,char * StudentID);




#define TimeoutTemp 5		//����ʱ�䣬��β��·��㿴�仯
#define TemperatureLimit	3050		//���±�����ֵ

u16 Temperature;     //�¶�
char IDcard[10]="6666666666";		//RFID���ţ�ʵ��ֻ�У�λ
u8 TempMeas=TimeoutTemp*2;	//��������ʶ��ĳ�ʱʱ��ļ�������
u8 ChineseName;//��������Ϣ���


int main(void)
{
	unsigned char status;
	u8 Exist=1;
	u8 FaceRecognitionResult = 1;
	

	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	MyusartInit(115200);
	
	//��ģ��ĳ�ʼ��
	RGBledInit();//LED��
	BuzzInit();//������
	RC522_Init();//RFID��Ƶ��
	OLED_Init();	//OLEDҺ����ʾ
	MAX7219_Init();//�������ʾ
	MotorInit();//��������Ž���
	HDC1080_Init();//�����ʪ��
	Timeout_Init();//��ʱ����ʼ��
	UsartTimeout_Init();//���ڳ�ʱ����
	
	
	
	
	OLED_DisplayInit();//
	Digitaltube_Close();
	

  while (1)
  {
	
		OLED_Clear();
		Digitaltube_Close();
		ClearArray(StuNumber,10);
		ChineseName=32;

		delay_ms(2);//�Ӹ���ʱ����Ȼһֱ�����������ö������ɹ�
		status = RFID_Search(IDcard);//һֱ����Ƿ��п�
	
		if(status != 0)
			continue;
		
		
		//��⵽����ˢ�������±�����ʾ����ʾ����ƥ�����Ϳ���ID��
		BUZZ=0;delay_ms(100);BUZZ=1;//ˢ����ʾ��
		Exist = RFID_Updata();
		OLED_DisplayUpdata(ChineseName,IDcard,StuNumber);//������ʾRFID����ƥ����Ϣ
		if(Exist == 1)//δ¼��ÿ�����Ϣ
		{
			LED_Flashing(red,10);
			continue;
		}
		
		
		//RFID����ƥ��ɹ�����������ʶ��������������
		Digitaltube_Wait();//�������ʾ��ܣ���ʾ���ڵȴ�����ʶ��Ľ��
		FaceRecognitionResult = 1;
		FaceRecognitionResult = SendFaceRecognitionRequest(USART1,3);//����0��ʾ�յ�����ʶ����,3����������
		if(FaceRecognitionResult)
			BUZZ_Flashing(3);//�����������α�ʾ����ʶ��ʧ��
//����������ʶ��ɹ�����ʧ�ܣ������в��ºͿ��Ų����������ִ�У�����Ӹ�continue��


		
		TempMeas=TimeoutTemp*2;//װ�ظ��µĲ���ʱ�����
		while(--TempMeas)//��β���
		{
			Temperature=HDC1080_Read_Temper();
			Temperature_DisplayUpdata(Temperature);//ʵʱ��ʾ�������¶�
			LedBlue=!LedBlue;//���ƽ�����˸����ʾ���ڲ���
			delay_ms(200);
		}
		LedTurnOFF(blue);//�رղ���״ָ̬ʾ��
		

		
		
		//�ж�����ʶ��Ľ��
		if(FaceRecognitionResult == 0)//����ʶ��ɹ����ϴ�������Ϣ�����ұ���������ʾ��ʾ
		{
			BUZZ=0;delay_ms(300);BUZZ=1;//����ʶ��ɹ���ʾ��
			UpdataTemperature(USART1,Temperature,3);//�������ϴ�������Ϣ��
			OLED_DisplayUpdata(ChineseName,IDcard,StuNumber);
			delay_ms(1000);delay_ms(1000);delay_ms(1000);
		}
		

		//˳���ж��¶��Ƿ�ȫ
		if(Temperature>TemperatureLimit)//���³����趨��ֵ��������Ӧ������ʾ
			BUZZ_Flashing(10);//��������5������������ʾ
		else
		{
			Motor_open();//����
			LED_Flashing(green,5);//�̵���˸,���Ѿ���ͨ��
			LedTurnON(green);//�̵��ڼ�ͨ��
			delay_ms(1000);delay_ms(1000);delay_ms(1000);//����3sʱ��Ͽ�ͨ��
			LedTurnOFF(green);
			LED_Flashing(red,5);//�����˸,�����ż����ر�
			Motor_close();//����
		}
  }
}



//OLED��ʾ��ʼ��
void OLED_DisplayInit(void)
{
		
		OLED_Clear();
	
		OLED_ShowCHinese(0,0,26);//��
		OLED_ShowCHinese(18,0,27);//��
		OLED_ShowString(36,0,": ");
	
		OLED_ShowCHinese(0,2,30);//��
		OLED_ShowCHinese(16,2,31);//��
		OLED_ShowString(32,2,"ID: ");

		OLED_ShowCHinese(0,5,28);//ѧ
		OLED_ShowCHinese(16,5,29);//��
		OLED_ShowString(32,5,": ");

}


//����RFID��Ϣ��ʾ��������ƥ����
//����ֵ0��ƥ��ɹ�������ֵ1��ƥ��ʧ��
u8 RFID_Updata(void)
{
	u8 state=1;

	//���濪ʼ���Ѿ�¼�����Ϣ��һ�ȶ�
	state=strncmp(IDcard,ID_liaotingchun,8);//��Ա1����͢��
	if(state==0)
	{
		ChineseName = LTC;
		return 0;
	}

	state=strncmp(IDcard,ID_zhongweimin,8);//��Ա2����ΰ��
	if(state==0)
	{
		ChineseName = ZWM;
		return 0;
	}
	
	state=strncmp(IDcard,ID_zhengwujie,8);//��Ա3��֣���
	if(state==0)
	{
		ChineseName = ZWJ;
		return 0;
	}

	state=strncmp(IDcard,ID_liaotingchun,8);//��Ա4��������
	if(state==0)
	{
		ChineseName = HSY;
		return 0;
	}
	
	state=strncmp(IDcard,ID_maozimeng,8);//��Ա5��ë����
	if(state==0)
	{
		ChineseName = MZM;
		return 0;
	}
	
	
	
	state=strncmp(IDcard,ID_hulingqiao,8);//��Ա6��������
	if(state==0)
	{
		ChineseName = HLQ;
		return 0;
	}
	
	state=strncmp(IDcard,ID_wangtianhui,8);//��Ա7��������
	if(state==0)
	{
		ChineseName = WTH;
		return 0;
	}	
	
	state=strncmp(IDcard,ID_zhoujiahui,8);//��Ա8���ܼλ�
	if(state==0)
	{
		ChineseName = ZJH;
		return 0;
	}
	
	state=strncmp(IDcard,ID_biance,8);//��Ա9���߲�
	if(state==0)
	{
		ChineseName = BC;
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
//Function OK



//����OLED��ʾ��Ϣ
void OLED_DisplayUpdata(u8 Name,char *IDcard,char * StudentID)
{
	OLED_DisplayInit();
	
	//��ʾ��������
//	if(Name<25)
//	{
		OLED_ShowCHinese(54,0,Name);
		OLED_ShowCHinese(72,0,Name+1);
		if(ChineseName!=BC)
			OLED_ShowCHinese(90,0,Name+2);
//		}
		
	OLED_ShowString(60,2,IDcard);	//������ʾIC����
	
	OLED_ShowString(42,5,StudentID);	//������ʾѧ��
		
}