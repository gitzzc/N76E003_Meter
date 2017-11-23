#include "stdlib.h"
#include "bike.h"
#include "bl55072.h"

unsigned char BL_Data[19];
unsigned char flashflag = 0;

const unsigned char SegDataTime[10]     = {0xF5,0x60,0xD3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};
const unsigned char SegDataVoltage[10]  = {0xF5,0x05,0xD3,0x97,0x27,0xB6,0xF6,0x15,0xF7,0xB7};
const unsigned char SegDataMile[10] 	= {0xFA,0x0A,0xD6,0x9E,0x2E,0xBC,0xFC,0x1A,0xFE,0xBE};
const unsigned char SegDataMile2[10]    = {0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF,0xEB};
const unsigned char SegDataSpeed[10]    = {0x5F,0x06,0x6B,0x2F,0x36,0x3D,0x7D,0x07,0x7F,0x3F};
const unsigned char SegDataTemp[10] 	= {0xFA,0x0A,0xBC,0x9E,0x4E,0xD6,0xF6,0x8A,0xFE,0xDE};

void MenuUpdate(BIKE_STATUS* bike)
{
	unsigned char i = 0;
  
	flashflag++;
	flashflag %= 10;
	
	for(i=0;i<18;i++)	BL_Data[i] = 0x00;
	
    if ( bike->bLFlashType ){
		if ( bike->bLeftFlash 	)	BL_Data[6] |= 0x08;	//S1
    } else {
     	if ( bike->bLeftFlash 	&& flashflag >= 5 )	BL_Data[6] |= 0x08;	//S1
    }
    if ( bike->bRFlashType ){
		if ( bike->bRightFlash	)	BL_Data[15]|= 0x80;	//S9
    } else {
     	if ( bike->bRightFlash && flashflag >= 5 )	BL_Data[15]|= 0x80;	//S9
    }
	
	if( bike->bCruise		) BL_Data[5] |= 0x02;	//S4
	if( bike->bNearLight 	) BL_Data[5] |= 0x01;	//S5
	if( bike->bHallERR 		) BL_Data[5] |= 0x08;	//S2	电机霍尔故障
	if( bike->bWheelERR 	) BL_Data[5] |= 0x04;	//S3	手把故障
	if( bike->bECUERR 		) BL_Data[15]|= 0x20;	//S7 	电机控制器故障
	//if( bike->bPhaseERR  	) BL_Data[15]|= 0x40;	//S8 	电机缺相故障
	if( bike->bBraked  		) BL_Data[15]|= 0x40;	//S8 	刹车

  /***************************Battery Area Display**********************************/
	BL_Data[16] |= 0x10;
	switch ( bike->ucBatStatus ){
    case 0:
		if ( flashflag < 5 ) 
			BL_Data[16]&= 0xEF;break;
    case 1: BL_Data[3] |= 0x10;break;
    case 2: BL_Data[3] |= 0x30;break;
    case 3: BL_Data[3] |= 0x70;break;
    case 4: BL_Data[3] |= 0xF0;break;
    case 5: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0x80;break;
    case 6: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xC0;break;      
    case 7: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xE0;break;
    case 8: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xF0;break;          
    default:break; 
	}

	/***************************Temp Area Display**********************************/
	BL_Data[3] |= (SegDataTemp[abs(bike->siTemperature/10)%10] & 0xF);
	BL_Data[4] |= (SegDataTemp[abs(bike->siTemperature/10)%10] & 0xF0);
	BL_Data[4] |= (SegDataTemp[abs(bike->siTemperature/10)/10] & 0xF);       
	BL_Data[5] |= (SegDataTemp[abs(bike->siTemperature/10)/10] & 0xF0);  
	BL_Data[3] |= 0x1;
	if (bike->siTemperature < 0)
		BL_Data[4] |= 0x01;       
		
	/***************************Time Area Display**********************************/
#if ( TIME_ENABLE == 1 )
	if ( bike->bHasTimer ){
		//DISPLAY 1 
		if(bike->ucHour > 9) BL_Data[8] |= 0x08; else BL_Data[8] |= 0x00;     
		BL_Data[8] |= ( SegDataTime[(bike->ucHour/10	)%10]);
		BL_Data[7] |= ( SegDataTime[(bike->ucMinute/10	)%10]);
		BL_Data[6] |= ( SegDataTime[ bike->ucMinute		 %10]);       
		if ( bike->bTimeSet ){
			switch ( bike->ucTimePos ){
			case 0:
				if ( flashflag < 5  ) { 
					BL_Data[8] &= 0xF7; 
					BL_Data[8] &= 0x08; 
					BL_Data[7] &= 0x08; 
					BL_Data[6] &= 0x08;
				}
				break;			
			case 1:if ( flashflag < 5  ) BL_Data[8] &= 0x08; break;
			case 2:if ( flashflag < 5  ) BL_Data[7] &= 0x08; break;
			case 3:if ( flashflag < 5  ) BL_Data[6] &= 0x08; break;
			default:break;		
			}
			BL_Data[7] |= 0x08;
		} else if ( flashflag < 5 ) BL_Data[7] |= 0x08;	//col
	}
#endif
	
	/*************************** Voltage Display**********************************/
	BL_Data[2] |= (SegDataVoltage[ bike->uiVoltage	 	 %10]) | 0x8;
	BL_Data[1] |= (SegDataVoltage[(bike->uiVoltage/10	)%10]) | 0x8;
	BL_Data[0] |= (SegDataVoltage[(bike->uiVoltage/100	)%10]); 
	// BL_Data[2] |= (SegDataVoltage[ bike->uiVoltage/10 %10]) | 0x8;
	// BL_Data[1] |= (SegDataVoltage[(bike->uiVoltage/100)%10]);

	/*************************** Mile Display**********************************/  
	BL_Data[12] |= (SegDataMile [ bike->ulMile	   	 %10]) | 0x1;
	BL_Data[11] |= (SegDataMile [(bike->ulMile/10   )%10]);
	BL_Data[10] |= (SegDataMile [(bike->ulMile/100  )%10]); 
	BL_Data[ 9] |= (SegDataMile [(bike->ulMile/1000 )%10] & 0x0F); 
	BL_Data[15] |= (SegDataMile [(bike->ulMile/1000 )%10] >> 4);
	BL_Data[16] |= (SegDataMile2[(bike->ulMile/10000)%10]); 
	if ( (bike->bMileFlash && flashflag < 5) ) {
		BL_Data[12]  = 0x1;
		BL_Data[11]  = 0;
		BL_Data[10]  = 0; 
		BL_Data[ 9] &= 0xF0; 
		BL_Data[15] &= 0xF0;
		BL_Data[16]  = 0; 
	}		

	/*************************** Speed Display**********************************/
	BL_Data[14] |= (SegDataSpeed[ bike->ucSpeed	 	%10]);
	BL_Data[13] |= (SegDataSpeed[(bike->ucSpeed/10)	%10]); 
	BL_Data[14] |= 0x80;
	if ( bike->bSpeedFlash ){
		if ( flashflag < 5  ) {
			BL_Data[14] = 0x80;
			BL_Data[13] = 0; 
		}
	}

	/*************************** Mode Display**********************************/ 
	switch (bike->ucSpeedMode){
    case 1: BL_Data[11] |= 0x01;break;
    case 2: BL_Data[10] |= 0x01;break;
    case 3: BL_Data[9 ] |= 0x01;break;
	case 4: BL_Data[13] |= 0x80;break;
    default:
			BL_Data[11] &= ~0x01;
			BL_Data[10] &= ~0x01;
			BL_Data[ 9] &= ~0x01;
			BL_Data[13] &= ~0x80;
			break;
	}
  
	BL_Write_Data(0,18,BL_Data);
}


void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

