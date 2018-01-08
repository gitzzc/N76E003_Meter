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
#include "TM16XX.h"
#include "display.h"
#include "bike.h"
#include "YXT.h"

/*----------------------------------------------------------*/
const uint16_t uiBatStatus48[] = BAT_STATUS_48V;
const uint16_t uiBatStatus60[] = BAT_STATUS_60V;
const uint16_t uiBatStatus72[] = BAT_STATUS_72V;

uint16_t uiSpeedBuf[16];
uint16_t uiVolBuf[16];
#ifdef BATV_ADC_CH2
uint16_t uiVol2Buf[8];
#endif
uint16_t uiTempBuf[8];

#if ( TIME_ENABLE == 1 )
uint8_t ucUart1Buf[16];
uint8_t ucUart1Index=0;
#endif

/*----------------------------------------------------------*/
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
	if ( pin > 7 ) return;
	switch ( mode ){
	case GPIO_QUASI:
		switch(port){
			case 0x80:	P0M1&=~(1<<pin);P0M2&=~(1<<pin);break;
			case 0x90:	P1M1&=~(1<<pin);P1M2&=~(1<<pin);break;
			case 0xB0:	if (pin == 0) 
						P3M1&=~(0x01);	P3M2&=~(0x01);break;
			default:	break;
		}
		break;
	case GPIO_PUSH_PULL:
		switch(port){
			case 0x80:	P0M1&=~(1<<pin);P0M2|=(1<<pin);break;
			case 0x90:	P1M1&=~(1<<pin);P1M2|=(1<<pin);break;
			case 0xB0:	if (pin == 0) 
						P3M1&=~(0x01);	P3M2|=(0x01);break;
			default:	break;
		}
		break;
	case GPIO_INPUT_ONLY:
		switch(port){
			case 0x80:	P0M1|=(1<<pin);P0M2&=~(1<<pin);break;
			case 0x90:	P1M1|=(1<<pin);P1M2&=~(1<<pin);break;
			case 0xB0:	if (pin == 0) 
						P3M1|=(0x01);	P3M2&=~(0x01);break;
			default:	break;
		}
		break;
	case GPIO_OPEN_DRAIN:
		switch(port){
			case 0x80:	P0M1|=(1<<pin);P0M2|=(1<<pin);break;
			case 0x90:	P1M1|=(1<<pin);P1M2|=(1<<pin);break;
			case 0xB0:	if (pin == 0) 
						P3M1|=(0x01);	P3M2|=(0x01);break;
			default:	break;
		}
		break;
	default : break;
	}	
}

