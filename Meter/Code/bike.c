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

#include "bl55072.h"
#include "display.h"
#include "pcf8563.h"
#include "bike.h"
#include "YXT.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef JINPENG_4860
const uint16_t uiBatStatus48[8] = {420,426,432,439,445,451,457,464};
const uint16_t uiBatStatus60[8] = {520,528,536,542,550,558,566,574};
const uint16_t uiBatStatus72[8] = {0};
#elif defined JINPENG_6072
const uint16_t uiBatStatus48[8] = {0};
const uint16_t uiBatStatus60[8] = {480,493,506,519,532,545,558,570};
const uint16_t uiBatStatus72[8] = {550,569,589,608,628,647,667,686};
#elif defined LCD6040
const uint16_t uiBatStatus48[] = {425,432,444,456,468};
const uint16_t uiBatStatus60[] = {525,537,553,566,578};
const uint16_t uiBatStatus72[] = {630,641,661,681,701};
#else
const uint16_t uiBatStatus48[8] = {420,427,435,444,453,462,471,481};
const uint16_t uiBatStatus60[8] = {520,531,544,556,568,577,587,595};
const uint16_t uiBatStatus72[8] = {630,642,653,664,675,687,700,715};
#endif

BIKE_STATUS sBike;
__no_init BIKE_CONFIG sConfig;
volatile uint16_t  uiSysTick = 0;
uint16_t uiSpeedBuf[16];
uint16_t uiVolBuf[28];
uint16_t uiTempBuf[4];

#if ( TIME_ENABLE == 1 )
uint8_t ucUart1Buf[16];
uint8_t ucUart1Index=0;
#endif
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/*----------------------------------------------------------*/

int GetTemp(void)
{
	static uint8_t ucIndex = 0;
	int32_t slTemp;
	uint8_t i;

	//GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);  //Temp
	//ADC1_DeInit();  
	ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D2, \
				ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL6 ,\
				DISABLE);
	ADC1_Cmd(ENABLE);
	Delay(1000);
	ADC1_StartConversion(); 
	while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
	slTemp = ADC1_GetConversionValue();
	ADC1_Cmd(DISABLE);
  
	uiTempBuf[ucIndex++] = slTemp;
	if ( ucIndex >= ContainOf(uiTempBuf) )
		ucIndex = 0;
	for(i=0,slTemp=0;i<ContainOf(uiTempBuf);i++)
		slTemp += uiTempBuf[i];
	slTemp /= ContainOf(uiTempBuf);

	//slTemp = 470UL*1024/(1024-slTemp)-470;
	//slTemp = NTCtoTemp(slTemp)/10;
	//slTemp = ((3600- (long)slTemp * 2905/1024)/10);

	slTemp = 10000*1024UL/(1024-slTemp)-10000;
	slTemp = NTCtoTemp(slTemp);
	if ( slTemp > 999  ) slTemp =  999;
	if ( slTemp < -999 ) slTemp = -999;
	
	return slTemp;
}
#if 0
uint16_t GetVol(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiVol;
	uint8_t i;

	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);  //B+  
	ADC1_DeInit();  
	ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
				ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL2,\
				DISABLE);
	ADC1_Cmd(ENABLE);
	Delay(5000);  
	ADC1_StartConversion(); 
	while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
	uiVol = ADC1_GetConversionValue();
	ADC1_Cmd(DISABLE);

	uiVolBuf[ucIndex++] = uiVol;
	if ( ucIndex >= ContainOf(uiVolBuf) )
		ucIndex = 0;
	for(i=0,uiVol=0;i<ContainOf(uiVolBuf);i++)
		uiVol += uiVolBuf[i];
	uiVol /= ContainOf(uiVolBuf);
	uiVol = (uint32_t)uiVol*1050/1024 ;
	
	return uiVol;
}
#else
uint8_t GetVolStabed(uint16_t* uiVol)
{
	uint32_t ulMid;
	uint16_t uiBuf[32];
	uint8_t i;
	
	//GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);  //B+  
	//ADC1_DeInit();  
	ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
				ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL2,\
				DISABLE);

	ADC1_Cmd(ENABLE);
	for(i=0;i<32;i++){
		Delay(500);  
		ADC1_StartConversion(); 
		while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
		uiBuf[i] = ADC1_GetConversionValue();
	}
	ADC1_Cmd(DISABLE);
	
	*uiVol = (uint32_t)uiBuf[0]*1050UL/1024UL;

	for(i=0,ulMid=0;i<32;i++)	ulMid += uiBuf[i];
	ulMid /= 32;
	for( i=0;i<32;i++){
		if ( ulMid > 5 && ((ulMid *100 / uiBuf[i]) > 101 ||  (ulMid *100 / uiBuf[i]) < 99) )
			return 0;
	}
	
	return 1;
}

