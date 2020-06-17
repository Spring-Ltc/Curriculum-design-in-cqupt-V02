
#ifndef __LEDandBuzz_H__
#define __LEDandBuzz_H__


#include "sys.h"

void RGBledInit(void);
void BuzzInit(void);
void LedTurnON(u8 color);
void LedTurnOFF(u8 color);
void LED_Flashing(u8 color,u8 times);
void BUZZ_Flashing(u8 times);



#define red 1
#define green 2
#define blue 3

#define LedRed				 PBout(13)
#define LedGreen			 PBout(12)
#define LedBlue 	   	 PAout(1)
#define BUZZ 					 PAout(0)


#endif


