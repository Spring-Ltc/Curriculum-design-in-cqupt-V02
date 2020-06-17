



/********************重庆邮电大学2017级08051704班第6学期课程设计底层代码（第六组）************************************

MCU型号：STM32F103C8T6
作者：Liao_Tingchun；邮箱：2093181896@qq.com
修改日志：
2020/06/02/
	RC522程序移植完成，已经接上模块测试，能正常使用，但未进行读写操作
2020/06/03/17:33	
	OLED程序移植完成，无error和warning，但还未接上模块测试
2020/06/07/09:55
	PCB焊接测试：RGB灯测试完全OK；蜂鸣器驱动电路更换分压电阻后测试完全OK；
	移植的测试程序，RFID和OLED均正常运行，可确认PCB无问题，待完善代码
2020/06/07/10:10
	发现问题：OLED和RC522不能同时初始化（20200607/10：23解决问题，OLED中重新初始化了PB8管脚）
2020/06/07/11:36
	MAX7219驱动数码管测试完全OK，之前电路设计有问题，MAX7219供电电压范围（4-5.5V）,逻辑高电平大于3.3V
	最后使用5V电源串联一
	个150Ω的电阻降低供电电压，从而降低逻辑高电平电压。
	本次程序有之前51的程序移植过来，由于STM32主频比51快很多倍，芯片有建立时间和保持时间，再串行传输时要加延时
2020/06/07/14:59
	L298N驱动步进电机部分移植完成，功能函数测试完全OK；注意初始化的时候IO电平拉高，不然驱动一直打开导致电源降低
	调节电机转速的延时变量speed最小设置为2
2020/06/07/20:25
	HDC1080温湿度传感器移植完成，温湿度读取测试完全OK
2020/06/07/22:17
	发现问题：RFIC检测到的卡号是16进制输出到串口，OLED只能显示字符和汉字，需要完成转换后再显示
2020/06/10/22:24
	完成IC卡号的字符转换在OLED显示，添加判断了部分成员的卡号，测试OK
2020/06/11/10:00
	修改完善了RFID更新显示和匹配的方式，改用宏定义和全局变量的方式，方便后面添加新成员
2020/06/11/10:24
	添加数码管显示温度，带小数点，显示精度0.01。
	发现新问题，程序运行一段时间后，数码管显示全部点亮，但是串口打印输出的温湿度都正常，RFID也能正常读取，初步猜想是硬件上的问题
2020/06/11/11:44
	将RFID相关功能写成了一个功能函数，简化主函数
	
	
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


///////////////////////////已录入成员的射频卡ID号和中文姓名编码/////////////////////////
char ID_liaotingchun[8]="0F23660A";
#define LTC 0
char ID_zhongweimin[8]="00000000";//暂未录入
#define ZWM 3
char ID_zhengwujie[8]="031202D3";
#define ZWJ 6 
char ID_haoshuyi[8]="00000000";//暂未录入
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








//自定义函数声明
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
u16 Humidity ;       //湿度
u16 Temperature;     //温度
u16 Device_ID;
} hdc1080_measurement_t;
hdc1080_measurement_t HDC;

	char MyText[10];
int main(void)
{
	unsigned char status,i,j;

	NVIC_Configuration();    
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();
	uart_init(115200);//串口1，USB连接
	
	//各模块的初始化
	RGBledInit();//LED灯
	BuzzInit();//蜂鸣器
	RC522_Init();//RFID射频卡
	OLED_Init();	//OLED液晶显示
	MAX7219_Init();//数码管显示
	MotorInit();//电机控制门禁打开
	HDC1080_Init();//检测温湿度

	
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
		delay_ms(1500);delay_ms(1500);//两次刷卡时间间隔
		Digitaltube_Close();
  }
}



//OLED显示初始化
void OLED_DisplayInit(void)
{
		
		OLED_Clear();
		OLED_ShowCHinese(0,2,30);//设
		OLED_ShowCHinese(16,2,31);//备
		OLED_ShowString(32,2,"ID: ");

		OLED_ShowCHinese(0,0,26);//姓
		OLED_ShowCHinese(18,0,27);//名
		OLED_ShowString(36,0,": ");

}



void RFID_DisplayUpdata(void)
{
	u8 state=1;
	OLED_DisplayInit();
	OLED_ShowString(60,2,MyText);	//更新现实IC卡号

	//下面开始和已经录入的信息逐一比对
	state=strncmp(MyText,ID_liaotingchun,8);//成员1：廖廷春
	if(state==0)
	{
		OLED_ShowCHinese(54,0,LTC);
		OLED_ShowCHinese(72,0,LTC+1);
		OLED_ShowCHinese(90,0,LTC+2);
		return;
	}

	state=strncmp(MyText,ID_zhongweimin,8);//成员2：钟伟民
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWM);
		OLED_ShowCHinese(72,0,ZWM+1);
		OLED_ShowCHinese(90,0,ZWM+2);
		return;
	}
	
	state=strncmp(MyText,ID_zhengwujie,8);//成员3：郑武杰
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZWJ);
		OLED_ShowCHinese(72,0,ZWJ+1);
		OLED_ShowCHinese(90,0,ZWJ+2);
		return;
	}

	state=strncmp(MyText,ID_liaotingchun,8);//成员4：郝书逸
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HSY);
		OLED_ShowCHinese(72,0,HSY+1);
		OLED_ShowCHinese(90,0,HSY+2);
		return;
	}
	
	state=strncmp(MyText,ID_maozimeng,8);//成员5：毛梓蒙
	if(state==0)
	{
		OLED_ShowCHinese(54,0,MZM);
		OLED_ShowCHinese(72,0,MZM+1);
		OLED_ShowCHinese(90,0,MZM+2);
		return;
	}
	
	state=strncmp(MyText,ID_hulingqiao,8);//成员6：胡凌桥
	if(state==0)
	{
		OLED_ShowCHinese(54,0,HLQ);
		OLED_ShowCHinese(72,0,HLQ+1);
		OLED_ShowCHinese(90,0,HLQ+2);
		return;
	}
	
	state=strncmp(MyText,ID_wangtianhui,8);//成员7：王天晖
	if(state==0)
	{
		OLED_ShowCHinese(54,0,WTH);
		OLED_ShowCHinese(72,0,WTH+1);
		OLED_ShowCHinese(90,0,WTH+2);
		return;
	}	
	
	state=strncmp(MyText,ID_zhoujiahui,8);//成员8：周嘉辉
	if(state==0)
	{
		OLED_ShowCHinese(54,0,ZJH);
		OLED_ShowCHinese(72,0,ZJH+1);
		OLED_ShowCHinese(90,0,ZJH+2);
		return;
	}
	
	state=strncmp(MyText,ID_biance,8);//成员9：边策
	if(state==0)
	{
		OLED_ShowCHinese(54,0,BC);
		OLED_ShowCHinese(72,0,BC+1);
		return;
	}

	
	
	state=strncmp(MyText,"19189198",8);//成员10：测试成员
	if(state==0)
	{
		OLED_ShowString(60,0,"TextName");	//更新现实IC卡号
		return;
	}
	
	OLED_ShowString(48,0,"NameError!");	//检测到未录入的卡号，提示错误
}