#endif

#if ( PCB_VER == 0013 )
uint8_t GetSpeedAdj(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiAdj;
	uint8_t i;

	ADC1_DeInit();  
	ADC1_Init(	ADC1_CONVERSIONMODE_CONTINUOUS, SPEEDV_ADJ_CH, ADC1_PRESSEL_FCPU_D2,\
				ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, SPEEDV_ADJ_SCH,DISABLE);

	ADC1_Cmd(ENABLE);
	Delay(1000);  
	ADC1_StartConversion(); 
	while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
	uiAdj = ADC1_GetConversionValue();
	ADC1_Cmd(DISABLE);
  	
	uiSpeedBuf[ucIndex++] = uiAdj;
	if ( ucIndex >= ContainOf(uiSpeedBuf) )
		ucIndex = 0;

	for(i=0,uiAdj=0;i<ContainOf(uiSpeedBuf);i++)
		uiAdj += uiSpeedBuf[i];
	uiAdj /= ContainOf(uiSpeedBuf);
	
	if ( uiAdj > 99 )
		uiAdj = 99;
	
  return uiAdj;
}
#endif

uint8_t GetSpeed(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiSpeed;
	uint8_t i;

	//GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	//ADC1_DeInit();  
	ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, SPEEDV_ADC_CH, ADC1_PRESSEL_FCPU_D2, \
			ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, SPEEDV_ADC_SCH,\
			DISABLE);

	ADC1_Cmd(ENABLE);
	Delay(1000);  
	ADC1_StartConversion(); 
	while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
	uiSpeed = ADC1_GetConversionValue();
	ADC1_Cmd(DISABLE);
  	
	uiSpeedBuf[ucIndex++] = uiSpeed;
	if ( ucIndex >= ContainOf(uiSpeedBuf) )
		ucIndex = 0;

	for(i=0,uiSpeed=0;i<ContainOf(uiSpeedBuf);i++)
		uiSpeed += uiSpeedBuf[i];
	uiSpeed /= ContainOf(uiSpeedBuf);
	
	if ( sConfig.uiSysVoltage		== 48 ){
		uiSpeed = SPEED_CALC_48V((uint32_t)uiSpeed);
	} else if ( sConfig.uiSysVoltage== 60 ) {
		uiSpeed = SPEED_CALC_60V((uint32_t)uiSpeed);
	} else if ( sConfig.uiSysVoltage== 72 )	{
		uiSpeed = SPEED_CALC_72V((uint32_t)uiSpeed);
	}
	if ( uiSpeed > 99 )
		uiSpeed = 99;
	
  return uiSpeed;
}

/*----------------------------------------------------------*/