#ifdef SPEED_HALL_PORT
 void SpeedHallInit(void)
{
	GPIO_Init(SPEED_HALL_PORT, SPEED_HALL_PIN, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(SPEED_EXTI_PORT,EXTI_SENSITIVITY_RISE_ONLY);
}
#endif

uint8_t GPIO_Read(uint8_t port, uint8_t pin)
{
	GPIO_Init(port,pin,GPIO_INPUT_ONLY);
	return (port>>pin)&0x01;
}

int GetTemp(void)
{
	static uint8_t ucIndex = 0;
	int32_t slTemp;
	uint16_t sample;

	uiTempBuf[ucIndex++] = uiADC_SampleBuf[0][0];

	if ( ucIndex >= ContainOf(uiTempBuf) )
		ucIndex = 0;
	exchange_sort16(uiTempBuf,ContainOf(uiTempBuf));
	sample = get_average16(uiTempBuf+4,ContainOf(uiTempBuf)-4);

	//slTemp = 10000*1024UL/(1024-sample)-10000;
	slTemp = 10000UL*(int32_t)sample/(1024-sample);
	slTemp = NTCtoTemp(slTemp);
	if ( slTemp > 999  ) slTemp =  999;
	if ( slTemp < -999 ) slTemp = -999;
	
	return slTemp;
}

uint16_t GetVol(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiVol;
	uint16_t uiBuf[ContainOf(uiVolBuf)];
	uint8_t i;

	uiVolBuf[ucIndex++] = uiADC_SampleBuf[0][0];

	if ( ucIndex >= ContainOf(uiVolBuf) )
		ucIndex = 0;

	for(i=0;i<ContainOf(uiVolBuf);i++)
		uiBuf[i] = uiVolBuf[i];
	exchange_sort16(uiBuf,ContainOf(uiBuf));
	for(i=0;i<ContainOf(uiBuf);i++){
		if ( uiBuf[i] > 100 )	break;
	}
	uiVol = 0;
	if ( i + 3*2 < ContainOf(uiBuf) )
		uiVol = get_average16(uiBuf+i+3,ContainOf(uiBuf)-i-3*2);

//	uiVol = (uint32_t)uiVol / 1024UL / 10 * (200+10) * 5 * 10;	// 200k+10k
	uiVol = VOL_CALC(uiVol);
	
	return uiVol;
}

#ifdef BATV_ADC_CH2
uint16_t GetVol2(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiBuf[ContainOf(uiVol2Buf)];
	uint16_t uiVol;
	uint8_t i;

	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);  //B+  
	ADC1_DeInit();  
	ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, BATV_ADC_CH2, ADC1_PRESSEL_FCPU_D2, \
				ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, BATV_ADC_SCH2,\
				DISABLE);
	ADC1_Cmd(ENABLE);
	for(i=0;i<ContainOf(uiBuf);i++){
		Delay(500);  
		ADC1_StartConversion(); 
		while ( ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET );  
		uiBuf[i] = ADC1_GetConversionValue();
	}
	ADC1_Cmd(DISABLE);

	uiVol2Buf[ucIndex++] = get_average16(uiBuf,ContainOf(uiBuf));
	if ( ucIndex >= ContainOf(uiVol2Buf) )
		ucIndex = 0;
	
	for(i=0;i<ContainOf(uiVol2Buf);i++)
		uiBuf[i] = uiVol2Buf[i];
	exchange_sort16(uiBuf,ContainOf(uiBuf));
	for(i=0;i<ContainOf(uiBuf);i++){
		if ( uiBuf[i] > 100 )	break;
	}
	uiVol = 0;
	if ( i + 3*2 < ContainOf(uiBuf) )
		uiVol = get_average16(uiBuf+i+3,ContainOf(uiBuf)-i-3*2);
	
//	uiVol = (uint32_t)uiVol / 1024UL / 10 * (200+10) * 5 * 10;	// 200k+10k
	uiVol = VOL_CALC(uiVol);
	
	return uiVol;
}
#endif

#if ( PCB_VER == 0013 )
uint8_t GetSpeedAdj(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiAdj;
	uint8_t i;

	uiSpeedBuf[ucIndex++] = uiADC_SampleBuf[0][0];

	if ( ucIndex >= ContainOf(uiSpeedBuf) )
		ucIndex = 0;

	uiAdj = get_average16(uiSpeedBuf,ContainOf(uiSpeedBuf));

	if ( uiAdj > 99 )
		uiAdj = 99;
	
  return uiAdj;
}
#endif

uint8_t GetSpeed(void)
{
	static uint8_t ucIndex = 0;
	uint16_t uiSpeed;

#ifdef SPEEDV_ADC_CH
	uiSpeedBuf[ucIndex++] = uiADC_SampleBuf[0][0];

	if ( sConfig.uiSysVoltage		== 48 ){
		uiSpeed = SPEED_CALC_48V((uint32_t)uiSpeed);
	} else if ( sConfig.uiSysVoltage== 60 ) {
		uiSpeed = SPEED_CALC_60V((uint32_t)uiSpeed);
	} else if ( sConfig.uiSysVoltage== 72 )	{
		uiSpeed = SPEED_CALC_72V((uint32_t)uiSpeed);
	} else
		uiSpeed = 0;
#else	
	uiSpeed = GetSpeedHall();
#endif

	uiSpeedBuf[ucIndex++] = uiSpeed;
	if ( ucIndex >= ContainOf(uiSpeedBuf) )
		ucIndex = 0;

	uiSpeed = get_average16(uiSpeedBuf,ContainOf(uiSpeedBuf));
	if ( uiSpeed > 99 )
		uiSpeed = 99;
	
  return uiSpeed;
}

