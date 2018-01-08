
#line 1 "Code\display.c" /0
  
#line 1 "Code\bike.h" /0






































































































































 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 190 "Code\bike.h" /1
  
  
  
  
  
  
 
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
 
  
  
  
  
 
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
 
  
  
  
  
  
 
  
 
  
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
 
 
#line 354 "Code\bike.h" /0
 
 
 
#line 357 "Code\bike.h" /1
  
 
#line 359 "Code\bike.h" /0
 
 
#line 361 "Code\bike.h" /1
  
 
#line 363 "Code\bike.h" /0
 
 
#line 365 "Code\bike.h" /1
  
 
#line 367 "Code\bike.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef struct {
  unsigned char bLFlashType	:1;
  unsigned char bRFlashType	:1;
  unsigned char bLeftFlash	:1;	
  unsigned char bRightFlash	:1;	
  unsigned char bTurnLeft	:1;
  unsigned char bTurnRight	:1;
  unsigned char bNearLight	:1;
  unsigned char bLastNear	:1;
 
  unsigned char bFarLight	:1;
  unsigned char bOverSpeed	:1;
  unsigned char bHotReset	:1;	
  unsigned char bLcdFresh	:1;
  unsigned char bMileFlash	:1;	
  unsigned char bSpeedFlash	:1;	
  unsigned char bYXTERR		:1;
  unsigned char bCruise		:1;
 
  unsigned char bBraked		:1;
  unsigned char bECUERR		:1;
  unsigned char bPhaseERR	:1;
  unsigned char bHallERR	:1;
  unsigned char bWheelERR	:1;
  unsigned char bECO		:1;
  unsigned char bRCHG		:1;
  unsigned char bParking	:1;
 
  unsigned char bHasTimer	:1;
  unsigned char bUart		:1;	
  unsigned char bTimeSet	:1;
  unsigned char bBT			:1;
 
  unsigned char 	ucSpeedMode;
 signed int	siTemperature;
  unsigned int  	uiBatVoltage;
  unsigned int  	uiBatVoltage2;
  unsigned char 	ucSpeed;
  unsigned char 	ucPHA_Speed;
  unsigned char 	ucYXT_Speed;
  unsigned long 	ulMile;
  unsigned long 	ulFMile;
  unsigned int  	uiTick;
  unsigned int	uiHallCounter;
  unsigned int	uiHallCounter_250ms;
 
  unsigned char 	ucHour;
  unsigned char 	ucMinute;
  unsigned char 	ucSecond;
  unsigned char 	ucTimePos;
 
 } BIKE_STATUS,*pBIKE_STATUS;
 
 typedef struct {
  unsigned char 	ucBike[4];
  unsigned int	uiVersion;
  unsigned int  	uiSysVoltage;
  unsigned int  	uiVolScale;
  unsigned int  	uiVolScale2;
  unsigned int  	uiTempScale;
  unsigned int  	uiSpeedScale;
  unsigned int  	uiYXT_SpeedScale;
  unsigned int  	uiSingleTrip;
  unsigned long 	ulMile;
  unsigned char 	ucSum;
 } BIKE_CONFIG,*pBIKE_CONFIG;
 
 extern BIKE_STATUS xdata sBike;
 extern BIKE_CONFIG xdata sConfig;
 extern volatile unsigned int uiSysTick;
 
  unsigned int Get_SysTick(void);
  unsigned int Get_ElapseTick(unsigned int uiPreTick);
  signed int NTCtoTemp(signed int ntc);
 void HotReset(void);
 void WriteConfig(void);
 void ResetConfig(void);
 void InitConfig(void);
  unsigned char GetBatStatus(unsigned int uiVol);
  unsigned char GetBatEnergy(unsigned int uiVol);
  unsigned char GetSpeedHall(void);
 void LRFlashTask(void);
  unsigned char MileSetupTask(void);
 void MileTask(void);
  unsigned char SpeedCaltTask(void);
 void TimeTask(void);
 void LRFlashTask(void);
 void LightTask(void);
 void exchange_sort16(unsigned int* pData,unsigned int Count);
  unsigned int get_average16(unsigned int* dat, unsigned int len);
 
 
 
#line 521 "Code\bike.h" /1
  
  
 
  
  
 
  
  
 
  
  
  
  
 
  
  
  
  
  
  
 
 
 
  
  
 
  
  
  
  
  
  
 
  
  
  
 
 
#line 559 "Code\bike.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 590 "Code\bike.h" /1
  
  
 
  
  
 
  
  
 
  
  
  
  
 
  
  
  
  
  
  
 
 
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
 
 
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
 
  
  
 
  
 
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
 
#line 719 "Code\bike.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 1 "Code\display.c" /0
 
 
 
  
#line 1 "Code\display_9040.c" /0
  
