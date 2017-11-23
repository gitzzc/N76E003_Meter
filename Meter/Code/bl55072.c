
#include "stm8s_i2c.h"
#include "bl55072.h"
#include "bike.h"


/**
  * @brief Initialize the UART mode according to the specified
  *         parameters in the UART_InitTypeDef and initialize the associated handle.
  * @param huart: UART handle.
  * @retval HAL status
  */
volatile uint16_t  BLTimeout = BL_TIMEOUT;

void BL55072_Config(unsigned char allon)
{
	unsigned char BL_Reg[6];

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

	GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)(GPIO_PIN_4), GPIO_MODE_OUT_OD_HIZ_SLOW);
	GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)(GPIO_PIN_5), GPIO_MODE_OUT_OD_HIZ_SLOW);

	BL_Reg[0] = 0xEE;	//ICSET InnerOsc,softrst
	BL_Reg[1] = 0xF0;	//BLKCTL BlinkingOff
	BL_Reg[2] = 0xFD;	//DISCTL 50Hz,Line inversion mode,high power mode
	BL_Reg[3] = 0xEC;	//ICSET InnerOsc
	BL_Reg[4] = 0xC8;	//MODESET Display on,1/3 bias
	if ( allon )
		BL_Reg[5] = 0xFE;	//APCTL Apon
	else
		BL_Reg[5] = 0xFC;	//APCTL normal
	
	I2C_WriteBuf(BL_ADDR,BL_Reg,6);
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

unsigned char I2C_WriteBuf(unsigned char chip,unsigned char* buf, unsigned char len)
{
  unsigned char i = 0;
	
	/* Perform a read on SR1 and SR3 register to clear eventually pending flags */
	(void)I2C->SR1;
	(void)I2C->SR3;
	
  //I2C_Cmd( DISABLE);
	//Delay(50000);

	//I2C_DeInit();   
  I2C_Cmd( ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(50000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 8);

	BLTimeout = BL_TIMEOUT;
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY)){	if((BLTimeout--) == 0) return 0; }
	
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	/* Test on EV5 and clear it */
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) { if((BLTimeout--) == 0) return 0;	}

	I2C_Send7bitAddress((uint8_t)chip, I2C_DIRECTION_TX);
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {	if((BLTimeout--) == 0) return 0;}
	
	for(i=0;i<len;i++) {
		/* Write data to TXDR */
		I2C_SendData(buf[i]);

		/* Wait until TXIS flag is set */
		BLTimeout = BL_TIMEOUT;
		//while(!I2C_CheckEvent( I2C_FLAG_TRANSFERFINISHED)) { if((BLTimeout--) == 0) return 0;	}
		while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { if((BLTimeout--) == 0) return 0;	}
	}
	
	/* Send STOP condition */
	I2C_GenerateSTOP(ENABLE);

	/* Perform a read on SR1 and SR3 register to clear eventually pending flags */
	(void)I2C->SR1;
	(void)I2C->SR3;

	return 1;
}

unsigned char I2C_ReadBuf(unsigned char chip,unsigned char addr,unsigned char* buf, unsigned char len)
{
  unsigned char i = 0;
	
	//I2C_DeInit();   
  I2C_Cmd( ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(50000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, 2);

	BLTimeout = BL_TIMEOUT;
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY)){	if((BLTimeout--) == 0) return 0; }
	
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	/* Test on EV5 and clear it */
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) { if((BLTimeout--) == 0) return 0;	}

	I2C_Send7bitAddress((uint8_t)chip, I2C_DIRECTION_TX);
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {	if((BLTimeout--) == 0) return 0;}

	/* Write data to TXDR */
	I2C_SendData(addr);

	/* Wait until TXIS flag is set */
	BLTimeout = BL_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { if((BLTimeout--) == 0) return 0;	}

	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	/* Test on EV5 and clear it */
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) { if((BLTimeout--) == 0) return 0;	}

	I2C_Send7bitAddress((uint8_t)chip, I2C_DIRECTION_RX);
	BLTimeout=BL_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {	if((BLTimeout--) == 0) return 0;}

	for(i=0;i<len;i++) {
		BLTimeout=BL_TIMEOUT;
		while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)){ if((BLTimeout--) == 0) return 0;	}; /* Poll on BTF */
		/* Read data to TXDR */
		buf[i] = I2C_ReceiveData();
	}

	/* Clear ACK */
	I2C_AcknowledgeConfig(I2C_ACK_NONE);

	/* Send STOP condition */
	I2C_GenerateSTOP(ENABLE);

	/* Perform a read on SR1 and SR3 register to clear eventually pending flags */
	(void)I2C->SR1;
	(void)I2C->SR3;

	return 1;
}

