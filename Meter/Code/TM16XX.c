#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

#include "bike.h"
#include "TM16XX.h"

#ifdef TM16XX_PORT

/******************************************************************************/

#ifdef TM16XX_CS	
#define CS_SET()	do{TM16XX_PORT->ODR  |= TM16XX_CS;} while(0)
#define CS_CLR()	do{TM16XX_PORT->ODR  &=~TM16XX_CS;} while(0)
#endif

#define CLK_SET()	do{TM16XX_PORT->ODR  |= TM16XX_CLK;} while(0)
#define CLK_CLR()	do{TM16XX_PORT->ODR  &=~TM16XX_CLK;} while(0)
#define DAT_SET()	do{TM16XX_PORT->ODR  |= TM16XX_DAT;} while(0)
#define DAT_CLR()	do{TM16XX_PORT->ODR  &=~TM16XX_DAT;} while(0)

/******************************************************************************/

#define CONFIG_CMD	0x40
#define ADDR_AUTO	0x00
#define ADDR_CONST	0x04

#define DISPLAY_CMD	0x80
#define DISPLAY_ON	0x08
#define DISPLAY_OFF	0x00
#define DISPLAY_4	0x0A
#define DISPLAY_12	0x0D
#define DISPLAY_13	0x0E
#define DISPLAY_14	0x0F

#define ADDRESS_CMD	0xC0

/******************************************************************************/

unsigned char TM16XX[16];

/******************************************************************************/

void TM16XX_Init(unsigned char st)
{
	unsigned char i,dat;
	
#ifdef TM1624	
	GPIO_Init(TM16XX_PORT, TM16XX_CS , GPIO_MODE_OUT_PP_HIGH_FAST);
#endif
	GPIO_Init(TM16XX_PORT, TM16XX_CLK, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(TM16XX_PORT, TM16XX_DAT, GPIO_MODE_OUT_PP_HIGH_FAST);
	DAT_SET();
    CLK_SET();
	switch(st){
		case 0:	dat = 0x00; break;	
		case 1:	dat = 0xFF; break;	
		default: break;
	}
	for(i=0;i<16;i++)	TM16XX[i] = dat;
#ifdef TM1624	
	TM1624_Write_Data(TM16XX,14);
#elif defined TM1640
	TM1640_Write_Data(TM16XX,16);
#endif
}

#pragma optimize=none
void TM16XX_Delay(unsigned char cnt)
{
	volatile unsigned char i;
	for(i=0;i<cnt;i++);	
}

void TM16XX_WriteReg( unsigned char reg )
{
	unsigned char i;
	for(i=0;i<8;i++) {
		if((reg>>i)&0x01)	DAT_SET();
		else				DAT_CLR();
		TM16XX_Delay(5);
		CLK_SET();TM16XX_Delay(5);
		CLK_CLR();//TM16XX_Delay(2);
	}
}
#ifdef TM1624	
#pragma optimize=none
void TM1624_Write_Data(unsigned char* buf,unsigned char len)
{
	unsigned char i;
	CS_CLR();TM16XX_Delay(5);
	TM16XX_WriteReg(0x03);
	CS_SET();TM16XX_Delay(5);
	
	CS_CLR();TM16XX_Delay(5);
	TM16XX_WriteReg(CONFIG_CMD|ADDR_AUTO);
	CS_SET();TM16XX_Delay(5);
	
	CS_CLR();TM16XX_Delay(5);
	TM16XX_WriteReg(ADDRESS_CMD|0x00);
	
	for(i=0;i<len;i++)
		TM16XX_WriteReg(buf[i]);
	CS_SET();TM16XX_Delay(5);
	
	CS_CLR();TM16XX_Delay(5);
	TM16XX_WriteReg(DISPLAY_CMD|DISPLAY_14);
	CS_SET();TM16XX_Delay(5);
}
#elif defined TM1640
#pragma optimize=none
void TM1640_Write_Data(unsigned char* buf,unsigned char len)
{
	unsigned char i;
	DAT_SET();TM16XX_Delay(5);
	CLK_SET();TM16XX_Delay(5);
	DAT_CLR();TM16XX_Delay(5);
	CLK_CLR();TM16XX_Delay(5);
	TM16XX_WriteReg(CONFIG_CMD|ADDR_AUTO);
	DAT_CLR();TM16XX_Delay(5);
	CLK_SET();TM16XX_Delay(5);
	DAT_SET();TM16XX_Delay(5);
	DAT_CLR();TM16XX_Delay(5);
	CLK_CLR();TM16XX_Delay(5);
		
	TM16XX_WriteReg(ADDRESS_CMD|0x00);
	
	for(i=0;i<len;i++)
		TM16XX_WriteReg(buf[i]);
	
	DAT_CLR();TM16XX_Delay(5);
	CLK_SET();TM16XX_Delay(5);
	DAT_SET();TM16XX_Delay(5);
	DAT_CLR();TM16XX_Delay(5);
	CLK_CLR();TM16XX_Delay(5);

	//TM16XX_WriteReg(DISPLAY_CMD|DISPLAY_14);
	TM16XX_WriteReg(DISPLAY_CMD|0X08);
	DAT_CLR();TM16XX_Delay(5);
	CLK_SET();TM16XX_Delay(5);
	DAT_SET();TM16XX_Delay(5);
}
#endif

#endif
