#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "bike.h"

#if ( defined TM1640 ) || ( defined TM1624 )
#define DisplayInit(x)	TM16XX_Init(x)
#else
#define DisplayInit(x)	BL55072_Config(x)
#endif

void Display(BIKE_STATUS* bike);

#endif