void GetSysVoltage(void)
{	
#if defined BENLING_OUSHANG
	uint16_t uiVol,i;
	for(i=0;i<0xFF;i++){
		uiVol = GetVolStabed();
		if ( uiVol > 120 ) break;
		FEED_DOG();  
	}
	if ( 720 <= uiVol && uiVol <= 870 ){
		sConfig.uiSysVoltage = 72;
		WriteConfig();
	} else if ( 480 <= uiVol && uiVol <= 600 ){
		sConfig.uiSysVoltage = 60;
		WriteConfig();
	}
#elif defined VD61723650
	uint16_t uiVol,i;
	for(i=0;i<0xFF;i++){
		uiVol = GetVolStabed();
		if ( uiVol > 120 ) break;
		FEED_DOG();  
	}
	if ( 610 <= uiVol && uiVol <= 720 ){
		sConfig.uiSysVoltage = 60;
		WriteConfig();
	}	else if ( 360 <= uiVol && uiVol <= 500 ){
		sConfig.uiSysVoltage = 48;
		WriteConfig();
	}		
#elif defined VD48
	sConfig.uiSysVoltage = 48;
#elif defined VD60
	sConfig.uiSysVoltage = 60;
#elif defined VD72
	sConfig.uiSysVoltage = 72;
#elif defined VD48N72L
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined VD48L72N
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 48;
	} else {
		sConfig.uiSysVoltage = 60;
	}
#elif defined VD48H72N
	if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
		sConfig.uiSysVoltage = 60;
	} else {
		sConfig.uiSysVoltage = 48;
	}
#elif defined VD72L48L
	if ( GPIO_Read(V72_PORT, V72_PIN) == RESET ){
		sConfig.uiSysVoltage = 72;
	} else {
		if ( GPIO_Read(V48_PORT, V48_PIN) == RESET ){
			sConfig.uiSysVoltage = 48;
		} else {
			sConfig.uiSysVoltage = 60;
		}
	}
#else
	#error "Please select a system voltage detection method!!!"
