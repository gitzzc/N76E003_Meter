/**
  ******************************************************************************
  * @file    bike.c
  * @author  
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "adc.h"
#include "flash.h"

#include "bl55072.h"
#include "display.h"
#include "bike.h"
#include "YXT.h"

/*----------------------------------------------------------*/
void GetSysVoltage(void);
uint8_t GPIO_Read(uint8_t port, uint8_t pin);
uint8_t GetSpeed(void);

/*----------------------------------------------------------*/
extern const uint16_t uiBatStatus48[8];
extern const uint16_t uiBatStatus60[8];
extern const uint16_t uiBatStatus72[8];

/*----------------------------------------------------------*/
BIKE_STATUS xdata sBike;
BIKE_CONFIG xdata sConfig; 
volatile uint16_t  uiSysTick = 0;

/*----------------------------------------------------------*/

void exchange_sort16(uint16_t* pData,uint16_t Count)
{
	uint16_t iTemp;
	uint16_t i,j;
	
	for(i=0;i<Count-1;i++){
		for(j=i+1;j<Count;j++){
			if(pData[j]<pData[i]){
				iTemp = pData[i];
				pData[i] = pData[j];
				pData[j] = iTemp;
			}
		}
	}
}

uint16_t get_average16(uint16_t* dat, uint16_t len)
{
	uint32_t sum;
	uint16_t i;
	
	for(sum=0,i=0;i<len;i++)
		sum += dat[i];	
	return (sum/len);
}

uint16_t Get_SysTick(void)
{
	uint16_t uiTick;
	
	DISABLE_INTERRUPTS();
	uiTick = uiSysTick;
	ENABLE_INTERRUPTS();
	
	return uiTick;
}

uint16_t Get_ElapseTick(uint16_t uiPreTick)
{
	uint16_t uiTick = Get_SysTick();

	if ( uiTick >= uiPreTick )	
		return (uiTick - uiPreTick); 
	else 
		return (0xFFFF - uiPreTick + uiTick);
}

#if 0
const int32_t NTC_B3450[29][2] = 
{
	251783,	-400,	184546,	-350,	137003,	-300,	102936,	-250,	78219,	-200,
	60072,	-150,	46601,	-100,	36495,	-50,	28837,	0,		22980,	50,
	18460,	100,	14942,	150,	12182,	200,	10000,	250,	8263,	300,
	6869,	350,	5745,	400,	4832,	450,	4085,	500,	3472,	550,
	2965,	600,	2544,	650,	2193,	700,	1898,	750,	1649,	800,
	1439,	850,	1260,	900,	1108,	950,	977,	1000
};

int32_t NTCtoTemp(int32_t ntc)
{
	uint8_t i,j;

	if ( ntc > NTC_B3450[0][0] ){
		return NTC_B3450[0][1];
	} else {
		for(i=0;i<sizeof(NTC_B3450)/sizeof(NTC_B3450[0][0])/2-1;i++){
			if ( ntc <= NTC_B3450[i][0] && ntc > NTC_B3450[i+1][0] )
				break;
		}
		if ( i == sizeof(NTC_B3450)/sizeof(NTC_B3450[0][0])/2-1 ){
			return NTC_B3450[28][1];
		} else {
			for(j=0;j<50;j++){
				if ( NTC_B3450[i][0] - (j*(NTC_B3450[i][0] - NTC_B3450[i+1][0])/50) <= ntc )
					return NTC_B3450[i][1] + j;
			}
			return NTC_B3450[i+1][1];
		}
	}
}
#endif

const uint16_t NTC_B3450[][2] = 
{
	60072,	0,		46601,	50,		36495,	100,	28837,	150,	22980,	200,
	18460,	250,	14942,	300,	12182,	350,	10000,	400,	8263,	450,
	6869,	500,	5745,	550,	4832,	600,	4085,	650,	3472,	700,
	2965,	750,	2544,	800,	2193,	850,	1898,	900,	1649,	950,
	1439,	1000,	1260,	1050,	1108,	1100,	977,	1150
};

