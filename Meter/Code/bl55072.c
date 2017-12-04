#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#include "bl55072.h"
#include "bike.h"

#define I2C_CLOCK               13
#define I2C_WRITE               0
#define I2C_READ	            1


//========================================================================================================
void Init_I2C(void)
{
//  /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;                                   
}
//========================================================================================================
void I2C_Error(void)
{
//    P3 = I2STAT;
//    P3 = ERROR_CODE;
//    while (1);    
}
//========================================================================================================
unsigned char I2C_WriteBuf(unsigned char chip,unsigned char* buf, unsigned char len)
{
    UINT8 u8Count;

	Init_I2C();

    /* Step1 */
    set_STA;                                    /* Send Start bit to I2C EEPROM */
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x08) {                       //Check status value after every step
        I2C_Error();
		return 0;
	}
    
    /* Step2 */
    clr_STA;                                    //STA=0
    I2DAT = (chip | I2C_WRITE);
    clr_SI;
    while (!SI);                                //Check SI set or not
    if (I2STAT != 0x18) {
        I2C_Error();
		return 0;
	}

    /* Step5 */
    for (u8Count = 0; u8Count < len; u8Count++)
    {
        I2DAT = buf[u8Count];
        clr_SI;
        while (!SI);                            //Check SI set or not
        if (I2STAT != 0x28) {
            I2C_Error();
			return 0;
		}
    }

    /* Step7 */
    set_STO;
    clr_SI;
    while (STO);                                /* Check STOP signal */

	return 1;
}

unsigned char BL_Write_Data(unsigned char ADSTART,unsigned char LEN, unsigned char * p)
{
	unsigned char reg_buf[19];
	unsigned char i;
	
	if ( ADSTART + LEN > 19 ) return 0;
	
	reg_buf[0] = 0;
	for(i=0;i<LEN;i++)
		reg_buf[i+1] = p[i];
	
	return I2C_WriteBuf(BL_ADDR,reg_buf,LEN+1);	
}

void BL55072_Config(unsigned char status)
{
	unsigned char BL_Reg[6];

	BL_Reg[0] = 0xEE;	//ICSET InnerOsc,softrst
	BL_Reg[1] = 0xF0;	//BLKCTL BlinkingOff
	BL_Reg[2] = 0xFD;	//DISCTL 50Hz,Line inversion mode,high power mode
	BL_Reg[3] = 0xEC;	//ICSET InnerOsc
	BL_Reg[4] = 0xC8;	//MODESET Display on,1/3 bias
	switch(status){
		case 0:	BL_Reg[5] = 0xFC; break;	//APCTL normal
		case 1:	BL_Reg[5] = 0xFE; break;	//APCTL Apon
		case 2:	BL_Reg[5] = 0xFC; BL_Reg[1] = 0xF3; break;	//APCTL normal,1Hz
		default: break;
	}

	I2C_WriteBuf(BL_ADDR,BL_Reg,6);
}

