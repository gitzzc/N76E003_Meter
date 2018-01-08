/**
  ******************************************************************************
  * @file    
  * @author  ahzzc@sina.com
  * @version V2.xx
  * @date    
  * @brief   
  ******************************************************************************
  * @Changlog
  * V2.35 - 20180104
  * 修改48V系统电量显示最后一格电压值从481到479；
  * 增加电压标定延时，等待电压稳定；
  * 修改霍尔速度处理方式；
  * 
  * V2.34 - 20171226
  * 改BAT_STATUS_xxV，在bike.h文件中定义；调整电压采样滤波效果；
  * 
  * V2.33 - 20171219
  * 改电量外框常亮，调整TM1640通讯速度；
  * 
  * V2.32 - 20171214
  * 完善对JINGPENG-12080的支持；
  * 
  * V2.31 - 20171214
  * 增加对JINGPENG-12080的支持；
  *
  * V2.30 - 20171201
  * 减小闪光器关闭时的判断时间，更改MileTask结构，优化显示效果；
  *
  * V2.29 - 20171128
  * 去除MileResetTask中对速度和刹车状态的判断；改正上电打右转时进入MileResetTask后返回1，造成单次里程显示时间长的问题
  *
  * V2.28 - 20171128
  * 改正RESET_CONFIG时显示的功能，增加JIKE13050的Parking灯输出控制；
  *
  * V2.27 - 20171128
  * 增加RESET_CONFIGA功能，去掉程序检测到版本不一致时清空配置信息的功能；
  *
  * V2.26 - 20171128
  * 修改一线通函数每次调用后都更新速度，解决一线通状态下速度调整时闪烁的问题；
  * 改正了相电压状态下速度调整时显示异常的问题；
  * 
  * V2.25 - 20171127
  * 增加软件版本配置信息，当程序检测到版本不一致时清空配置信息；
  * 修改系统电压功能为预定义模式；
  * 增加PCB200的发行版本；
  * 
  * V2.24 - 20171125
  * 改正速度修正的问题；
  * 
  * V2.23 - 20171122
  * 改正左右转信号检测问题，MileResetTask问题，修改程序结构；
  * 
  * V2.22 - 20171120
  * 增加对9040基本版本2.0PCB的支持、JIKE13050版本的支持
  * 增加相关设计文档
  * 
  * V2.21 - 20171102
  * 增加参数设置单次里程功能，可以通过开关操作进行设置；
  * 
  * V2.20 - 20171027
  * 统一清里程、速度设定的方法；兼容左右灯开关信号和闪光器信号；统一变量命名方式；
  * 
  * V2.13 - 20170921
  * 改正左右转闪光器信号的识别，MileResetTask,YXT_SpeedCaltTask两个功能可以更稳定工作
  * 
  * V2.12 - 20170916
  * 按9040_5535型仪表软件设置要求-0914.docx进行更改
  * 增加MileResetTask,YXT_SpeedCaltTask两个功能．
  * 增加相电压速度时的调节功能，改下一线通电压时初始化的问题．

  * 
  * V2.11 - 20170909
  * 按9040_5535型仪表软件设置要求-0908.doc进行更改：
  * 1.	60/72V系统，电池图标对应电压值
  * 2.	无挡位输入时，相电压对应速度
  * 3.	计算累积里程时，设定的最高速度
  * 增加LCD9040_4860版本
  * 修改左右转灯为独立闪烁
  * 修改工程结构
  * 修正最高速度限制
  * 更改ＧetVol函数和程序结构使用电源快速开关过程中，仪表显示不变．
  * 开机全显时间改为２秒．
  *
  * V2.10 - 20170824
  * 修改总里程进行复位的方法：打右转灯，开机后10秒内开关大灯8次；
  * 左右转信号改回中断方式，适应闪光器；
  * 增加OUPAINONG四个版本的支持；
  * 改正OUPAINONG_6072版本的电压识别；
  *
  * V2.09 - 20170821
  * 增加了开机3秒内开关8次大灯对总里程进行复位的方法；
  * 左右转信号由中断改为程序扫描方式；
  * 增加批量编译功能；
  *
  * V2.08 - 20170817
  * 修改刹车图标显示方式，当YXT DATA2 BIT5 为1时显示刹车灯
  * 改正DENGGUAN_XUNYING_T、JINPENG_6072的速度标定
  * 优化程序空间
  *
  * V2.07
  * 增加开机显示累计里程5秒，如果2秒后有速度则跳到显示单次里程
  * 修正对一线通吉详方案的支持，数据高电平占空比放宽到15-42
  *
  * V2.06
  * 改正标定时的异常
  *
  * V2.05
  * 改正温度显示
  *
  * V2.04
  * V2.02
  * 统一各版本采用release进行定义
  * 增加金彭产品支持，JINPENG_4060、JINPENG_6072
  * 删除对老PCB版本的支持，
  *
  * V2.01
  * 一线通改为多协议同时支持；
  * 速度分档修正；
  *
  * V2.00
	* 功能：
	* 1.系统电压通过MODE1,MODE2自动检测；
	* 2.时钟功能自动检测，无芯片不显示；
	* 3.通过左右转向进行时间调整模式；
	* 4.通过串口可进行参数标定,	短接低速、SWIM信号进行参数标定；
	* 5.参数保存于EEPROM;
	* 6.一线通功能；
	* 7.档位信息优先读取一线通数据，实现四档信息判断，一线通中断后采用档把数据；
	* 8.开启开门狗功能；
	* 9.通过PCB_VER定义不同的硬件版本，支持0011、0012、0022、0041；
	*10.通过YXT_XX定义不同的控制器版本；
  *
  ******************************************************************************
  */
