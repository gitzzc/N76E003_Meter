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
//  File Function: N76E003 APROM program DATAFLASH (APROM) demo code
//***********************************************************************************************************
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

//***********************************************************************************************************
/*
	Since the DATAFLASH is in the APROM. Program command is same as program APROM
*/
#define     PAGE_ERASE_AP       0x22
#define     BYTE_READ_AP        0x00
#define     BYTE_PROGRAM_AP     0x21
#define     PAGE_SIZE           128

#define     ERASE_FAIL          0x70
#define     PROGRAM_FAIL        0x71
#define     IAPFF_FAIL          0x72
#define     IAP_PASS            0x00

//---------------------------------------------------------------
// Following define by customer
// Please confirm the start addresss not over your code size
//---------------------------------------------------------------
#define     DATA_SIZE           256   
#define     DATA_START_ADDR     0x3700 					

volatile unsigned char code Data_Flash[DATA_SIZE] _at_ DATA_START_ADDR;

/********************************************************************************************
 Following IAP command register is also define in SFR_Macro.h
 
	#define set_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON |= SET_BIT0 ;EA=BIT_TMP
	#define clr_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON &= ~SET_BIT0;EA=BIT_TMP
	#define set_APUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN |= SET_BIT0 ;EA=BIT_TMP
	#define clr_APUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN &= ~SET_BIT0;EA=BIT_TMP
	
**********************************************************************************************/
void IAP_ERROR_LED(void)
{
	while (1)
	{
		LED_OFF();
		Timer0_Delay1ms(100);
		LED_ON();
		Timer0_Delay1ms(100);
	}

}

//-----------------------------------------------------------------------------------------------------------/
void Trigger_IAP(void)
{   
    set_IAPGO;									//trigger IAP
    if((CHPCON&SET_BIT6)==SET_BIT6)             // if fail flag is set, toggle error LED and IAP stop
	{
		clr_IAPFF;
		IAP_ERROR_LED();
	}
}
/*
		WARNING:
	No matter read or writer, when IAPFF is set 1, 
	this step process is fail. DATA should be ignore.
*/
//-----------------------------------------------------------------------------------------------------------/

/*****************************************************************************************************************
Erase APROM subroutine:
	

******************************************************************************************************************/		
void Erase_APROM(void)
{   
    UINT16 u16Count;

    set_IAPEN;									// Enable IAP function
	IAPFD = 0xFF;								// IMPORTANT !! To erase function must setting IAPFD = 0xFF 
    IAPCN = PAGE_ERASE_AP;
    set_APUEN;									//  APROM modify Enable
	
    for(u16Count=0x0000;u16Count<DATA_SIZE/PAGE_SIZE;u16Count++)		//
    {        
        IAPAL = LOBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        IAPAH = HIBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        Trigger_IAP(); 
    } 
    clr_APUEN;
    clr_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
UINT8 Erase_APROM_Verify(void)
{   
    UINT16 u16Count;
    set_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_AP;

    for(u16Count=0;u16Count<DATA_SIZE;u16Count++)
    {   
        IAPFD = 0x00;    
        Trigger_IAP();
        if(IAPFD != 0xFF)
			return 0;
			//IAP_ERROR_LED();
        IAPAL++;
        if(IAPAL == 0x00)
			IAPAH++;
    } 
		
    clr_IAPEN;
	return 1;
}
//-----------------------------------------------------------------------------------------------------------
UINT8 Program_APROM(UINT8 buf,UINT16 len)
{   
    UINT16 u16Count;
	
	if ( len > DATA_SIZE )
		return 0;

    set_IAPEN;
    set_APUEN;    
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_PROGRAM_AP;
    
    for(u16Count=0;u16Count<len;u16Count++)
    {   
        IAPFD = buf[u16Count];     
        Trigger_IAP();
       
        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
    } 
		
    clr_APUEN;
    clr_IAPEN;
	return 1;
}
//-----------------------------------------------------------------------------------------------------------
UINT8 Program_APROM_Verify(UINT8 buf,UINT16 len)
{   
    UINT16 u16Count;
	
	if ( len > DATA_SIZE ) 
		return 0;

    set_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_AP;

    for(u16Count=0;u16Count<DATA_SIZE;u16Count++)
    {   
        Trigger_IAP();
        if(IAPFD != buf[u16Count])
			IAP_ERROR_LED();
        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
    } 

    clr_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
UINT8 FlashWrite(UINT8* cfg,UINT16 len)
{
	Erase_APROM();
	if ( Erase_APROM_Verify() == 0 ) return 0;
	if ( Program_APROM((UINT8*)cfg,16) == 0 ) return 0;
	if ( Program_APROM_Verify((UINT8*),16) == 0 ) return 0;
	return 1;
}

UINT8 FlashRead(UINT8* cfg,UINT16 len)
{
	if ( len > DATA_SIZE ) 
		return 0;
	
	for(i=0;i<len;i++)
		cfg[i] = Data_Flash[i]
	
	return 1;
}

#if 0
//-----------------------------------------------------------------------------------------------------------
void main (void) 
{

	UINT8 str[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	
	Set_All_GPIO_Quasi_Mode;
//---------toggle GPIO1---------	
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
//---------end toggle GPIO1---------
	
    Erase_APROM();
	Erase_APROM_Verify();
    Program_APROM(str,16);
    Program_APROM_Verify(str,16);

//---------toggle GPIO1---------	
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
//---------end toggle GPIO1---------

    while(1);
}
#endif
//-----------------------------------------------------------------------------------------------------------