int16_t NTCtoTemp(int16_t ntc)
{
	uint8_t i,j;

	if ( ntc > NTC_B3450[0][0] ){
		return (int16_t)NTC_B3450[0][1] - 150;
	} else {
		for(i=0;i<ContainOf(NTC_B3450)/2-1;i++){
			if ( ntc <= NTC_B3450[i][0] && ntc > NTC_B3450[i+1][0] )
				break;
		}
		if ( i == ContainOf(NTC_B3450)/2-1 ){
			return (int16_t)NTC_B3450[i][1] - 150;
		} else {
			for(j=0;j<50;j++){
				if ( NTC_B3450[i][0] - (j*(NTC_B3450[i][0] - NTC_B3450[i+1][0])/50) <= ntc )
					return (int16_t)NTC_B3450[i][1] + j - 150;
			}
			return (int16_t)NTC_B3450[i+1][1] - 150;
		}
	}
}

void HotReset(void)
{
	if (sConfig.ucBike[0] == 'b' &&
	//	sConfig.ucBike[1] == 'i' && 
	//	sConfig.ucBike[2] == 'k' && 
		sConfig.ucBike[3] == 'e' ){
		sBike.bHotReset = 1;
	} else {
		sBike.bHotReset = 0;
	}
}

void WriteConfig(void)
{
	uint8_t *cbuf = (uint8_t *)&sConfig;
	uint8_t i;

	sConfig.ucBike[0] = 'b';
	sConfig.ucBike[1] = 'i';
	sConfig.ucBike[2] = 'k';
	sConfig.ucBike[3] = 'e';
//	sConfig.uiVersion = SW_VER;
	for(sConfig.ucSum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sConfig.ucSum += cbuf[i];
		
	FlashWrite(cbuf,sizeof(BIKE_CONFIG));
}
void ResetConfig(void)
{
	sConfig.uiSysVoltage 	= 60;
	sConfig.uiVolScale  	= 1000;
	sConfig.uiTempScale 	= 1000;
	sConfig.uiSpeedScale	= 1000;
	sConfig.uiYXT_SpeedScale= 1000;
#ifdef SINGLE_TRIP
	sConfig.uiSingleTrip	= 1;
#else
	sConfig.uiSingleTrip	= 0;
#endif
	sConfig.ulMile			= 0;
	WriteConfig();
}

void InitConfig(void)
{
	uint8_t *cbuf = (uint8_t *)&sConfig;
	uint8_t i,sum;

	FlashRead(cbuf,sizeof(BIKE_CONFIG));

	for(sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sum += cbuf[i];
		
	if (sConfig.ucBike[0] != 'b' || 
		sConfig.ucBike[1] != 'i' || 
		sConfig.ucBike[2] != 'k' || 
		sConfig.ucBike[3] != 'e' || 
	//	sConfig.uiVersion != SW_VER || 
		sum != sConfig.ucSum ){
		ResetConfig();
	}

	sBike.ulMile = sConfig.ulMile;
	sBike.bHasTimer = 0;
	sBike.ulFMile 		= 0;
	sBike.ucSpeedMode = 0;
	sBike.bYXTERR = 1;

	GetSysVoltage();
}

uint8_t GetBatStatus(uint16_t uiVol)
{
	uint8_t i;
	const uint16_t *uiBatStatus;
	uint8_t ucLength;

	switch ( sConfig.uiSysVoltage ){
	case 48:uiBatStatus = uiBatStatus48;ucLength = ContainOf(uiBatStatus48);break;
	case 60:uiBatStatus = uiBatStatus60;ucLength = ContainOf(uiBatStatus60);break;
	case 72:uiBatStatus = uiBatStatus72;ucLength = ContainOf(uiBatStatus72);break;
	default:uiBatStatus = uiBatStatus60;ucLength = ContainOf(uiBatStatus60);break;
	}

	for(i=0;i<ucLength;i++)
		if ( uiVol < uiBatStatus[i] ) break;
	return i;
}

#ifdef SPEED_HALL_PORT
uint8_t GetSpeedHall(void)
{
	uint16_t count;
	uint32_t speed;
	
	DISABLE_INTERRUPTS();
	count = sBike.uiHallCounter_250ms;
	ENABLE_INTERRUPTS();
	
	//speed = PERIMETER * 60 * 60 * count / 1000 / 1000 / PULSE_C;
	speed = count * 4;
	speed = PERIMETER * 36 * speed / 10 / 1000 / PULSE_C;
	if ( speed < 25 )
		speed = speed * 244 / 100;
	else
		speed = speed * 260 / 100;
	if (speed > 65)
		speed = 65;
	return speed;
}
#endif

#if 0
#ifdef LCD8794GCT

const uint16_t BatEnergy48[] = {420,490};
const uint16_t BatEnergy60[] = {520,620};
const uint16_t BatEnergy72[] = {630,740};

uint8_t GetBatEnergy(uint16_t uiVol)
{
	uint16_t uiEnergy ;
	const uint16_t* BatEnergy;
	
	switch ( sConfig.uiSysVoltage ){
	case 48:BatEnergy = BatEnergy48;break;
	case 60:BatEnergy = BatEnergy60;break;
	case 72:BatEnergy = BatEnergy72;break;
	default:BatEnergy = BatEnergy60;break;
	}

	if ( sBike.uiVoltage <= BatEnergy[0] ) uiEnergy = 0;
	else if ( sBike.uiVoltage >= BatEnergy[1] ) uiEnergy = 100;
	else {
		uiEnergy = (sBike.uiVoltage - BatEnergy[0])*100/(BatEnergy[1] - BatEnergy[0]);
	}
	return uiEnergy;
}
#endif
#endif

void LRFlashTask(void)
{
	static uint8_t ucLeftOn=0	,ucLeftOff=0;	//开启计时器
	static uint8_t ucRightOn=0	,ucRightOff=0;	//关闭计时器
	static uint8_t ucLeftCount=0,ucRightCount=0;//开启时间计时器

	if ( READ_TURN_LEFT() ){	//ON
        ucLeftOff = 0;
        if ( ucLeftOn ++ > 10 ){		//200ms 滤波
            if ( ucLeftOn > 100 ){
          	    ucLeftOn = 101;
                sBike.bLFlashType = 0;	//长时间开启，为开关信号
            }
           	if ( ucLeftCount < 0xFF-50 ){
	            ucLeftCount++;
            }
			sBike.bLeftFlash= 1;		//闪光信号
			sBike.bTurnLeft = 1;		//转向信号
        }
	} else {					//OFF
        ucLeftOn = 0;
        if ( ucLeftOff ++ == 10 ){
        	ucLeftCount += 25;			//在开启时间上加500ms
			sBike.bLeftFlash = 0;
        } else if ( ucLeftOff > 10 ){
	        ucLeftOff = 11;
            sBike.bLFlashType = 1;		//闪光器信号
            if ( ucLeftCount == 0 ){
				sBike.bTurnLeft = 0;
            } else
				ucLeftCount --;
		}
	}
	
	if ( READ_TURN_RIGHT() ){	//ON
        ucRightOff = 0;
        if ( ucRightOn ++ > 10 ){
            if ( ucRightOn > 100 ){
          	    ucRightOn = 101;
                sBike.bRFlashType = 0;
            }
           	if ( ucRightCount < 0xFF-50 ){
				ucRightCount++;
            }
			sBike.bRightFlash= 1;
			sBike.bTurnRight = 1;
        }
	} else {					//OFF
        ucRightOn = 0;
        if ( ucRightOff ++ == 10 ){
        	ucRightCount += 25;	//500ms
			sBike.bRightFlash = 0;
        } else if ( ucRightOff > 10 ){
	        ucRightOff = 11;
            sBike.bRFlashType = 1;
            if ( ucRightCount == 0 ){
				sBike.bTurnRight = 0;
            } else
				ucRightCount --;
		}
	}
}

uint8_t MileSetupTask(void)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucCount = 0;
	uint8_t ret = 0;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && sBike.bTurnRight == 1 ){
			TaskFlag = TASK_STEP1;
			ucCount = 0;
		}
		break;
	case TASK_STEP1:
		if ( sBike.bLastNear == 0 && sBike.bNearLight ){
			uiPreTick = Get_SysTick();
			if ( ++ucCount >= 8 ){
				TaskFlag = TASK_STEP2;
				ucCount = 0;
				sBike.bMileFlash = 1;
				sBike.ulMile = sConfig.ulMile;
			} 
		}
		sBike.bLastNear = sBike.bNearLight;
		ret = 1;
		break;
	case TASK_STEP2:
		if ( sBike.bTurnRight == 0 && sBike.bTurnLeft == 1 ) {
			ucCount = 0;
			TaskFlag = TASK_EXIT;
			sBike.ulFMile 	= 0;
			sBike.ulMile 	= 0;
			sConfig.ulMile 	= 0;
			WriteConfig();
		} else if ( sBike.bTurnRight == 0 && sBike.bTurnLeft == 0 ) {
			if ( sBike.bLastNear == 0 && sBike.bNearLight){
				uiPreTick = Get_SysTick();
				if ( ++ucCount >= 4 ){
					TaskFlag = TASK_STEP3;
					if ( sConfig.uiSingleTrip ){
						sConfig.uiSingleTrip = 0;
						sBike.ulMile = 22222UL;
					} else {
						sConfig.uiSingleTrip = 1;
						sBike.ulMile = 11111UL;
					}
					WriteConfig();
				}
			}
		}
		sBike.bLastNear = sBike.bNearLight;
		ret = 1;
		break;
	case TASK_STEP3:
		if ( Get_ElapseTick(uiPreTick) > 3000 ) {
			TaskFlag = TASK_EXIT;
			if ( sConfig.uiSingleTrip )
				sBike.ulMile = 0;
			else
				sBike.ulMile = sConfig.ulMile;
		}
		ret = 1;
		break;
	case TASK_EXIT:
	default:
			sBike.bMileFlash = 0;
		break;
	}

	if ( Get_ElapseTick(uiPreTick) > 10000/* || sBike.bBraked || sBike.ucSpeed*/ )
		TaskFlag = TASK_EXIT;
	
	return ret;
}

