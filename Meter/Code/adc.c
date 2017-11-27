#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "bike.h"
#include "adc.h"

uint8_t xdata ucADC_ChannelBuf[] = {0x00,0x08};
uint16_t xdata uiADC_SampleBuf[sizeof(ucADC_ChannelBuf)][ADC_BUF_LEN];

uint16_t uiBandgap_Voltage,uiVDD_Voltage;
		
void ADC_Bypass (void)							// First three times band-gap convert result bypass
{
	unsigned char ozc;
	for (ozc=0;ozc<0x03;ozc++)
	{
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0);
	}
}
		
void READ_BANDGAP()								// Read band-gap actually value after UID two byte.
{												// Detail description see datasheet V1.02 band-gap chapter.
	uint8_t  BandgapHigh,BandgapLow;
	uint16_t Bandgap_Value;						// always use double mode define.

	set_IAPEN;
	IAPAL = 0x0C;
	IAPAH = 0x00;
	IAPCN = READ_UID;
	set_IAPGO;
	BandgapHigh = IAPFD;
	
	IAPAL = 0x0d;
	IAPAH = 0x00;
	IAPCN = READ_UID;
	set_IAPGO;
	BandgapLow = IAPFD;
	BandgapLow = BandgapLow&0x0F;
	clr_IAPEN;
	
	Bandgap_Value = ((uint16_t)BandgapHigh<<4)+BandgapLow;
	uiBandgap_Voltage = 3072*Bandgap_Value/0x0fff;
}
			
void ADC_Init(void)
{
	READ_BANDGAP();
	Enable_ADC_BandGap;				//Find in "Function_define.h" - "ADC INIT"
	ADC_Bypass();
	
	Enable_ADC_AIN0;
}

/******************************************************************************
 * FUNCTION_PURPOSE: ADC interrupt Service Routine
 ******************************************************************************/
void ADC_ISR (void) interrupt 11
{
	static uint8_t ucIndex=0,ucChannel=0;
	uint8_t ucAdcrh,ucAdclh;
	
    clr_ADCF;                               //clear ADC interrupt flag
	ucAdcrh = ADCRH;
	ucAdclh = ADCRL;
	
	uiADC_SampleBuf[ucChannel][ucIndex++] = ((uint16_t)ucAdcrh<<8) | ucAdclh;
	if ( ucIndex >= ADC_BUF_LEN ){
		ucIndex = 0;
		if ( ++ucChannel >= ContainOf(ucADC_ChannelBuf) )
			ucChannel = 0;
		ADCCON0 = (ADCCON0&0xF0) | ucADC_ChannelBuf[ucChannel];
	}
}