#line 1 "C:\Keil_v5\C51\Inc\stdlib.h" /0






 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 
 
 
 
 
 
 typedef char wchar_t;
 
 
 
 
 
 
 #pragma SAVE
 #pragma REGPARMS
 
 extern int    abs  (int   val);
 extern long  labs  (long  val);
 
 extern float atof (char *s1);
 extern long  atol (char *s1);
 extern int   atoi (char *s1);
 extern int   rand ();
 extern void  srand (int);
 
 extern float         strtod  (char *, char **);
 extern long          strtol  (char *, char **, unsigned char);
 extern unsigned long strtoul (char *, char **, unsigned char);
 
 
 
 extern void init_mempool          (void xdata *p, unsigned int size);
 extern void xdata *malloc  (unsigned int size);
 extern void free                  (void xdata *p);
 extern void xdata *realloc (void xdata *p, unsigned int size);
 extern void xdata *calloc  (unsigned int size, unsigned int len);
 
 #pragma RESTORE
 
 
#line 1 "Code\display_9040.c" /0
 
  
#line 1 "Code\bike.h" /0






































































































































 
 
 
 
#line 139 "Code\bike.h" /1
  
 
 
 
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
 
  
 
  
 
 
 
  
 
  
  
  
 
  
  
  
  
  
  
 
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
 
  
  
  
  
 
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
  
  
 
  
  
  
  
  
 
  
 
  
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
  
  
 
  
  
  
  
  
  
  
 
  
  
  
  
  
 
  
  
  
  
  
 
 
 
 
 
 
  
 
 
 
  
 
 
 
  
 
 
 
 
  
 
 
 
  
 
 
 
  
 
 
 
 
  
 
 
 
 
  
 
 
 
  
 
 
 
  
 
 
 
 
 
  
  
  
  
  
  
 
  
  
  
  
 
  
  
 
 
 
  
  
  
 
  
  
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
 
  
  
 
  
  
 
  
  
  
  
 
  
  
  
  
  
  
 
 
 
  
  
 
  
  
  
  
  
  
 
  
  
  
 
 
  
  
 
  
  
 
  
  
  
  
 
  
  
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
 
 
  
  
 
  
  
 
  
  
 
  
  
  
  
 
  
  
  
  
  
  
 
 
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
 
 
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
  
  
  
  
 
  
  
 
  
  
 
  
  
 
  
  
 
  
 
  
  
 
  
  
  
  
  
  
  
  
  
  
 
  
  
  
  
 
 
  
 
  
 
  
  
 
  
 
  
  
 
  
  
 
 
 
 
#line 2 "Code\display_9040.c" /0
#line 2 "Code\display_9040.c" /0
 
  
#line 1 "Code\bl55072.h" /0
 
 
 
 
 
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
 
 
 BLDispMode_TypeDef BL_DispMode;
 BLBias_TypeDef BL_Bias;
 
 BLFrameFreq_TypeDef BL_FrameFreq;
 BLDriveMode_TypeDef BL_DriveMode;
 BL_PowerSave_TypeDef BL_PowerSave;
 
 BLState BL_SoftResetCmd;
 BL_OscMode_TypeDef BL_OscMode;
 
 BL_Blinking_TypeDef Bl_Blinking;
 
 BLState BL_AllPixelOnCmd;
 BLState BL_AllPixelOffCmd;
 
 } BL_InitTypeDef;
 
 
 
 
 
 
 
 
 
 
 void BL55072_Config(unsigned char status);
 unsigned char BL_Write_Data(unsigned char ADSTART,unsigned char LEN,unsigned char * p);
 unsigned char I2C_WriteBuf(unsigned char chip,unsigned char* buf, unsigned char len);
 unsigned char I2C_ReadBuf(unsigned char chip,unsigned char addr,unsigned char* buf, unsigned char len);
 