void MileTask(void)
{
#define MT_INIT					0
#define MT_SHOW_TOTAL_MILE_2S	1
#define MT_WAIT_SPEED			2
#define MT_SHOW_MILE			3

	static uint16_t uiTime = 0;
	static uint8_t task=MT_INIT;
	
	if ( MileSetupTask() ){
		task	= MT_INIT;
		return ;
	}

	uiTime ++;
	switch( task ){
	case MT_INIT:
		if ( sConfig.uiSingleTrip == 0 )
			task = MT_SHOW_MILE;
		else
			task = MT_SHOW_TOTAL_MILE_2S;
		sBike.ulMile  = sConfig.ulMile;
		sBike.ulFMile = 0;
		uiTime 	= 0;
		break;
	case MT_SHOW_TOTAL_MILE_2S:
		if ( uiTime > 20 )	//2s
			task = MT_WAIT_SPEED;
		break;
	case MT_WAIT_SPEED:
		if ( uiTime > 50 || sBike.ucSpeed > 0 ){
			task = MT_SHOW_MILE;
			sBike.ulMile = 0;
		}
		break;	
	case MT_SHOW_MILE:
		if ( sBike.ucSpeed > DISPLAY_MAX_SPEED )
			sBike.ulFMile += DISPLAY_MAX_SPEED;
		else
			sBike.ulFMile += sBike.ucSpeed;
		
		if(sBike.ulFMile >= 36000)
		{
			sBike.ulFMile = 0;
			sBike.ulMile++;
			if ( sBike.ulMile > 99999 )		sBike.ulMile = 0;
			sConfig.ulMile ++;
			if ( sConfig.ulMile > 99999 )	sConfig.ulMile = 0;
			WriteConfig();
		}  
		break;
	default:
		task = MT_INIT;
		break;
	}
#undef MT_INIT
#undef MT_SHOW_TOTAL_MILE_2S
#undef MT_WAIT_SPEED
#undef MT_SHOW_MILE	
}

