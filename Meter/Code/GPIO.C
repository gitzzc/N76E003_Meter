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


/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
static void WDG_Config(void)
{
}

//enum GPIO_PORT = {GPIO0,GPIO1,GPIO2,GPIO3};
//enum GPIO_PIN  = {PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7};

void GPIO_Init(uint8_t port, uint8_t pin, uint8_t mode)
{
	switch ( mode ){
		case GPIO_QUASI:
			switch(port){
				case 0x80:	P0M1&=~(1<<pin);P0M2&=~(1<<pin);break;
				case 0x90:	P1M1&=~(1<<pin);P1M2&=~(1<<pin);break;
				case 0xB0:	P3M1&=~(1<<pin);P3M2&=~(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_PUSH_PULL:
			switch(port){
				case 0x80:	P0M1&=~(1<<pin);P0M2|=(1<<pin);break;
				case 0x90:	P1M1&=~(1<<pin);P1M2|=(1<<pin);break;
				case 0xB0:	P3M1&=~(1<<pin);P3M2|=(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_INPUT_ONLY:
			switch(port){
				case 0x80:	P0M1|=(1<<pin);P0M2&=~(1<<pin);break;
				case 0x90:	P1M1|=(1<<pin);P1M2&=~(1<<pin);break;
				case 0xB0:	P3M1|=(1<<pin);P3M2&=~(1<<pin);break;
				default:	break;
			}
			break;
		case GPIO_OPEN_DRAIN:
			switch(port){
				case 0x80:	P0M1|=(1<<pin);P0M2|=(1<<pin);break;
				case 0x90:	P1M1|=(1<<pin);P1M2|=(1<<pin);break;
				case 0xB0:	P3M1|=(1<<pin);P3M2|=(1<<pin);break;
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
/*	if ( sBike.bHotReset == 0 ) {
		BL55072_Config(1);
	} else
		BL55072_Config(0);
*/
	ADC_Init();
//	for(i=0;i<32;i++){	GetVol();	/*FEED_DOG(); */ }
//	for(i=0;i<16;i++){	GetSpeed();	/*FEED_DOG(); */ }
//	for(i=0;i<4;i++) {	GetTemp();	FEED_DOG(); }

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
	//YXT_Init();  
#endif
  
	ENABLE_INTERRUPTS();
	
	if ( sBike.bHotReset == 0 ) {
		while ( Get_SysTick() < PON_ALLON_TIME ) FEED_DOG();
		//BL55072_Config(0);
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
			//YXT_Task(&sBike,&sConfig);  
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
	
			//MenuUpdate(&sBike);
			
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