uint16_t Get_SysTick(void)
{
	uint16_t uiTick;
	
	disableInterrupts();
	uiTick = uiSysTick;
	enableInterrupts();
	
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
	for(sConfig.ucSum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sConfig.ucSum += cbuf[i];
		
	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		FLASH_ProgramByte(0x4000+i, cbuf[i]);
}

void InitConfig(void)
{
	uint8_t *cbuf = (uint8_t *)&sConfig;
	uint8_t i,sum;

	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		cbuf[i] = FLASH_ReadByte(0x4000 + i);

	for(sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sum += cbuf[i];
		
	if (sConfig.ucBike[0] != 'b' || 
		//sConfig.ucBike[1] != 'i' || 
		//sConfig.ucBike[2] != 'k' || 
		//sConfig.ucBike[3] != 'e' || 
		sum != sConfig.ucSum ){
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
	}

	sBike.ulMile = sConfig.ulMile;
#if ( TIME_ENABLE == 1 )
	sBike.bHasTimer = 0;
#endif
	sBike.ulFMile = 0;
	sBike.ucSpeedMode = 0;
	sBike.bYXTERR = 1;
}

uint8_t GetBatStatus(uint16_t uiVol)
{
	uint8_t i;
	const uint16_t *uiBatStatus;

	switch ( sConfig.uiSysVoltage ){
	case 48:uiBatStatus = uiBatStatus48;break;
	case 60:uiBatStatus = uiBatStatus60;break;
	case 72:uiBatStatus = uiBatStatus72;break;
	default:uiBatStatus = uiBatStatus60;break;
	}

	for(i=0;i<ContainOf(uiBatStatus60);i++)
		if ( uiVol < uiBatStatus[i] ) break;
	return i;
}

#if 0
#ifdef LCD8794GCT

const uint16_t BatEnergy48[8] = {420,490};
const uint16_t BatEnergy60[8] = {520,620};
const uint16_t BatEnergy72[8] = {630,740};

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
	static uint8_t ucLeftOn=0	,ucLeftOff=0;
	static uint8_t ucRightOn=0	,ucRightOff=0;
	static uint8_t ucLeftCount=0,ucRightCount=0;

	if ( READ_TURN_LEFT() ){	//ON
        ucLeftOff = 0;
        if ( ucLeftOn ++ > 10 ){		//200ms ÂË²¨
            if ( ucLeftOn > 100 ){
          	    ucLeftOn = 101;
                sBike.bLFlashType = 0;
            }
           	if ( ucLeftCount < 0xFF-50 ){
	            ucLeftCount++;
            }
			sBike.bLeftFlash= 1;
			sBike.bTurnLeft = 1;
        }
	} else {					//OFF
        ucLeftOn = 0;
        if ( ucLeftOff ++ == 10 ){
        	ucLeftCount += 50;	//500ms
			sBike.bLeftFlash = 0;
        } else if ( ucLeftOff > 10 ){
	        ucLeftOff = 11;
            sBike.bLFlashType = 1;
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
        	ucRightCount += 50;	//500ms
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

uint8_t MileResetTask(void)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucCount = 0;
	uint8_t ret=0;
	
    if ( TaskFlag == TASK_EXIT )
        return 0;
    
	if ( Get_ElapseTick(uiPreTick) > 10000 | sBike.bBraked | sBike.ucSpeed )
		TaskFlag = TASK_EXIT;

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
			sBike.bMileFlash = 0;
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
						sBike.ulMile = 99999UL;
					} else {
						sConfig.uiSingleTrip = 1;
						sBike.ulMile = 0;
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
			sBike.bMileFlash = 0;
		}
		ret = 1;
		break;
	case TASK_EXIT:
	default:
		sBike.bMileFlash = 0;
		ret = 0;
		break;
	}
	
	return ret;
}

void MileTask(void)
{
	static uint16_t uiTime = 0;
	uint8_t uiSpeed;
	
	if ( MileResetTask() )
		return ;
	
	uiSpeed = sBike.ucSpeed;
	if ( uiSpeed > DISPLAY_MAX_SPEED )
		uiSpeed = DISPLAY_MAX_SPEED;

//#ifdef SINGLE_TRIP
	uiTime ++;
	if ( uiTime < 20 ) {	//2s
		if ( sConfig.uiSingleTrip == 0 )
			uiTime = 51;
		sBike.ulMile = sConfig.ulMile;
	} else if ( uiTime < 50 ) { 	//5s
		if ( uiSpeed ) {
			uiTime = 50;
			sBike.ulMile = 0;
		}
	} else if ( uiTime == 50 ){
		sBike.ulMile = 0;
	} else 
//#endif	
	{
		uiTime = 51;
		
		sBike.ulFMile = sBike.ulFMile + uiSpeed;
		if(sBike.ulFMile >= 36000)
		{
			sBike.ulFMile = 0;
			sBike.ulMile++;
			if ( sBike.ulMile > 99999 )		sBike.ulMile = 0;
			sConfig.ulMile ++;
			if ( sConfig.ulMile > 99999 )	sConfig.ulMile = 0;
			WriteConfig();
		}  
	}
}

uint8_t SpeedCaltTask(void)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucLastSpeed = 0;
	static uint8_t ucCount = 0;
    static signed char cSpeedInc=0;
	static uint8_t yxterr=0;
	
    if ( TaskFlag == TASK_EXIT )
      	return 0;
    
	if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bBraked )
		TaskFlag = TASK_EXIT;

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
				cSpeedInc 	= 0;
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
        //if ( sConfig.uiSysVoltage == 48 )
		//	sBike.ucSpeed = 42;
        //else if ( sConfig.uiSysVoltage == 60 )
		//	sBike.ucSpeed = 44;
        
		if ( sBike.bLastNear == 0 && sBike.bNearLight == 1 ){
			uiPreTick = Get_SysTick();
            if ( sBike.bTurnLeft == 1 ) {
				ucCount = 0;
				if ( sBike.ucSpeed + cSpeedInc > 1 )
					cSpeedInc --;
	        } else if ( sBike.bTurnRight == 1 ) {
				ucCount = 0;
                if ( sBike.ucSpeed + cSpeedInc < 99 )
					cSpeedInc ++;
            } else {
				if ( ++ucCount >= 5 ){
					TaskFlag = TASK_EXIT;
					sBike.bSpeedFlash = 0;
					if ( sBike.ucSpeed ) {
						if ( yxterr )
							sConfig.uiSpeedScale 	 = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+cSpeedInc);
						else
							sConfig.uiYXT_SpeedScale = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+cSpeedInc);
						WriteConfig();
					}
				}
			}
		}
		sBike.bLastNear = sBike.bNearLight;
    
		if ( ucLastSpeed && sBike.ucSpeed == 0 ){
			TaskFlag = TASK_EXIT;
		}
        
		if ( sBike.ucSpeed )
			uiPreTick = Get_SysTick();

        sBike.ucSpeed += cSpeedInc;
		ucLastSpeed = sBike.ucSpeed;
		break;
	case TASK_EXIT:
	default:
		sBike.bSpeedFlash = 0;
		break;
	}
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
/*----------------------------------------------------------*/