/******************************************************************************/

#ifndef __BIKE_H__
#define __BIKE_H__

/******************************************************************************/
//release define
#define LCD9040
//#define LCD9040_VD48H72L_ST
//#define LCD9040_VD48H72N_ST
//#define LCD9040_JP_45KM
//#define JINPENG_4860
//#define JINPENG_6072
//#define JINPENG_9737
//#define JINPENG_QD
//#define LCD6040
//#define LCD9040
//#define LCD9040_4860
//#define LCD9040T
//#define JIKE13050
//#define DENGGUAN_XUNYING		//DG55,BLSP55
//#define DENGGUAN_XUNYING_T	//TDG55
//#define BENLING_OUSHANG		//BL60_72
//#define BENLING_BL48_60		//BL48_60
//#define BENLING_ZHONGSHA		//BLV72
//#define OUJUN					//M2_60_72
//#define OUPAINONG_4860		//LCD9040_4860
//#define OUPAINONG_6072		//LCD9040_6072
//#define OUPAINONG_ADJ_4860	//LCD9040ADJ_4860
//#define OUPAINONG_ADJ_6072	//LCD9040ADJ_6072
/******************************************************************************/

//#define SW_VER				

//#define SINGLE_TRIP	
//#define LCD_SEG_TEST
//#define RESET_CONFIG

/******************************************************************************/
#define VOL_CALIBRATIOIN	600UL	//60.0V
#define TEMP_CALIBRATIOIN	250UL	//25.0C

#define PON_ALLON_TIME		2000UL	//2000ms

#define DISPLAY_MAX_SPEED	45UL	//40km/h
/******************************************************************************/

#if defined LCD9040
	#define PCB_VER		0100
	//#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD72L48L	
#elif defined LCD9040_ST
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD72L48L	
	#define SINGLE_TRIP
#elif defined LCD9040_VD48H72N_ST
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1	
	#define VD48H72N	
	#define SINGLE_TRIP
#elif defined LCD9040_JP_45KM
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1	
	#define VD72L48L	
	#define SPEED_CALC_48V(spd) spd*1575UL/8192UL	/*24V->45KM/H*/
	#define SPEED_CALC_60V(spd) spd*315UL /2048UL	/*30V->45KM/H*/
	#define SPEED_CALC_72V(spd) spd*525UL /4096UL	/*36V->45KM/H*/
#elif defined JIKE13050
	#define PCB_VER		201745UL
	#define LCD8794GCT
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
//	#define VD48L72N	
	#define VD48N72L	
#elif JINPENG_4860
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1	
	#define VD48L72N
	#define SPEED_CALC_48V(spd) spd*1575UL/8192UL	/*24V->45KM/H*/
	#define SPEED_CALC_60V(spd) spd*315UL /2048UL	/*30V->45KM/H*/
	#define BAT_STATUS_48V 		{420,426,432,439,445,451,457,464}
	#define BAT_STATUS_60V 		{520,528,536,542,550,558,566,574}
	#define BAT_STATUS_72V 		{0xFFFF}	
