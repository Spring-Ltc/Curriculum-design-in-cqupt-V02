



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
2020/0607/22:17
	发现问题：RFIC检测到的卡号是16进制输出到串口，OLED只能显示字符和汉字，需要完成转换后再显示

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
u16 Humidity ;       //湿度
u16 Temperature;     //温度
u16 Device_ID;
} hdc1080_measurement_t;
hdc1080_measurement_t HDC;


int main(void)
{
	unsigned char status,i;
	u16 MyText;
	NVIC_Configuration();    
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();
	uart_init(115200);//串口1，USB连接
	
	//各模块的初始化
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
		

		
  

		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
		 if (status != MI_OK)
		 {    
				 PcdReset();
				 PcdAntennaOff(); 
				 PcdAntennaOn(); 
				 continue;
		 }		     
		 printf("卡的类型:");
	  for(i=0;i<2;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
		}	
		printf("\t\t");
		//【寻卡并打印输出卡的类型完毕】
		
		
		
		 status = PcdAnticoll(g_ucTempbuf);//防冲撞
		 if(status != MI_OK)
		 {    
				continue;   
		 }
		 //【防碰撞完毕】  
		 
		 
		////////以下为超级终端打印出的内容////////////////////////
	
		printf("卡序列号：");	//超级终端显示,
		for(i=0;i<4;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
		}
		printf("\r\n");
		//【打印输出卡的序列号完毕】	
		//OLED_ShowString(0,48,g_ucTempbuf);
		OLED_DisplayInit();
		BUZZ=0;delay_ms(100);BUZZ=1;delay_ms(1500);delay_ms(1500);//两次刷卡时间间隔
  }
}



//OLED显示初始化
void OLED_DisplayInit(void)
{
		OLED_ShowCHinese(0,0,26);//姓
		OLED_ShowCHinese(18,0,27);//名
		OLED_ShowString(36,0,": ");
	
		OLED_ShowCHinese(0,2,30);//设
		OLED_ShowCHinese(16,2,31);//备
		OLED_ShowString(32,2,"ID: ");
	OLED_ShowString(0,4,g_ucTempbuf);
}