void Light_Task(void)
{
	uint8_t ucSpeedMode;

	if( GPIO_Read(NearLight_PORT, 	NearLight_PIN) ) sBike.bNearLight = 1; else sBike.bNearLight = 0;
	//if( GPIO_Read(TurnRight_PORT, TurnRight_PIN) ) sBike.bTurnRight = 1; else sBike.bTurnRight = 0;
	//if( GPIO_Read(TurnLeft_PORT, 	TurnLeft_PIN ) ) sBike.bTurnLeft  = 1; else sBike.bTurnLeft  = 0;
	//if( GPIO_Read(Braked_PORT, 	Braked_PIN	 ) ) sBike.bBraked    = 1; else sBike.bBraked  	 = 0;
	
	if ( sBike.bYXTERR ){
		ucSpeedMode = 0;
		if( GPIO_Read(SPMODE1_PORT,SPMODE1_PIN) ) ucSpeedMode |= 1<<0;
		if( GPIO_Read(SPMODE2_PORT,SPMODE2_PIN) ) ucSpeedMode |= 1<<1;
		if( GPIO_Read(SPMODE3_PORT,SPMODE3_PIN) ) ucSpeedMode |= 1<<2;
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
		sBike.ucPHA_Speed= GetSpeed();
		sBike.ucSpeed 	 = (uint32_t)sBike.ucPHA_Speed*1000UL/sConfig.uiSpeedScale;
	}
}

