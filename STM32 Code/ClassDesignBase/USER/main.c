



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
	发现问题：OLED和RC522不能同时初始化




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
u16 Humidity ;       //湿度
u16 Temperature;     //温度
u16 Device_ID;
} hdc1080_measurement_t;
hdc1080_measurement_t HDC;


int main(void)
{
	unsigned char status,i;
	//unsigned int temp;
	
	
	NVIC_Configuration();    
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();
	uart_init(115200);//串口1，USB连接
	
	RGBledInit();
	BuzzInit();

	
	
	
	
	RC522_Init();
	
		OLED_Init();			//初始化OLED  
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
//		OLED_ShowCHinese(0,0,0);//中
//		OLED_ShowCHinese(18,0,1);//景
//		OLED_ShowCHinese(36,0,2);//园
//		OLED_ShowCHinese(54,0,3);//电
//		OLED_ShowCHinese(72,0,4);//子
//		OLED_ShowCHinese(90,0,5);//科
//		OLED_ShowCHinese(108,0,6);//技
//		OLED_ShowString(0,3,"1.3' OLED TEST");		
		
//		 HDC.Device_ID=HDC1080_Read_DeviceID();
//		 HDC.Temperature=HDC1080_Read_Temper();
//		 HDC.Humidity=HDC1080_Read_Humidi();
//		 printf("Temperature=%d  Humidity=%d \r\n",HDC.Temperature,HDC.Humidity);
//		 printf("HDC1080Device_ID=%0xH\r\n", HDC.Device_ID);
//		 
		
		
  

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
		//【打印输出卡的序列号完毕】	
		

  }
}