#line 3 "Code\display_9040.c" /0
 
 
 unsigned char BL_Data[19];
 unsigned char flashflag = 0;
 
 const unsigned char SegDataTime[10]     = {0xF5,0x60,0xD3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};
 const unsigned char SegDataVoltage[10]  = {0xF5,0x05,0xD3,0x97,0x27,0xB6,0xF6,0x15,0xF7,0xB7};
 const unsigned char SegDataMile[10] 	= {0xFA,0x0A,0xD6,0x9E,0x2E,0xBC,0xFC,0x1A,0xFE,0xBE};
 const unsigned char SegDataMile2[10]    = {0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF,0xEB};
 const unsigned char SegDataSpeed[10]    = {0x5F,0x06,0x6B,0x2F,0x36,0x3D,0x7D,0x07,0x7F,0x3F};
 const unsigned char SegDataTemp[10] 	= {0xFA,0x0A,0xBC,0x9E,0x4E,0xD6,0xF6,0x8A,0xFE,0xDE};
 
 void Display(BIKE_STATUS* bike)
 {
 unsigned char i = 0;
 
 flashflag++;
 flashflag %= 10;
 
 for(i=0;i<18;i++)	BL_Data[i] = 0x00;
 
 if( bike->bLFlashType || flashflag >= 5 ) { if ( bike->bLeftFlash 	) BL_Data[6] |= 0x08; }	 
 if( bike->bRFlashType || flashflag >= 5 ) {	if ( bike->bRightFlash	) BL_Data[15]|= 0x80; }	 
 if( bike->bCruise		) BL_Data[5] |= 0x02;	 
 if( bike->bNearLight 	) BL_Data[5] |= 0x01;	 
 if( bike->bHallERR 		) BL_Data[5] |= 0x08;	 
 if( bike->bWheelERR 	) BL_Data[5] |= 0x04;	 
 if( bike->bECUERR 		) BL_Data[15]|= 0x20;	 
 
 if( bike->bBraked  		) BL_Data[15]|= 0x40;	 
 
 
 BL_Data[16]|= 0x10;
 switch ( GetBatStatus(sBike.uiBatVoltage) ){
 case 0:
 if ( flashflag < 5 ) 
 BL_Data[16]&=~0x10;break;
 case 1: BL_Data[3] |= 0x10;break;
 case 2: BL_Data[3] |= 0x30;break;
 case 3: BL_Data[3] |= 0x70;break;
 case 4: BL_Data[3] |= 0xF0;break;
 case 5: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0x80;break;
 case 6: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xC0;break;      
 case 7: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xE0;break;
 case 8: BL_Data[3] |= 0xF0;BL_Data[ 9] |= 0xF0;break;          
 default:break; 
 }
 
 
 BL_Data[3] |= (SegDataTemp[abs(bike->siTemperature/10)%10] & 0xF);
 BL_Data[4] |= (SegDataTemp[abs(bike->siTemperature/10)%10] & 0xF0);
 BL_Data[4] |= (SegDataTemp[abs(bike->siTemperature/10)/10] & 0xF);       
 BL_Data[5] |= (SegDataTemp[abs(bike->siTemperature/10)/10] & 0xF0);  
 BL_Data[3] |= 0x1;
 if (bike->siTemperature < 0)
 BL_Data[4] |= 0x01;       
 
 
 
#line 62 "Code\display_9040.c" /1
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 87 "Code\display_9040.c" /0
 
 
 BL_Data[2] |= (SegDataVoltage[ bike->uiBatVoltage	 	 %10]) | 0x8;
 BL_Data[1] |= (SegDataVoltage[(bike->uiBatVoltage/10	)%10]) | 0x8;
 BL_Data[0] |= (SegDataVoltage[(bike->uiBatVoltage/100	)%10]); 
 
 
 
 
 if ( bike->bMileFlash == 0 || flashflag >= 5 ) {
 BL_Data[12] |= (SegDataMile [ bike->ulMile	   	 %10]) | 0x1;
 BL_Data[11] |= (SegDataMile [(bike->ulMile/10   )%10]);
 BL_Data[10] |= (SegDataMile [(bike->ulMile/100  )%10]); 
 BL_Data[ 9] |= (SegDataMile [(bike->ulMile/1000 )%10] & 0x0F); 
 BL_Data[15] |= (SegDataMile [(bike->ulMile/1000 )%10] >> 4);
 BL_Data[16] |= (SegDataMile2[(bike->ulMile/10000)%10]); 
 }		
 
 
 if ( bike->bSpeedFlash == 0 || flashflag >= 5 ) {
 BL_Data[14] |= (SegDataSpeed[ bike->ucSpeed	 	%10]);
 BL_Data[13] |= (SegDataSpeed[(bike->ucSpeed/10)	%10]); 
 BL_Data[14] |= 0x80;
 }
 
 
 switch (bike->ucSpeedMode){
 case 1: BL_Data[11] |= 0x01;break;
 case 2: BL_Data[10] |= 0x01;break;
 case 3: BL_Data[9 ] |= 0x01;break;
 case 4: BL_Data[13] |= 0x80;break;
 default:
 BL_Data[11] &= ~0x01;
 BL_Data[10] &= ~0x01;
 BL_Data[ 9] &= ~0x01;
 BL_Data[13] &= ~0x80;
 break;
 }
 
 BL_Write_Data(0,18,BL_Data);
 }
 
 
#line 4 "Code\display.c" /0
 
 
#line 6 "Code\display.c" /1
  
 
  
 
  
 
  
 
 
 
#line 16 "Code\display.c" /0
 
 
 
 