void GetSysVoltage(void)
{	
#if defined BENLING_OUSHANG
	uint16_t uiVol;
	for(i=0;i<0xFF;i++){
		if ( GetVolStabed(&uiVol) && (uiVol > 120) ) break;
		FEED_DOG();  
	}
	if ( 720 <= uiVol && uiVol <= 870 ){
		if ( sConfig.uiSysVoltage != 72 ){
			sConfig.uiSysVoltage = 72
			WriteConfig();
		}
	} else if ( 480 <= uiVol && uiVol <= 600 ){
		if ( sConfig.uiSysVoltage != 60 ){
			sConfig.uiSysVoltage = 60;
			WriteConfig();
		}
	}
#elif defined BENLING_BL48_60
	uint16_t uiVol;
	for(i=0;i<0xFF;i++){
		if ( GetVolStabed(&uiVol) && (uiVol > 120) ) break;
		FEED_DOG();  
	}
	if ( 610 <= uiVol && uiVol <= 720 ){
		if ( sConfig.uiSysVoltage != 60 ){
			sConfig.uiSysVoltage = 60;
			WriteConfig();
		}
	}	else if ( 360 <= uiVol && uiVol <= 500 ){
		if ( sConfig.uiSysVoltage != 48 ){
			sConfig.uiSysVoltage = 48;
			WriteConfig();
		}
	}		
#elif defined BENLING_ZHONGSHA
	sConfig.uiSysVoltage = 72;
#elif (defined OUJUN) || (defined OUPAINONG_6072)
	//GPIO_Init(V72_PORT, V72_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(V48_PORT, V48_PIN, GPIO_MODE_IN_PU_NO_IT);
	if ( GPIO_ReadInputPin(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined OUPAINONG_4860
	GPIO_Init(V48_PORT, V48_PIN, GPIO_MODE_IN_PU_NO_IT);
	if ( GPIO_ReadInputPin(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 48;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined LCD9040_4860
	GPIO_Init(V48_PORT, V48_PIN, GPIO_MODE_IN_PU_NO_IT);
	if ( GPIO_ReadInputPin(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 60;
	} else {
		sConfig.uiSysVoltage = 48;
	}
#else
	GPIO_Init(V72_PORT, V72_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(V48_PORT, V48_PIN, GPIO_MODE_IN_PU_NO_IT);
	if ( GPIO_ReadInputPin(V72_PORT, V72_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		if ( GPIO_ReadInputPin(V48_PORT, V48_PIN) == RESET ){
			sConfig.uiSysVoltage = 48;
		} else {
			sConfig.uiSysVoltage = 60;
		}
	}
#endif
}

void Calibration(void)
{
	uint8_t i;
	uint16_t uiVol;
	
	CFG->GCR = CFG_GCR_SWD;
	//¶Ì½ÓµÍËÙ¡¢SWIMÐÅºÅ
	GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW);

	for(i=0;i<32;i++){
		GPIO_WriteLow (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE1_PORT	, SPMODE1_PIN) ) break;
		GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE1_PORT	, SPMODE1_PIN)  == RESET ) break;
	}
	if ( i == 32 ){
		for(i=0;i<0xFF;i++){
			if ( GetVolStabed(&uiVol) && (uiVol > 120) ) break;
			FEED_DOG();  
		}
		sBike.uiVoltage		= uiVol;
		//sBike.siTemperature= GetTemp();
		//sBike.ucSpeed		= GetSpeed();

		sConfig.uiVolScale	= (uint32_t)sBike.uiVoltage*1000UL/VOL_CALIBRATIOIN;		//60.00V
		//sConfig.TempScale	= (long)sBike.siTemperature*1000UL/TEMP_CALIBRATIOIN;	//25.0C
		//sConfig.uiSpeedScale = (uint32_t)sBike.ucSpeed*1000UL/SPEED_CALIBRATIOIN;	//30km/h
		//sConfig.ulMile = 0;
		WriteConfig();
	}
	
#if ( TIME_ENABLE == 1 )
	for(i=0;i<32;i++){
		GPIO_WriteLow (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN) ) break;
		GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN)  == RESET ) break;
	}
	if ( i == 32 ){
		sBike.bUart = 1;
	} else
		sBike.bUart = 0;
#endif

	CFG->GCR &= ~CFG_GCR_SWD;
}

