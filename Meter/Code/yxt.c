#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "yxt.h"


unsigned char YXT_Buf[12];
unsigned char YXT_Status[9];
unsigned char YXT_RxBit,YXT_RxData;
unsigned char YXT_Update=0;


/************************************************************************************************************
*    Timer2 Capture interrupt subroutine
************************************************************************************************************/
void Capture_ISR (void) interrupt 12
{
	unsigned char cl,ch;
	static uint16_t high,low;
	
	clr_CAPF0;			// clear capture0 interrupt flag
	cl = C0L;			// For capture mode CxL/CxH with data capture from I/O pin
	ch = C0H;											
	clr_TF2;
	
	if ( P12 == 1 ){
		low = ((uint16_t)ch<<8)|cl;
	} else {
		high = ((uint16_t)ch<<8)|cl;
		YXT_Tim_Receive(low*100/(low+high));
	}
}

void YXT_Init(void)
{
	TIMER2_DIV_32;
	TIMER2_CAP0_Capture_Mode;
	IC0_P12_CAP0_BothEdge_Capture;

	set_ECAP;                                   //Enable Capture interrupt
	set_TR2;                                    //Triger Timer2
	
	YXT_Update = 0;
}


void YXT_Tim_Receive(uint16_t duty)
{
	int8_t PlusCode;
	uint8_t checksum;
	uint8_t i;
  	
	YXT_RxData <<= 1;
	if ( duty > 15 && duty < 42 ) {
		YXT_RxData |= 0x01;
	}
  
	if ( (++YXT_RxBit % 8) == 0 ){
		YXT_Buf[YXT_RxBit/8-1] = YXT_RxData;
		YXT_RxData = 0;
	}
	
	if ( YXT_RxBit >= 96 ){
		YXT_RxBit = 0;
		
		checksum = YXT_Buf[0];
		for(i=1;i<12;i++)
			checksum ^= YXT_Buf[i];
	
		if ( checksum )	return ;
		if ( YXT_Update ) return ;
		
		if ( YXT_Buf[0] == 0x1F ){	//YXT_WXJX
			PlusCode = (char)(YXT_Buf[1]+0x9E);
			PlusCode = (char)(PlusCode ^ 0x47);
			PlusCode = (char)(PlusCode + 0x8D);
			PlusCode = (char)(PlusCode ^ 0x3A);
			PlusCode = (char)(PlusCode + (YXT_Buf[2] >> 4));
			PlusCode = (char)(PlusCode ^ 0xCB);
			PlusCode = (char)(PlusCode + 0x03);
			PlusCode = (char)(PlusCode ^ 0xAC);
			PlusCode = PlusCode & 0x7F;
		} else if ( YXT_Buf[0] == 0x25 ){	//协昌-台州本铃
			PlusCode = (char)(YXT_Buf[1]+0xEA);
			PlusCode = (char)(PlusCode ^ 0xDB);
			PlusCode = (char)(PlusCode + 0xBF);
			PlusCode = (char)(PlusCode ^ 0x5A);
			PlusCode = (char)(PlusCode + (YXT_Buf[2] >> 4));
			PlusCode = (char)(PlusCode ^ 0x4B);
			PlusCode = (char)(PlusCode + 0xDB);
			PlusCode = (char)(PlusCode ^ 0xBD);
			PlusCode = PlusCode & 0x7F;
		} else if ( YXT_Buf[0] == 0x2B ){	//YXT_LQGS
			PlusCode = (char)(YXT_Buf[1]+0x2C);
			PlusCode = (char)(PlusCode ^ 0x6F);
			PlusCode = (char)(PlusCode + 0xDF);
			PlusCode = (char)(PlusCode ^ 0x7A);
			PlusCode = (char)(PlusCode + (YXT_Buf[2] >> 4));
			PlusCode = (char)(PlusCode ^ 0x2B);
			PlusCode = (char)(PlusCode + 0x0D);
			PlusCode = (char)(PlusCode ^ 0xC2);
			PlusCode = PlusCode & 0x7F;
		} else if ( YXT_Buf[0] == 0x08 ){	//英科尔、晶汇本铃
			PlusCode = 0;
		} else {
			PlusCode = YXT_Buf[10];
		}
		
		//YXT_Status[0] = YXT_Buf[2] - (((YXT_Buf[2] >> 4))*0x10);
		YXT_Status[1] = YXT_Buf[3] - PlusCode;
		YXT_Status[2] = YXT_Buf[4] - PlusCode;
		//YXT_Status[3] = YXT_Buf[5] - PlusCode;
		//YXT_Status[4] = YXT_Buf[6];
		YXT_Status[5] = YXT_Buf[7] - PlusCode;
		YXT_Status[6] = YXT_Buf[8] - PlusCode;
		//YXT_Status[7] = YXT_Buf[9] - PlusCode;
		//YXT_Status[8] = YXT_Buf[10]- PlusCode;
		
		YXT_Update = 1;
	}
	if ( duty > 2560 ) {
		YXT_RxBit  = 0;
		YXT_RxData = 0;
	}
}

void YXT_Task(BIKE_STATUS *bike,BIKE_CONFIG* config)
{
	static unsigned int pre_tick=0;
	uint32_t speed;
	
	if ( YXT_Update ){
		pre_tick = Get_SysTick();
		bike->bYXTERR = 0;

#ifdef JIKE13050	
		if ( (YXT_Status[1] & (1<<7)) )	bike->bECO     = 1; else bike->bECO     = 0;
#endif
		if ( (YXT_Status[1] & (1<<6)) )	bike->bHallERR = 1; else bike->bHallERR = 0;
		if ( (YXT_Status[1] & (1<<5)) ) bike->bWheelERR= 1; else bike->bWheelERR= 0; 
		if ( (YXT_Status[1] & (1<<4)) )	bike->bECUERR  = 1; else bike->bECUERR  = 0;
	//	if ( (YXT_Status[1] & (1<<2)) )	bike->bCruise  = 1; else bike->bCruise  = 0;
		if ( (YXT_Status[1] & (1<<0)) )	bike->bPhaseERR= 1; else bike->bPhaseERR= 0;

		if ( (YXT_Status[2] & (1<<5)) )	bike->bBraked  = 1; else bike->bBraked  = 0;
#ifdef JIKE13050	
		if ( (YXT_Status[2] & (1<<3)) )	bike->bRCHG    = 1; else bike->bRCHG    = 0;
		if ( (YXT_Status[2] & (1<<6)) )	bike->bParking = 0; else bike->bParking = 1;
		
#endif
	
		bike->ucSpeedMode = ((YXT_Status[2]>>5)&0x04)|(YXT_Status[2]&0x03);
		speed = ((unsigned int )YXT_Status[5]<<8) | YXT_Status[6];
		speed = speed*5/60;	//600->50Km/h
		bike->ucYXT_Speed 	= speed;
		bike->ucSpeed 		= bike->ucYXT_Speed*1000UL/config->uiYXT_SpeedScale;

		YXT_Update = 0;  
	} else if ( Get_ElapseTick(pre_tick) > 3000 ){
		bike->bYXTERR 	= 1;
		bike->bHallERR 	= 0;
		bike->bPhaseERR	= 0;
		bike->bWheelERR = 0;
		bike->bECUERR 	= 0;
	}	
    
	if ( bike->bYXTERR 	== 0 ){
		bike->ucSpeed 	= bike->ucYXT_Speed*1000UL/config->uiYXT_SpeedScale;
    }
}

