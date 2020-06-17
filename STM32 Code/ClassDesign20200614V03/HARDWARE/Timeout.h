

#ifndef __Timeout_H__
#define __Timeout_H__


#include "sys.h"

extern u8 AckTimeoutEnable;
extern u8 AckTimeoutCount;
extern u8 RequestTimeoutEnable;
extern u8 RequestTimeoutCount;

void Timeout_Init(void);
void UsartTimeout_Init(void);
#endif