#elif defined JINPENG_6072
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD48N72L	
	#define SPEED_CALC_60V(spd) spd*1505UL/8192UL	/*24V->43KM/H*/
	#define SPEED_CALC_72V(spd) spd*1505UL/12288UL	/*36V->43KM/H*/
	#define BAT_STATUS_48V 		{0xFFFF}
	#define BAT_STATUS_60V 		{480,493,506,519,532,545,558,570}
	#define BAT_STATUS_72V 		{550,569,589,608,628,647,667,686}
#elif defined JINPENG_9737
	#define PCB_VER		9737
	#define TM1624
	#define TIME_ENABLE 0
	#define YXT_ENABLE  0
	#define VD48	
	#define VOL_CALC(uiVol) 	(uint32_t)uiVol*650UL/1024UL	/* 120K+10K */
	#define BAT_STATUS_48V 		{300,410,438,464,490,516};
	#define BAT_STATUS_60V 		{0xFFFF};
	#define BAT_STATUS_72V 		{0xFFFF};

	#define PERIMETER			2200UL	//mm
	#define PULSE_C				56
#elif defined JINPENG_QD
	#define PCB_VER		0200
	#define TM1640
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD48L72N	
	#define SPEED_CALC_48V(spd) spd*1575UL/8192UL	/*24V->45KM/H*/
	#define SPEED_CALC_60V(spd) spd*315UL /2048UL	/*30V->45KM/H*/
	#define BAT_STATUS_48V 		{420,426,433,440,447,454,462,470};
	#define BAT_STATUS_60V 		{520,528,537,546,555,563,571,579};
	#define BAT_STATUS_72V 		{0xFFFF};
#elif defined LCD9040_4860
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1	
	#define VD48H72L	
	#define SINGLE_TRIP
#elif defined LCD9040T
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 1
	#define YXT_ENABLE  1				
	#define VD72L48L	
#elif defined LCD6040
	#define PCB_VER		0200
	//#define LCD6040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD72L48L	
	#define BAT_STATUS_48V {425,432,444,456,468};
	#define BAT_STATUS_60V {525,537,553,566,578};
	#define BAT_STATUS_72V {630,641,661,681,701};
#elif defined DENGGUAN_XUNYING
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD72L48L	
#elif defined DENGGUAN_XUNYING_T
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 1
	#define YXT_ENABLE  0
	#define VD72L48L	
#elif defined BENLING_OUSHANG
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD72L48L	
#elif defined BENLING_BL48_60
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD72L48L	
#elif defined BENLING_ZHONGSHA
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD72
#elif defined OUJUN
	#define PCB_VER		0200
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define VD48N72L
#elif defined OUPAINONG_4860
	#define PCB_VER		0200
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD48L72N	
	#define SPEED_CALC_48V(spd) spd*57750UL/242688UL	/*23.7V->55KM/H*/
	#define SPEED_CALC_60V(spd) spd*63000UL/258048UL	/*25.2V->60KM/H*/
#elif defined OUPAINONG_6072
	#define PCB_VER		0200
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD48N72L	
	#define SPEED_CALC_60V(spd) spd*68250UL/339968UL	/*33.2V->65KM/H*/
	#define SPEED_CALC_72V(spd) spd*68250UL/339968UL	/*33.2V->65KM/H*/
#elif defined OUPAINONG_ADJ_4860
	#define PCB_VER		0013
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD72L48L	
#elif defined OUPAINONG_ADJ_6072
	#define PCB_VER		0013
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define VD72L48L	
#else
	#error "Please select a release!!!"
#endif

/******************************************************************************/
#ifndef PCB_VER
	#define PCB_VER	0200
#endif

#ifndef TIME_ENABLE
	#define TIME_ENABLE 	0				
#endif

#ifndef YXT_ENABLE
	#define YXT_ENABLE      1				
#endif

// spd*5V*(200+10)/10/1024/fullV*fullSpeed	/*200k+10k*/
#ifndef SPEED_CALC_48V
#define SPEED_CALC_48V(spd) spd*1925UL/8192UL	/*200k+10k 24V->55KM/H*/
#endif

#ifndef SPEED_CALC_60V
#define SPEED_CALC_60V(spd) spd*385UL /2048UL;	/*200k+10k 30V->55KM/H*/
#endif

#ifndef SPEED_CALC_72V
#define SPEED_CALC_72V(spd) spd*1925UL/12288UL	/*200k+10k 36V->55KM/H*/
#endif

