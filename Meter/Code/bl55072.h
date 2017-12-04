#ifndef __BL55072_H__
#define __BL55072_H__



typedef enum {
  
  BL_DISABLE = 0, 
  BL_ENABLE = !BL_DISABLE
    
} BLState;

typedef enum {
  
	Alloff = 0,
	AllOn  = 1
          
} BLAllPixel_TypeDef;

typedef enum {

	DispOff = 0,
	DispOn  = 1
          
} BLDispMode_TypeDef;

typedef enum{
  
	OneThirdOfBias = 0,
	OneHalfOfBias = 1
          
} BLBias_TypeDef;

typedef enum {
  
	BL_FrameFreq_80Hz = 0,
	BL_FrameFreq_71Hz = 1,
	BL_FrameFreq_64Hz = 2,
	BL_FrameFreq_50Hz = 3
          
} BLFrameFreq_TypeDef;

typedef enum {
  
	LineInversion = 0,
	FrameInversion = 1
          
} BLDriveMode_TypeDef;

typedef enum {
  
	PowerSaveMode1 = 0,
	PowerSaveMode2 = 1,
	NormalMode = 2,
	HighPowerMode = 3
          
} BL_PowerSave_TypeDef;

typedef enum {
  
	InnerOsc = 0,
	OutsideOsc = 1
          
} BL_OscMode_TypeDef;

typedef enum {
  
	BlinkingOff = 0,
	Blinking0_25Hz =1,
	Blinking0_50Hz = 2,
	Blinking1_00Hz = 3
          
} BL_Blinking_TypeDef;

typedef struct
{
  
	/*MODE SET Register*/
	BLDispMode_TypeDef BL_DispMode;
	BLBias_TypeDef BL_Bias;
	/*DISCTL Register*/
	BLFrameFreq_TypeDef BL_FrameFreq;
	BLDriveMode_TypeDef BL_DriveMode;
	BL_PowerSave_TypeDef BL_PowerSave;
	/*ICSET Register*/
	BLState BL_SoftResetCmd;
	BL_OscMode_TypeDef BL_OscMode;
	/*BLKCTL*/
	BL_Blinking_TypeDef Bl_Blinking;
	/*APCTL*/
	BLState BL_AllPixelOnCmd;
	BLState BL_AllPixelOffCmd;
        
} BL_InitTypeDef;

#define REG_MODE_SET_DEFAULT  (unsigned char)(0xC0)
#define REG_ADSET_DEFAULT     (unsigned char)(0x00)
#define REG_DISCTL_DEFAULT    (unsigned char)(0xE2)
#define REG_ICSET_DEFAULT     (unsigned char)(0xE8)
#define REG_BLKCTL_DEFAULT    (unsigned char)(0xF0)
#define REG_APCTL_DEFAULT     (unsigned char)(0xFC)
#define BL_ADDR               (unsigned char)(0x7C)
#define BL_TIMEOUT            (uint16_t)(0x1000)

void BL55072_Config(unsigned char status);
unsigned char BL_Write_Data(unsigned char ADSTART,unsigned char LEN,unsigned char * p);
unsigned char I2C_WriteBuf(unsigned char chip,unsigned char* buf, unsigned char len);
unsigned char I2C_ReadBuf(unsigned char chip,unsigned char addr,unsigned char* buf, unsigned char len);
#endif
