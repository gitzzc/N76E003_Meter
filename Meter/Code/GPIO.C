/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 GPIO demo code
//***********************************************************************************************************
#include <stdlib.h>
#include <string.h>

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "adc.h"
#include "bl55072.h"
#include "display.h"
#include "bike.h"
#include "YXT.h"


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
BIKE_CONFIG sConfig;
volatile uint16_t  uiSysTick = 0;
uint16_t uiSpeedBuf[16];
uint16_t uiVolBuf[28];
uint16_t uiTempBuf[4];

#if ( TIME_ENABLE == 1 )
uint8_t ucUart1Buf[16];
uint8_t ucUart1Index=0;
#endif

#if 0

/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
static void IWDG_Config(void)
{
}

void GPIO_Init(uint8_t port, uint8_t pin, uint8_t mode)
{
	switch ( mode ){
		case GPIO_QUASI:
			switch(port){
				case P0:	P0M1&=~(1<<pin);P0M2&=~(1<<pin);break;
				case P1:	P1M1&=~(1<<pin);P1M2&=~(1<<pin);break;
				case P2:	P2M1&=~(1<<pin);P2M2&=~(1<<pin);break;
				case P3:	P3M1&=~(1<<pin);P3M2&=~(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_PUSH_PULL:
			switch(port){
				case P0:	P0M1&=~(1<<pin);P0M2|=(1<<pin);break;
				case P1:	P1M1&=~(1<<pin);P1M2|=(1<<pin);break;
				case P2:	P2M1&=~(1<<pin);P2M2|=(1<<pin);break;
				case P3:	P3M1&=~(1<<pin);P3M2|=(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_INPUT_ONLY:
			switch(port){
				case P0:	P0M1|=(1<<pin);P0M2&=~(1<<pin);break;
				case P1:	P1M1|=(1<<pin);P1M2&=~(1<<pin);break;
				case P2:	P2M1|=(1<<pin);P2M2&=~(1<<pin);break;
				case P3:	P3M1|=(1<<pin);P3M2&=~(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_OPEN_DRAIN:
			switch(port){
				case P0:	P0M1|=(1<<pin);P0M2|=(1<<pin);break;
				case P1:	P1M1|=(1<<pin);P1M2|=(1<<pin);break;
				case P2:	P2M1|=(1<<pin);P2M2|=(1<<pin);break;
				case P3:	P3M1|=(1<<pin);P3M2|=(1<<pin);break;
				default:	break;
			}
			break;
		default : break;
	}				
}

uint8_t GPIO_Read(uint8_t port, uint8_t pin)
{
	GPIO_Init(port,pin,GPIO_INPUT_ONLY);
	return (port>>pin)&0x01;
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
		sConfig.uiSysVoltage = 72;
		WriteConfig();
	} else if ( 480 <= uiVol && uiVol <= 600 ){
		sConfig.uiSysVoltage = 60;
		WriteConfig();
	}
#elif defined BENLING_BL48_60
	uint16_t uiVol;
	for(i=0;i<0xFF;i++){
		if ( GetVolStabed(&uiVol) && (uiVol > 120) ) break;
		FEED_DOG();  
	}
	if ( 610 <= uiVol && uiVol <= 720 ){
		sConfig.uiSysVoltage = 60;
		WriteConfig();
	}	else if ( 360 <= uiVol && uiVol <= 500 ){
		sConfig.uiSysVoltage = 48;
		WriteConfig();
	}		
#elif defined BENLING_ZHONGSHA
	sConfig.uiSysVoltage = 72;
#elif (defined OUJUN) || (defined OUPAINONG_6072)
	//GPIO_Init(V72_PORT, V72_PIN, GPIO_INPUT_ONLY);
	GPIO_Init(V48_PORT, V48_PIN, GPIO_INPUT_ONLY);
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined OUPAINONG_4860 || defined JIKE13050
	GPIO_Init(V48_PORT, V48_PIN, GPIO_INPUT_ONLY);
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 48;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined LCD9040_4860
	GPIO_Init(V48_PORT, V48_PIN, GPIO_INPUT_ONLY);
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 60;
	} else {
		sConfig.uiSysVoltage = 48;
	}
#else
	GPIO_Init(V72_PORT, V72_PIN, GPIO_INPUT_ONLY);
	GPIO_Init(V48_PORT, V48_PIN, GPIO_INPUT_ONLY);
	if ( GPIO_Read(V72_PORT, V72_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
			sConfig.uiSysVoltage = 48;
		} else {
			sConfig.uiSysVoltage = 60;
		}
	}
#endif
}

void Light_Task(void)
{
	uint8_t ucSpeedMode;

	if( GPIO_Read(NearLight_PORT,NearLight_PIN) 	== 1 ) sBike.bNearLight = 1; else sBike.bNearLight = 0;
	//if( GPIO_Read(TurnRight_PORT,TurnRight_PIN) 	== 1 ) sBike.bTurnRight = 1; else sBike.bTurnRight = 0;
	//if( GPIO_Read(TurnLeft_PORT,TurnLeft_PIN) 	== 1 ) sBike.bTurnLeft  = 1; else sBike.bTurnLeft  = 0;
	//if( GPIO_Read(Braked_PORT,Braked_PIN)			== 1 ) sBike.bBraked    = 1; else sBike.bBraked    = 0;
	
	if ( sBike.bYXTERR ){
		ucSpeedMode = 0;
		if( GPIO_Read(SPMODE1_PORT,SPMODE1_PIN) == 1 ) ucSpeedMode |= 1<<0;
		if( GPIO_Read(SPMODE2_PORT,SPMODE2_PIN) == 1 ) ucSpeedMode |= 1<<1;
		if( GPIO_Read(SPMODE3_PORT,SPMODE3_PIN) == 1 ) ucSpeedMode |= 1<<2;
	#ifdef SPMODE4_PORT
		if( GPIO_Read(SPMODE4_PORT,SPMODE4_PIN) == 1 ) ucSpeedMode |= 1<<3;
	#endif
		switch(ucSpeedMode){
			case 0x01: 	sBike.ucSpeedMode = 1; break;
			case 0x02: 	sBike.ucSpeedMode = 2; break;
			case 0x04: 	sBike.ucSpeedMode = 3; break;
			case 0x08: 	sBike.ucSpeedMode = 4; break;
			default:	sBike.ucSpeedMode = 0; break;
		}
		sBike.ucPHA_Speed= GetSpeed();
		sBike.ucSpeed 	= (uint32_t)sBike.ucPHA_Speed*1000UL/sConfig.uiSpeedScale;
	}
}

#if ( TIME_ENABLE == 1 )
void InitUART(void)
{
	if ( sBike.bUart == 0 )
		return ;
	
	/* USART configured as follow:
		- BaudRate = 9600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Odd parity
		- Receive and transmit enabled
		- UART Clock disabled
	*/
	UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_ODD,
				   UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_RX_ENABLE|UART1_MODE_TX_DISABLE);

	/* Enable the UART Receive interrupt: this interrupt is generated when the UART
	receive data register is not empty */
	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

	/* Enable UART */
	UART1_Cmd(ENABLE);
}

void UartTask(void)
{   
	uint16_t uiVol,i;
	
	if ( sBike.bUart == 0 )
		return ;
	
	if ( ucUart1Index > 0 && ucUart1Buf[ucUart1Index-1] == '\n' ){
		if ( ucUart1Index >= 11 && ucUart1Buf[0] == 'T' /*&& ucUart1Buf[1] == 'i' && ucUart1Buf[2] == 'm' && ucUart1Buf[3] == 'e' */) {
			RtcTime.RTC_Hours 	= (ucUart1Buf[5]-'0')*10 + (ucUart1Buf[6] - '0');
			RtcTime.RTC_Minutes = (ucUart1Buf[8]-'0')*10 + (ucUart1Buf[9] - '0');
			PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		} else if ( ucUart1Index >= 5 && ucUart1Buf[0] == 'C' /*&& ucUart1Buf[1] == 'a' && ucUart1Buf[2] == 'l' && ucUart1Buf[3] == 'i' */){
			for(i=0;i<0xFF;i++){
				if ( GetVolStabed(&uiVol) && (uiVol > 120) ) break;
				FEED_DOG();  
			}
			sBike.uiVoltage	 	= uiVol;
			sBike.siTemperature = GetTemp();
			sBike.ucSpeed		= GetSpeed();

			sConfig.uiVolScale	= (uint32_t)sBike.uiVoltage*1000UL/VOL_CALIBRATIOIN;					
		//	sConfig.TempScale 	= (long)sBike.siTemperature*1000UL/TEMP_CALIBRATIOIN;	
			sConfig.uiSpeedScale= (uint32_t)sBike.ucSpeed*1000UL/SPEED_CALIBRATIOIN;				
			WriteConfig();
		}
		ucUart1Index = 0;
	}
}
#endif 

void Calibration(void)
{
	uint8_t i;
	uint16_t uiVol;
	
	//GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW);

	for(i=0;i<32;i++){
		//GPIO_WriteLow (GPIOD,GPIO_PIN_1);
		Timer1_Delay1ms(1000);
		if( GPIO_Read(SPMODE1_PORT	, SPMODE1_PIN) ) break;
		//GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Timer1_Delay1ms(1000);
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
		Timer1_Delay1ms(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN) ) break;
		GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Timer1_Delay1ms(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN)  == RESET ) break;
	}
	if ( i == 32 ){
		sBike.bUart = 1;
	} else
		sBike.bUart = 0;
#endif
}
#endif

#define TIMER0_TH0_1MS	0x05
#define TIMER0_TL0_1MS	0x35
#define INC_SYS_TICK()	uiSysTick++

void Timer0_Init(void)
{
	//TMOD = 0XFF;
	clr_TR0;
	TIMER0_MODE0_ENABLE;                        //Timer 0 and Timer 1 mode configuration
	clr_T0M;
    
	TH0 = TIMER0_TH0_1MS;
	TL0 = TIMER0_TL0_1MS;
    
	set_ET0;                                    //enable Timer0 interrupt
	set_EA;                                     //enable interrupts
	
	set_TR0;                                    //Timer0 run
}

/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
void Timer0_ISR (void) interrupt 1          //interrupt address is 0x000B
{
	clr_TR0;
    TH0 = TIMER0_TH0_1MS;
    TL0 = TIMER0_TL0_1MS; 
	set_TR0;	
	
	uiSysTick++;
	if ( (uiSysTick % 10) == 0)
		LRFlashTask();
	
	ADC_TRIG();
}

void main(void)
{
	uint8_t i;
	uint16_t uiTick;
	uint16_t uiCount=0;
	uint16_t uiVol=0;
	uint16_t tick_100ms=0;
	
	WDG_Config();
	Timer0_Init();  
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
  
	ENABLE_INTERRUPTS();
	
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
			
			if ( (uiCount % 4 ) == 0 ){
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
			//sBike.ucEnergy     	= uiCount/10 + uiCount/10*10UL;
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



