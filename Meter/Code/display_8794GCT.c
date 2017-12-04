#include "stdlib.h"
#include "display.h"
#include "bl55072.h"
#include "stm8s.h"

unsigned char BL_Data[19];
unsigned char flashflag = 0;

const unsigned char SegDataTime[10] 	= {0xFA,0x60,0xD6,0xF4,0x6C,0xBC,0xBE,0xE0,0xFE,0xFC};
const unsigned char SegDataVoltage[10]	= {0x5F,0x50,0x3D,0x79,0x72,0x6B,0x6F,0x51,0x7F,0x7B};
const unsigned char SegDataMile[10] 	= {0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF,0xEB};
#define SegDataMile2 	SegDataVoltage
#define SegDataSpeed 	SegDataVoltage
#define SegDataTemp 	SegDataTime
#define SegDataEnergy 	SegDataVoltage

void MenuUpdate(BIKE_STATUS* bike)
{
	unsigned char i = 0;

	flashflag++;
	flashflag %= 10;

	for(i=0;i<18;i++) BL_Data[i] = 0x00;

#if ( PCB_VER != 201745UL	)
	if ( bike->bLFlashType ){
		if ( bike->bLeftFlash ) BL_Data[0x08] |= 0x01;	//S1
    } else {
     	if ( bike->bLeftFlash && flashflag >= 5 ) BL_Data[0x08] |= 0x01;	//S1
    }
    if ( bike->bRFlashType ){
		if ( bike->bRightFlash ) BL_Data[0x0F] |= 0x04;	//S12
    } else {
     	if ( bike->bRightFlash && flashflag >= 5 ) BL_Data[0x0F] |= 0x04;	//S12
    }
#endif
	
	if( bike->bCruise 		) BL_Data[0x0B] |= 0x20;	//S4
	if( bike->bNearLight	) BL_Data[0x0B] |= 0x10;	//S5
	
#if ( PCB_VER != 201745UL	)
	#if ( ( TurnLeftOut_PIN == GPIO_PIN_1 ) )
	CFG->GCR = CFG_GCR_SWD;
	#endif
	GPIO_Init(NearLightOut_PORT, NearLightOut_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
	if( bike->bNearLight ) GPIO_WriteLow (NearLightOut_PORT,NearLightOut_PIN); else GPIO_WriteHigh (NearLightOut_PORT,NearLightOut_PIN);
#endif
	
	if( bike->bHallERR 	) BL_Data[0x0B] |= 0x80;	//S2	电机霍尔故障
	if( bike->bWheelERR ) BL_Data[0x0B] |= 0x40;	//S3	手把故障
	if( bike->bECUERR 	) BL_Data[0x0F] |= 0x01;	//S10 	电机控制器故障
//	if( bike->bPhaseERR ) BL_Data[0x0F] |= 0x02;	//S11	电机缺相故障
	if( bike->bBraked  	) BL_Data[0x0F] |= 0x02;	//S11	刹车
	if( bike->bECO		) BL_Data[0x0E] |= 0x80;	//S6	ECO
	if( bike->bRCHG		) BL_Data[0x0F] |= 0x80;	//S7	R
	if( bike->bParking	) {
		GPIO_Init(POut_PORT, POut_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
		GPIO_WriteLow (POut_PORT,POut_PIN);
	} else {
		GPIO_Init(POut_PORT, POut_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
		GPIO_WriteHigh (POut_PORT,POut_PIN);
	}

	/***************************Battery Area Display**********************************/
	BL_Data[0x06] |=  0x80; //T
	switch ( bike->ucBatStatus ){
	case 0:
		if ( flashflag < 5 ) 
			BL_Data[0x06]&=~0x80;break;
	case 1: BL_Data[0x07] = 0x80;break;
	case 2: BL_Data[0x07] = 0xC0;break;
	case 3: BL_Data[0x07] = 0xE0;break;
	case 4: BL_Data[0x07] = 0xF0;break;
	case 5: BL_Data[0x07] = 0xF1;break;
	case 6: BL_Data[0x07] = 0xF3;break;      
	case 7: BL_Data[0x07] = 0xF7;break;
	case 8: BL_Data[0x07] = 0xFF;break;          
	default:break; 
	}

	/***************************Temp Area Display**********************************/
	BL_Data[0x10] |= (SegDataTemp[abs(bike->siTemperature/10)%10]);	//D2
	BL_Data[0x11] |= (SegDataTemp[abs(bike->siTemperature/10)/10]);	//D1
	BL_Data[0x10] |= 0x01;		//S9
	if (bike->siTemperature < 0)
	BL_Data[0x11] |= 0x10;	//S8       

	/***************************Time Area Display**********************************/
#if ( TIME_ENABLE == 1 )
	if ( bike->bHasTimer ){
		if(bike->ucHour > 9) BL_Data[0x0A] |= 0x01;	//S13  
		BL_Data[0x08] |= ( SegDataTime[bike->ucHour%10]);		//D5
		BL_Data[0x09] |= ( SegDataTime[bike->ucMinute/10] );	//D4
		BL_Data[0x0A] |= ( SegDataTime[bike->ucMinute%10] );	//D3    
		if ( bike->bTimeSet ){
			switch ( bike->ucTimePos ){
			case 0:
			if ( flashflag < 5  ) { 
				BL_Data[0x08] &= 0x01; 
				BL_Data[0x09] &= 0x01; 
				BL_Data[0x0A] &= 0x01; 
			}
			break;			
			case 1:if ( flashflag < 5  ) BL_Data[0x08] &= 0x01; break;
			case 2:if ( flashflag < 5  ) BL_Data[0x09] &= 0x01; break;
			case 3:if ( flashflag < 5  ) BL_Data[0x0A] &= 0x01; break;
			default:break;		
		}
		BL_Data[0x09] |= 0x01;	//col
	} else if ( flashflag < 5 ) BL_Data[9] |= 0x01;	//col
	}
#endif

	/*************************** uiVoltage Display**********************************/
	BL_Data[0x05] |= (SegDataVoltage[ bike->uiVoltage/10  %10]) | 0x80; //V
	BL_Data[0x06] |= (SegDataVoltage[(bike->uiVoltage/100)%10]);

#if 0	
	/*************************** uiVoltage Energy**********************************/
	BL_Data[0x0D] |= 0x80; //S15
	BL_Data[0x0C] |= (SegDataEnergy[ bike->ucEnergy%10]	 )&0x0F;
	BL_Data[0x0D] |= (SegDataEnergy[ bike->ucEnergy%10]	 )&0xF0;
	BL_Data[0x0B] |= (SegDataEnergy[(bike->ucEnergy/10)%10])&0x0F; 
	BL_Data[0x0C] |= (SegDataEnergy[(bike->ucEnergy/10)%10])&0xF0; 
	if ( bike->ucEnergy == 100 ) BL_Data[0x0C] |= 0x80; //S14
#endif

	/*************************** ulMile Display**********************************/  
	BL_Data[0x00] |= (SegDataMile2[ bike->ulMile		 %10]) | 0x80;	//S17
	BL_Data[0x01] |= (SegDataMile [(bike->ulMile/10   )%10]);
	BL_Data[0x02] |= (SegDataMile [(bike->ulMile/100  )%10]); 
	BL_Data[0x03] |= (SegDataMile [(bike->ulMile/1000 )%10]);
	BL_Data[0x04] |= (SegDataMile [(bike->ulMile/10000)%10]); 
	if ( bike->bMileFlash ){
		if ( flashflag < 5  ) {
			BL_Data[0x00] = 0x80;	//S17
			BL_Data[0x01] = 0;
			BL_Data[0x02] = 0; 
			BL_Data[0x03] = 0;
			BL_Data[0x04] = 0; 
		}
	}
	
	/*************************** Speed Display**********************************/
	BL_Data[0x0F] |= 0x08;	//S16
	BL_Data[0x0E] |= (SegDataSpeed[ bike->ucSpeed%10]	   )&0x0F;
	BL_Data[0x0F] |= (SegDataSpeed[ bike->ucSpeed%10]	   )&0xF0;
	BL_Data[0x0D] |= (SegDataSpeed[(bike->ucSpeed/10)%10])&0x0F; 
	BL_Data[0x0E] |= (SegDataSpeed[(bike->ucSpeed/10)%10])&0xF0; 
	if ( bike->bSpeedFlash ){
		if ( flashflag < 5  ) {
			BL_Data[0x0E]  = 0;
			BL_Data[0x0F] &= 0x0F;
			BL_Data[0x0D] &= 0xF0; 
			BL_Data[0x0F] |= 0x08;	//S16
		}
	}

	/*************************** Mode Display**********************************/ 
	switch (bike->ucSpeedMode){
	case 1: BL_Data[0x01] |= 0x10;break;	//1
	case 2: BL_Data[0x02] |= 0x10;break;	//2
	case 3: BL_Data[0x03] |= 0x10;break;	//3
	case 4: BL_Data[0x04] |= 0x10;break;	//4
	default:
			BL_Data[0x01] &= ~0x10;
			BL_Data[0x02] &= ~0x10;
			BL_Data[0x03] &= ~0x10;
			BL_Data[0x04] &= ~0x10;
	break;
	}

	BL_Write_Data(0,18,BL_Data);
}


void Delay(uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