uint8_t SpeedCaltTask(void)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucLastSpeed = 0;
	static uint8_t ucCount = 0;
    static signed char scSpeedInc=0;
	static uint8_t yxterr=0;
	signed char scSpeed;
	
	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && sBike.bTurnLeft == 1 ){
			TaskFlag = TASK_STEP1;
			ucCount = 0;
		}
		break;
	case TASK_STEP1:
		if ( sBike.bLastNear == 0 && sBike.bNearLight){
			if ( ++ucCount >= 8 ){
				TaskFlag = TASK_STEP2;
				ucCount 	= 0;
				scSpeedInc 	= 0;
				sBike.bSpeedFlash = 1;
				yxterr = sBike.bYXTERR;
				if ( yxterr )
					sConfig.uiSpeedScale 	 = 1000;
				else
					sConfig.uiYXT_SpeedScale = 1000;
			}
			uiPreTick = Get_SysTick();
		}
		sBike.bLastNear  = sBike.bNearLight;
		break;
	case TASK_STEP2:
		if ( sBike.bLastNear == 0 && sBike.bNearLight == 1 ){
			uiPreTick = Get_SysTick();
            if ( sBike.bTurnLeft == 1 ) {
				ucCount = 0;
				if ( sBike.ucSpeed + scSpeedInc > 1 )
					scSpeedInc --;
	        } else if ( sBike.bTurnRight == 1 ) {
				ucCount = 0;
                if ( sBike.ucSpeed + scSpeedInc < 99 )
					scSpeedInc ++;
            } else {
				if ( ++ucCount >= 4 ){
					TaskFlag = TASK_EXIT;
					sBike.bSpeedFlash = 0;
					if ( sBike.ucSpeed ) {
						if ( yxterr )
							sConfig.uiSpeedScale 	 = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+scSpeedInc);
						else
							sConfig.uiYXT_SpeedScale = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+scSpeedInc);
						WriteConfig();
					}
				}
			}
		}
		sBike.bLastNear = sBike.bNearLight;
        
		if ( sBike.ucSpeed )
			uiPreTick = Get_SysTick();

		scSpeed = sBike.ucSpeed + scSpeedInc;
		if ( scSpeed < 0 )
			sBike.ucSpeed = 0;
		else
			sBike.ucSpeed = scSpeed;
			
		if ( ucLastSpeed && sBike.ucSpeed == 0 )
			TaskFlag = TASK_EXIT;
			
		ucLastSpeed = sBike.ucSpeed;
		break;
	case TASK_EXIT:
	default:
		sBike.bSpeedFlash = 0;
		break;
	}
	if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bBraked )
		TaskFlag = TASK_EXIT;

	return 0;
}

