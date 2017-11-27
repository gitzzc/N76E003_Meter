#ifndef __ADC_H__
#define __ADC_H__

#define ADC_GET_VDD(bgvalue) 0x1000*Bandgap_Voltage/bgvalue
#define ADC_TRIG()	set_ADCS

#define ADC_BUF_LEN	32

extern uint8_t xdata ucADC_ChannelBuf[];
extern uint16_t xdata uiADC_SampleBuf[2][ADC_BUF_LEN];
extern uint16_t uiBandgap_Voltage,uiVDD_Voltage;
		
void READ_BANDGAP();
void ADC_Init(void);

#endif