// uiVol = (uint32_t)uiVol / 1024UL / 10 * (200+10) * 5 * 10;	/* 200k+10k */
#ifndef VOL_CALC
#define VOL_CALC(uiVol) (uint32_t)uiVol*1050UL/1024UL	/* 200k+10k */
#endif

#ifndef BAT_STATUS_48V
//#define BAT_STATUS_48V {420,427,435,444,453,462,471,481}
#define BAT_STATUS_48V {420,427,435,444,453,462,471,479}	/*20180104*/
#endif

#ifndef BAT_STATUS_60V
#define BAT_STATUS_60V {520,531,544,556,568,577,587,595}
#endif

#ifndef BAT_STATUS_72V
#define BAT_STATUS_72V {630,642,653,664,675,687,700,715}
#endif

/******************************************************************************/


#define TASK_INIT	0
#define TASK_STEP1	1
#define TASK_STEP2	2
#define TASK_STEP3	3
#define TASK_STEP4	4
#define TASK_EXIT	0xFF

#define GPIO_QUASI		0
#define GPIO_PUSH_PULL	1
#define GPIO_INPUT_ONLY	2
#define GPIO_OPEN_DRAIN	3

#define RESET	0
#define SET		1


/******************************************************************************/
#define uint8_t 	unsigned char
#define uint16_t 	unsigned int
#define uint32_t 	unsigned long

#define int8_t 		signed char
#define int16_t 	signed int
#define int32_t 	signed long

/******************************************************************************/
typedef struct {
    uint8_t bLFlashType	:1;
    uint8_t bRFlashType	:1;
    uint8_t bLeftFlash	:1;	
    uint8_t bRightFlash	:1;	
    uint8_t bTurnLeft	:1;
    uint8_t bTurnRight	:1;
	uint8_t bNearLight	:1;
	uint8_t bLastNear	:1;

	uint8_t bFarLight	:1;
	uint8_t bOverSpeed	:1;
	uint8_t bHotReset	:1;	
    uint8_t bLcdFresh	:1;
	uint8_t bMileFlash	:1;	
	uint8_t bSpeedFlash	:1;	
	uint8_t bYXTERR		:1;
	uint8_t bCruise		:1;
    
	uint8_t bBraked		:1;
	uint8_t bECUERR		:1;
	uint8_t bPhaseERR	:1;
	uint8_t bHallERR	:1;
	uint8_t bWheelERR	:1;
	uint8_t bECO		:1;
	uint8_t bRCHG		:1;
	uint8_t bParking	:1;
	
	uint8_t bHasTimer	:1;
	uint8_t bUart		:1;	
	uint8_t bTimeSet	:1;
	uint8_t bBT			:1;
    
	uint8_t 	ucSpeedMode;
	signed int	siTemperature;
	uint16_t  	uiBatVoltage;
	uint16_t  	uiBatVoltage2;
	uint8_t 	ucSpeed;
	uint8_t 	ucPHA_Speed;
	uint8_t 	ucYXT_Speed;
	uint32_t 	ulMile;
	uint32_t 	ulFMile;
	uint16_t  	uiTick;
	uint16_t	uiHallCounter;
	uint16_t	uiHallCounter_250ms;
	
	uint8_t 	ucHour;
	uint8_t 	ucMinute;
	uint8_t 	ucSecond;
	uint8_t 	ucTimePos;
	
} BIKE_STATUS,*pBIKE_STATUS;
	
typedef struct {
	uint8_t 	ucBike[4];
	uint16_t	uiVersion;
	uint16_t  	uiSysVoltage;
	uint16_t  	uiVolScale;
	uint16_t  	uiVolScale2;
	uint16_t  	uiTempScale;
	uint16_t  	uiSpeedScale;
	uint16_t  	uiYXT_SpeedScale;
	uint16_t  	uiSingleTrip;
	uint32_t 	ulMile;
	uint8_t 	ucSum;
} BIKE_CONFIG,*pBIKE_CONFIG;
	
extern BIKE_STATUS xdata sBike;
extern BIKE_CONFIG xdata sConfig;
extern volatile uint16_t uiSysTick;