void main(void)
{
	uint8_t i;
	uint16_t uiTick;
	uint16_t uiCount=0;
	uint16_t uiVol=0;
	uint16_t tick_100ms=0;
	
	/* select Clock = 8 MHz */
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV2);
	CLK_HSICmd(ENABLE);
	IWDG_Config();

	Init_timer();  
	HotReset();
	if ( sBike.bHotReset == 0 ) {
		BL55072_Config(1);
	} else
		BL55072_Config(0);

//	for(i=0;i<32;i++){	GetVol();	/*FEED_DOG(); */ }
//	for(i=0;i<16;i++){	GetSpeed();	/*FEED_DOG(); */ }
	for(i=0;i<4;i++) {	GetTemp();	FEED_DOG(); }

	InitConfig();
	Calibration();
	
#if ( TIME_ENABLE == 1 )	
	//sBike.bHasTimer = !PCF8563_Check();
	sBike.bHasTimer = PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	#ifndef DENGGUAN_XUNYING_T
	InitUART();
	#endif
#endif

#if ( YXT_ENABLE == 1 )
	YXT_Init();  
#endif
  
	enableInterrupts();
	
	if ( sBike.bHotReset == 0 ) {
		while ( Get_SysTick() < PON_ALLON_TIME ) FEED_DOG();
		BL55072_Config(0);
	}
	
	GetVolStabed(&uiVol);
	sBike.uiVoltage = (uint32_t)uiVol*1000UL/sConfig.uiVolScale;
	sBike.siTemperature = GetTemp();
	
	while(1){
		uiTick = Get_SysTick();
		
		if ( (uiTick >= tick_100ms && (uiTick - tick_100ms) >= 100 ) || \
			 (uiTick <  tick_100ms && (0xFFFF - tick_100ms + uiTick) >= 100 ) ) {
			tick_100ms = uiTick;
			uiCount ++;
			
			if ( (uiCount % 5) == 0 ) {
				if ( GetVolStabed(&uiVol) ){
					sBike.uiVoltage = (uint32_t)uiVol*1000UL/sConfig.uiVolScale;
					sBike.ucBatStatus= GetBatStatus(sBike.uiVoltage);
				}
			}
			if ( (uiCount % 10) == 0 ){
				//	sBike.siTemperature= (long)GetTemp()	*1000UL/sConfig.TempScale;
					sBike.siTemperature= GetTemp();
				}
		#ifdef LCD8794GCT
			//sBike.ucEnergy 	= GetBatEnergy(sBike.uiVoltage);
		#endif
		
			Light_Task();
			MileTask(); 
			
		#if ( YXT_ENABLE == 1 )
			YXT_Task(&sBike,&sConfig);  
		#endif
			
			SpeedCaltTask();
		
		#if ( TIME_ENABLE == 1 )	
			TimeTask();   
		#endif
      
		#ifdef LCD_SEG_TEST
			if ( ++uiCount >= 100 ) uiCount = 0;
			sBike.uiVoltage 	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL;
			sBike.siTemperature	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL;
			sBike.ucSpeed		= uiCount/10 + uiCount/10*10;
			sBike.ulMile		= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL + uiCount/10*10000UL;
			sBike.ucHour       	= uiCount/10 + uiCount/10*10;
			sBike.ucMinute     	= uiCount/10 + uiCount/10*10;
			#ifdef LCD8794GCT
			//sBike.ucEnergy    = uiCount/10 + uiCount/10*10UL;
			#endif
		#endif
	
			MenuUpdate(&sBike);
			
			/* Reload IWDG counter */
			FEED_DOG();  
		} 

	#if ( TIME_ENABLE == 1 )
		#ifndef DENGGUAN_XUNYING_T
		UartTask();
		#endif
	#endif
	}
}


/************************ (C) COPYRIGHT  *****END OF FILE****/