#if ( TIME_ENABLE == 1 )
void TimeTask(void)
{
	static uint8_t ucTask=0,ucLeftOn= 0,NL_on = 0;
	static uint16_t uiPreTick;
	
	if (!sBike.bHasTimer)
		return ;
	
	if (sBike.ucSpeed > 1)
		ucTask = 0xff;
	
	switch ( ucTask ){
	case 0:
		if ( Get_SysTick() < 5000 && sBike.bNearLight == 0 && sBike.bTurnLeft == 1 ){
			uiPreTick = Get_SysTick();
			ucTask++;
		}
		break;
	case 1:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 2:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 3:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 4:
		if ( sBike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 5:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 6:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 7:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 8:
		if ( sBike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 9:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 10:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 11:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 12:
		if ( sBike.bTurnLeft == 1 || sBike.bNearLight ){
			 ucTask = 0xFF;
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000 ) {
				ucTask= 0;
				sBike.ucTimePos = 0;
				sBike.bTimeSet = 1; 
				uiPreTick = Get_SysTick();
			}
		}
		break;
	default:
		sBike.ucTimePos = 0;
		sBike.bTimeSet = 0; 
		ucTask = 0;
		break;
	}

	if ( sBike.bTimeSet ){
		if ( sBike.bTurnLeft ) { ucLeftOn = 1; }
		if ( sBike.bTurnLeft == 0 ) {
			if ( ucLeftOn == 1 ){
				sBike.ucTimePos ++;
				sBike.ucTimePos %= 4;
				ucLeftOn = 0;
				uiPreTick = Get_SysTick();
			}
		}
		if ( sBike.bNearLight ) { NL_on = 1; uiPreTick = Get_SysTick(); }
		if ( sBike.bNearLight == 0 && NL_on == 1 ) {
			NL_on = 0;
			if ( Get_ElapseTick(uiPreTick) < 5000 ){
				switch ( sBike.ucTimePos ){
				case 0:
					sBike.ucHour += 10;
					sBike.ucHour %= 20;
					break;
				case 1:
					if ( sBike.ucHour % 10 < 9 )
						sBike.ucHour ++;
					else 
						sBike.ucHour -= 9;
					break;
				case 2:
					sBike.ucMinute += 10;
					sBike.ucMinute %= 60;
					break;
				case 3:
					if ( sBike.ucMinute % 10 < 9 )
						sBike.ucMinute ++;
					else 
						sBike.ucMinute -= 9;
					break;
				default:
					sBike.bTimeSet = 0;
					break;
				}
			}
			RtcTime.RTC_Hours 	= sBike.ucHour;
			RtcTime.RTC_Minutes = sBike.ucMinute;
			PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		}
		if ( Get_ElapseTick(uiPreTick) > 30000 ){
			sBike.bTimeSet = 0;
		}
	}		
	
	 PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	 sBike.ucHour 		= RtcTime.RTC_Hours%12;
	 sBike.ucMinute 	= RtcTime.RTC_Minutes;
}
#endif 

void LightTask(void)
{
	uint8_t ucSpeedMode;

	if( GPIO_Read(NearLight_PORT, 	NearLight_PIN) ) sBike.bNearLight = 1; else sBike.bNearLight = 0;
	//if( GPIO_Read(TurnRight_PORT, TurnRight_PIN) ) sBike.bTurnRight = 1; else sBike.bTurnRight = 0;
	//if( GPIO_Read(TurnLeft_PORT, 	TurnLeft_PIN ) ) sBike.bTurnLeft  = 1; else sBike.bTurnLeft  = 0;
#ifdef OverSpeed_PORT
	if( GPIO_Read(OverSpeed_PORT, 	OverSpeed_PIN) ) sBike.bOverSpeed = 0; else sBike.bOverSpeed = 1;
#endif
#ifdef FarLight_PORT
	if( GPIO_Read(FarLight_PORT, 	FarLight_PIN ) ) sBike.bFarLight  = 1; else sBike.bFarLight  = 0;
#endif
	
	if ( sBike.bYXTERR ){
		ucSpeedMode = 0;
	#ifdef SPMODE1_PORT
		if( GPIO_Read(SPMODE1_PORT,SPMODE1_PIN) ) ucSpeedMode |= 1<<0;
		if( GPIO_Read(SPMODE2_PORT,SPMODE2_PIN) ) ucSpeedMode |= 1<<1;
		if( GPIO_Read(SPMODE3_PORT,SPMODE3_PIN) ) ucSpeedMode |= 1<<2;
	#endif
	#ifdef SPMODE4_PORT
		if( GPIO_Read(SPMODE4_PORT,SPMODE4_PIN) ) ucSpeedMode |= 1<<3;
	#endif
		switch(ucSpeedMode){
			case 0x01: 	sBike.ucSpeedMode = 1; break;
			case 0x02: 	sBike.ucSpeedMode = 2; break;
			case 0x04: 	sBike.ucSpeedMode = 3; break;
			case 0x08: 	sBike.ucSpeedMode = 4; break;
			default:	sBike.ucSpeedMode = 0; break;
		}
		sBike.ucPHA_Speed	= GetSpeed();
		sBike.ucSpeed 	 = (uint32_t)sBike.ucPHA_Speed*1000UL/sConfig.uiSpeedScale;
	}
}


/*----------------------------------------------------------*/