uint16_t Get_SysTick(void);
uint16_t Get_ElapseTick(uint16_t uiPreTick);
int16_t NTCtoTemp(int16_t ntc);
void HotReset(void);
void WriteConfig(void);
void ResetConfig(void);
void InitConfig(void);
uint8_t GetBatStatus(uint16_t uiVol);
uint8_t GetBatEnergy(uint16_t uiVol);
uint8_t GetSpeedHall(void);
void LRFlashTask(void);
uint8_t MileSetupTask(void);
void MileTask(void);
uint8_t SpeedCaltTask(void);
void TimeTask(void);
void LRFlashTask(void);
void LightTask(void);
void exchange_sort16(uint16_t* pData,uint16_t Count);
uint16_t get_average16(uint16_t* dat, uint16_t len);
/******************************************************************************/

#if ( PCB_VER == 0200 )
	#define BATV_ADC_CH		ADC1_CHANNEL_2
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL2

	#define SPEEDV_ADC_CH	ADC1_CHANNEL_3
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL3

	#define SPEEDV_ADJ_CH	ADC1_CHANNEL_4
	#define SPEEDV_ADJ_SCH	ADC1_SCHMITTTRIG_CHANNEL4
	
	#define V72_PORT		GPIOD
	#define V72_PIN			GPIO_PIN_5
	#define V48_PORT		GPIOD
	#define V48_PIN			GPIO_PIN_4

	#define SPMODE1_PORT	GPIOA
	#define SPMODE1_PIN		GPIO_PIN_3
	#define SPMODE2_PORT	GPIOA
	#define SPMODE2_PIN		GPIO_PIN_2
	#define SPMODE3_PORT	GPIOA
	#define SPMODE3_PIN		GPIO_PIN_1
	//#define SPMODE4_PORT	GPIOA
	//#define SPMODE4_PIN		GPIO_PIN_3
	
	#define CALI_PORT		SPMODE1_PORT
	#define CALI_PIN		SPMODE1_PIN
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_5
	
	#define TM16XX_PORT	GPIOB
	#define TM16XX_CLK	GPIO_PIN_4
	#define TM16XX_DAT	GPIO_PIN_5

#elif ( PCB_VER == 0100 )
	#define BATV_ADC_CH		ADC1_CHANNEL_2
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL2

	#define SPEEDV_ADC_CH	ADC1_CHANNEL_3
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL3

	#define V72_PORT		P0
	#define V72_PIN			1
	#define V48_PORT		P1
	#define V48_PIN			2

	#define SPMODE1_PORT	P0
	#define SPMODE1_PIN		0
	#define SPMODE2_PORT	P1
	#define SPMODE2_PIN		1
	#define SPMODE3_PORT	P2
	#define SPMODE3_PIN		2
	#define SPMODE4_PORT	P3
	#define SPMODE4_PIN		3
	
	#define CALI_PORT		SPMODE1_PORT
	#define CALI_PIN		SPMODE1_PIN
	
	#define NearLight_PORT	P2
	#define NearLight_PIN	1
	#define TurnRight_PORT	P2
	#define TurnRight_PIN	2
	#define TurnLeft_PORT	P2
	#define TurnLeft_PIN	3

#elif ( PCB_VER == 0013 )
	#define BATV_ADC_CH		ADC1_CHANNEL_2
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL2

	#define SPEEDV_ADC_CH	ADC1_CHANNEL_3
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL3

	#define SPEEDV_ADJ_CH	ADC1_CHANNEL_4
	#define SPEEDV_ADJ_SCH	ADC1_SCHMITTTRIG_CHANNEL4

	#define V72_PORT		GPIOD
	#define V72_PIN			GPIO_PIN_4
	#define V48_PORT		GPIOD
	#define V48_PIN			GPIO_PIN_5

	#define SPMODE1_PORT	GPIOA
	#define SPMODE1_PIN		GPIO_PIN_3
	#define SPMODE2_PORT	GPIOA
	#define SPMODE2_PIN		GPIO_PIN_2
	#define SPMODE3_PORT	GPIOA
	#define SPMODE3_PIN		GPIO_PIN_1