#endif
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
	uint16_t uiVol,uiVol2,i;
	
	if ( sBike.bUart == 0 )
		return ;
	
	if ( ucUart1Index > 0 && ucUart1Buf[ucUart1Index-1] == '\n' ){
		if ( ucUart1Index >= 11 && ucUart1Buf[0] == 'T' /*&& ucUart1Buf[1] == 'i' && ucUart1Buf[2] == 'm' && ucUart1Buf[3] == 'e' */) {
			RtcTime.RTC_Hours 	= (ucUart1Buf[5]-'0')*10 + (ucUart1Buf[6] - '0');
			RtcTime.RTC_Minutes = (ucUart1Buf[8]-'0')*10 + (ucUart1Buf[9] - '0');
			PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		} else if ( ucUart1Index >= 5 && ucUart1Buf[0] == 'C' /*&& ucUart1Buf[1] == 'a' && ucUart1Buf[2] == 'l' && ucUart1Buf[3] == 'i' */){
			for(i=0;i<0xFF;i++){
				uiVol = GetVol();
				if ( uiVol > 120 ) break;
				FEED_DOG();  
			}
			sBike.uiBatVoltage	 	= uiVol;
			sBike.siTemperature = GetTemp();
			sBike.ucSpeed		= GetSpeed();

			sConfig.uiVolScale	= (uint32_t)sBike.uiBatVoltage*1000UL/VOL_CALIBRATIOIN;					
		//	sConfig.TempScale 	= (long)sBike.siTemperature*1000UL/TEMP_CALIBRATIOIN;	
		//	sConfig.uiSpeedScale= (uint32_t)sBike.ucSpeed*1000UL/SPEED_CALIBRATIOIN;				
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

    for(i=0;i<64;i++){
	//	GPIO_WriteLow  (GPIOD,GPIO_PIN_1); FEED_DOG(); Delay(10000);
		if( GPIO_Read(CALI_PORT	, CALI_PIN) ) break;
	//	GPIO_WriteHigh (GPIOD,GPIO_PIN_1); FEED_DOG(); Delay(10000);
		if( GPIO_Read(CALI_PORT	, CALI_PIN)  == RESET ) break;
	}
	
	if ( i == 64 ){
		for(i=0;i<64;i++){ GetVol();	FEED_DOG();  }
		sBike.uiBatVoltage	= GetVol();
		sConfig.uiVolScale	= (uint32_t)sBike.uiBatVoltage*1000UL /VOL_CALIBRATIOIN;	//60.00V

	#if ( PCB_VER == 9737 )
		for(i=0;i<64;i++){ GetVol2();	FEED_DOG();  }
		sBike.uiBatVoltage2	= GetVol2();
		sConfig.uiVolScale2	= (uint32_t)sBike.uiBatVoltage2*1000UL /VOL_CALIBRATIOIN;	//60.00V
	#endif
        
	//	sBike.siTemperature	= GetTemp();
	//	sConfig.TempScale	= (uint32_t)sBike.siTemperature*1000UL/TEMP_CALIBRATIOIN;	//25.0C

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
#define TIMER0_TH0_10MS	0x34
#define TIMER0_TL0_10MS	0x12

void Timer0_Init(void)
{
	//TMOD = 0XFF;
	clr_TR0;
	TIMER0_MODE0_ENABLE;                        //Timer 0 and Timer 1 mode configuration
	clr_T0M;
    
	TH0 = TIMER0_TH0_10MS;
	TL0 = TIMER0_TL0_10MS;
    
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
    TH0 = TIMER0_TH0_10MS;
    TL0 = TIMER0_TL0_10MS; 
	set_TR0;	
	
	INC_SYS_TICK();
	LRFlashTask();	//10ms
	
	ADC_TRIG();
}

void main(void)
{
	uint8_t i;
	uint16_t uiPreTick=0;
	uint16_t uiCount=0;
	uint16_t uiVol=0,uiVol2=0;	
	WDG_Config();

#ifdef RESET_CONFIG
	ResetConfig();
	while(1){ 
		if ( Get_ElapseTick(uiPreTick) > 1000 ){
			uiPreTick = Get_SysTick();
			if ( i ){ i = 0; DisplayInit(i); } 
			else 	{ i = 1; DisplayInit(i); }
			Display(&sBike);
		}
		FEED_DOG(); 
	}
#else
	Timer0_Init();  
	HotReset();
    //sBike.bHotReset = 0;
	if ( sBike.bHotReset == 0 ) {
		DisplayInit(1);
	} else
		DisplayInit(0);

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
  
#ifdef SPEED_HALL_PORT
	SpeedHallInit();
#endif
  
	ENABLE_INTERRUPTS();
	
	if ( sBike.bHotReset == 0 ) {
		while ( Get_SysTick() < PON_ALLON_TIME ) FEED_DOG();
		DisplayInit(0);
	}
	
#if ( PCB_VER == 9737 )
	for(i=0;i<ContainOf(uiVolBuf )/2;i++) { GetVol();  FEED_DOG(); }
	for(i=0;i<ContainOf(uiVol2Buf)/2;i++) { GetVol2(); FEED_DOG(); }
#else
	for(i=0;i<ContainOf(uiVolBuf );	 i++) { GetVol(); FEED_DOG(); }
#endif
//	for(i=0;i<ContainOf(uiSpeedBuf); i++) { GetSpeed();FEED_DOG(); }
	for(i=0;i<ContainOf(uiTempBuf);  i++) { GetTemp(); FEED_DOG(); }
	
	while(1){
		if ( Get_ElapseTick(uiPreTick) >= 100 ){
			uiPreTick = Get_SysTick();
			
			if ( (uiCount % 2) == 0 ) 
			{
				uiVol  = GetVol();
				sBike.uiBatVoltage  = (uint32_t)(uiVol +0)*1000UL/sConfig.uiVolScale;
			#if ( PCB_VER == 9737 )
				uiVol2 = GetVol2();
				sBike.uiBatVoltage2 = (uint32_t)(uiVol2+0)*1000UL/sConfig.uiVolScale2;
			#endif
			}
			if ( (uiCount % 10) == 0 ){
			//	sBike.siTemperature= (long)GetTemp()	*1000UL/sConfig.TempScale;
				sBike.siTemperature= GetTemp();
			}
			uiCount ++;
			
			LightTask();
			MileTask(); 
			
		#if ( YXT_ENABLE == 1 )
			YXT_Task(&sBike,&sConfig);  
		#endif
		
			SpeedCaltTask();
		
		#if ( TIME_ENABLE == 1 )	
			TimeTask();   
		#endif
      
		#ifdef LCD_SEG_TEST
			if ( uiCount >= 100 ) uiCount = 0;
			sBike.uiBatVoltage 	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL;
			sBike.siTemperature	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL;
			sBike.ucSpeed		= uiCount/10 + uiCount/10*10;
			sBike.ulMile		= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL + uiCount/10*10000UL;
			sBike.ucHour       	= uiCount/10 + uiCount/10*10;
			sBike.ucMinute     	= uiCount/10 + uiCount/10*10;
		#endif
	
			Display(&sBike);
			
			/* Reload IWDG counter */
			FEED_DOG();  
		} 

	#if ( TIME_ENABLE == 1 )
		#ifndef DENGGUAN_XUNYING_T
		UartTask();
		#endif
	#endif
	}
#endif	
}