//	#define SPMODE4_PORT	
//	#define SPMODE4_PIN		
	
	#define CALI_PORT		SPMODE1_PORT
	#define CALI_PIN		SPMODE1_PIN
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_5
#elif ( PCB_VER == 0041 )	//for LCD8794GCT
	#define BATV_ADC_CH		ADC1_CHANNEL_2
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL2

	#define SPEEDV_ADC_CH	ADC1_CHANNEL_5
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL5

	#define SPMODE1_PORT	GPIOD
	#define SPMODE1_PIN		GPIO_PIN_4
	#define SPMODE2_PORT	GPIOA
	#define SPMODE2_PIN		GPIO_PIN_1
	#define SPMODE3_PORT	GPIOA
	#define SPMODE3_PIN		GPIO_PIN_2
	#define SPMODE4_PORT	GPIOA
	#define SPMODE4_PIN		GPIO_PIN_3
	
	#define CALI_PORT		SPMODE1_PORT
	#define CALI_PIN		SPMODE1_PIN
	
	#define NearLight_PORT	GPIOD
	#define NearLight_PIN	GPIO_PIN_3
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOD
	#define TurnLeft_PIN	GPIO_PIN_2
//	#define CRZLight_PORT	
//	#define CRZLight_PIN	
	
	#define NearLightOut_PORT	GPIOC
	#define NearLightOut_PIN	GPIO_PIN_7
	#define TurnRightOut_PORT	GPIOC
	#define TurnRightOut_PIN	GPIO_PIN_5
	#define TurnLeftOut_PORT	GPIOD
	#define TurnLeftOut_PIN		GPIO_PIN_1	
#elif ( PCB_VER == 201745UL )	//for LCD8794GCT
	#define BATV_ADC_CH		ADC1_CHANNEL_2
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL2

	#define SPEEDV_ADC_CH	ADC1_CHANNEL_3
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL3

	#define SPEEDV_ADJ_CH	ADC1_CHANNEL_4
	#define SPEEDV_ADJ_SCH	ADC1_SCHMITTTRIG_CHANNEL4

	#define V48_PORT		GPIOD
	#define V48_PIN			GPIO_PIN_5

	#define SPMODE1_PORT	GPIOA
	#define SPMODE1_PIN		GPIO_PIN_3
	#define SPMODE2_PORT	GPIOA
	#define SPMODE2_PIN		GPIO_PIN_1
	#define SPMODE3_PORT	GPIOA
	#define SPMODE3_PIN		GPIO_PIN_2
	
	#define CALI_PORT		SPMODE1_PORT
	#define CALI_PIN		SPMODE1_PIN
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOD
	#define TurnRight_PIN	GPIO_PIN_4
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_3
	
	#define POut_PORT		GPIOC
	#define POut_PIN		GPIO_PIN_5
#elif ( PCB_VER == 9737 )
	#define BATV_ADC_CH		ADC1_CHANNEL_6
	#define BATV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL6

	#define BATV_ADC_CH2	ADC1_CHANNEL_2
	#define BATV_ADC_SCH2	ADC1_SCHMITTTRIG_CHANNEL2
	
	#define SPEED_HALL_PORT	GPIOC
	#define SPEED_HALL_PIN	GPIO_PIN_3

    #define SPEED_EXTI_PORT EXTI_PORT_GPIOC

	#define CALI_PORT		GPIOA
	#define CALI_PIN		GPIO_PIN_3
	
	#define NearLight_PORT	GPIOD
	#define NearLight_PIN	GPIO_PIN_4
	#define FarLight_PORT	GPIOD
	#define FarLight_PIN	GPIO_PIN_5
	#define OverSpeed_PORT	GPIOA
	#define OverSpeed_PIN	GPIO_PIN_3
	#define TurnRight_PORT	GPIOB
	#define TurnRight_PIN	GPIO_PIN_4
	#define TurnLeft_PORT	GPIOB
	#define TurnLeft_PIN	GPIO_PIN_5

	#define TM16XX_PORT		GPIOC
	#define TM16XX_CLK		GPIO_PIN_5
	#define TM16XX_DAT		GPIO_PIN_6
	#define TM16XX_CS		GPIO_PIN_7
#endif

#define INC_SYS_TICK()	uiSysTick++

#define ContainOf(x) 		(sizeof(x)/sizeof(x[0]))
	
#define READ_TURN_LEFT()	GPIO_Read(TurnLeft_PORT , TurnLeft_PIN	)
#define READ_TURN_RIGHT()	GPIO_Read(TurnRight_PORT, TurnRight_PIN )

#define FEED_DOG()			

#define DISABLE_INTERRUPTS()	EA = 0
#define ENABLE_INTERRUPTS()		EA = 1

#define LED_OFF()	P15 = 0
#define LED_ON()	P15 = 1

/******************************************************************************/

#endif
