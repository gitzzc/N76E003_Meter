/**
  ******************************************************************************
  * @file    
  * @author  ahzzc@sina.com
  * @version V2.xx
  * @date    
  * @brief   
  ******************************************************************************
  * @Changlog
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
//#define JINPENG_4860
//#define JINPENG_6072
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

//#define SINGLE_TRIP	
//#define LCD_SEG_TEST

// uiSpeed*5V*21/1024/fullV*fullSpeed
#define SPEED_CALC_48V(uiSpeed) uiSpeed*1925UL/8192UL	/*24V->55KM/H*/
#define SPEED_CALC_60V(uiSpeed) uiSpeed*1925UL/8192UL	/*30V->55KM/H*/
#define SPEED_CALC_72V(uiSpeed) uiSpeed*1925UL/8192UL	/*36V->55KM/H*/

#ifdef JINPENG_4860
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE

	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_48V
	#undef	SPEED_CALC_60V
	#define SPEED_CALC_48V(uiSpeed) uiSpeed*1505UL/8192UL	/*24V->43KM/H*/
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*301UL /2048UL	/*30V->43KM/H*/
#elif defined JINPENG_6072
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_60V
	#undef	SPEED_CALC_72V
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*1505UL/8192UL	/*24V->43KM/H*/
	#define SPEED_CALC_72V(uiSpeed) uiSpeed*1505UL/12288UL	/*36V->43KM/H*/
#elif defined LCD9040
	#define PCB_VER		0200
	//#define PCB_VER		0100
	//#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
#elif defined LCD9040_4860
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
	#define SINGLE_TRIP
#elif defined LCD9040T
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 1
	#define YXT_ENABLE  1				
	#undef RESET_MILE_ENABLE
#elif defined LCD6040
	#define PCB_VER		0100
	//#define LCD6040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
#elif defined DENGGUAN_XUNYING
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_48V
	#undef	SPEED_CALC_60V
	#undef	SPEED_CALC_72V
	#define SPEED_CALC_48V(uiSpeed) uiSpeed*1925UL/8192UL	/*24V->55KM/H*/
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*385UL /2048UL	/*30V->55KM/H*/
	#define SPEED_CALC_72V(uiSpeed) uiSpeed*1925UL/12288UL	/*36V->55KM/H*/
#elif defined DENGGUAN_XUNYING_T
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 1
	#define YXT_ENABLE  0
	#undef  RESET_MILE_ENABLE
	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_48V
	#undef	SPEED_CALC_60V
	#undef	SPEED_CALC_72V
	#define SPEED_CALC_48V(uiSpeed) uiSpeed*1925UL/8192UL	/*24V->55KM/H*/
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*385UL /2048UL	/*30V->55KM/H*/
	#define SPEED_CALC_72V(uiSpeed) uiSpeed*1925UL/12288UL	/*36V->55KM/H*/
#elif defined BENLING_OUSHANG
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
#elif defined BENLING_BL48_60
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
#elif defined BENLING_ZHONGSHA
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
#elif defined OUJUN
	#define PCB_VER		0100
	#define LCD9040
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
#elif defined JIKE13050
	#define PCB_VER		201745UL
	#define LCD8794GCT
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1
	#define RESET_MILE_ENABLE
#elif defined OUPAINONG_4860
	#define PCB_VER		0100
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_48V
	#undef	SPEED_CALC_60V
	#define SPEED_CALC_48V(uiSpeed) uiSpeed*57750UL/242688UL	/*23.7V->55KM/H*/
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*63000UL/258048UL	/*25.2V->60KM/H*/
#elif defined OUPAINONG_6072
	#define PCB_VER		0100
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
	// uiSpeed*5V*21/1024/fullV*fullSpeed
	#undef	SPEED_CALC_60V
	#undef	SPEED_CALC_72V
	#define SPEED_CALC_60V(uiSpeed) uiSpeed*68250UL/339968UL	/*33.2V->65KM/H*/
	#define SPEED_CALC_72V(uiSpeed) uiSpeed*68250UL/339968UL	/*33.2V->65KM/H*/
#elif defined OUPAINONG_ADJ_4860
	#define PCB_VER		0013
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
#elif defined OUPAINONG_ADJ_6072
	#define PCB_VER		0013
	#define LCD5535
	#define TIME_ENABLE 0
	#define YXT_ENABLE  1				
	#define RESET_MILE_ENABLE
#else
	#error "Please select a release!!!"
#endif

/******************************************************************************/
#if (defined LCD9040) || (defined LCD5535) || (defined LCD8794GCT) || (defined LCD6040 )
#else
#error "Not defined LCD_TYPE"
#endif
    
#ifndef PCB_VER
	#define PCB_VER	0100
#endif

#ifndef TIME_ENABLE
	#define TIME_ENABLE 	0				
#endif

#ifndef YXT_ENABLE
	#define YXT_ENABLE      1				
#endif

#define ContainOf(x) (sizeof(x)/sizeof(x[0]))

#define TASK_INIT	0
#define TASK_STEP1	1
#define TASK_STEP2	2
#define TASK_STEP3	3
#define TASK_STEP4	4
#define TASK_EXIT	0xFF

/******************************************************************************/
#define VOL_CALIBRATIOIN	600UL	//60.0V
#define TEMP_CALIBRATIOIN	250UL	//25.0C
#define SPEED_CALIBRATIOIN	30UL	//30km/h

#define PON_ALLON_TIME		2000UL	//1000ms

#define DISPLAY_MAX_SPEED	45UL	//40km/h
#define SPEEDMODE_DEFAULT	1		//1档

/******************************************************************************/
typedef struct {
    uint8_t bLFlashType	:1;
    uint8_t bRFlashType	:1;
    uint8_t bLeftFlash	:1;	
    uint8_t bRightFlash	:1;	
    uint8_t bTurnLeft	:1;
    uint8_t bTurnRight	:1;

	uint8_t bHotReset	:1;	
	uint8_t bNearLight	:1;
	uint8_t bLastNear	:1;
	uint8_t bBraked		:1;
	uint8_t bCruise		:1;
    uint8_t bLcdFresh	:1;
	uint8_t bMileFlash	:1;	
	uint8_t bSpeedFlash	:1;	
    
	uint8_t bECUERR		:1;
	uint8_t bPhaseERR	:1;
	uint8_t bHallERR	:1;
	uint8_t bWheelERR	:1;
	uint8_t bYXTERR		:1;
	uint8_t bYXTECO		:1;
	uint8_t bYXTRCHG	:1;
	uint8_t bHasTimer	:1;
	uint8_t bTimeSet	:1;
	uint8_t bUart		:1;	
    
	uint8_t 	ucSpeedMode;
	signed int	siTemperature;
	uint16_t  	uiVoltage;
	uint8_t 	ucBatStatus;
	uint8_t 	ucEnergy;
	uint8_t 	ucSpeed;
	uint8_t 	ucPHA_Speed;
	uint8_t 	ucYXT_Speed;
	uint32_t 	ulMile;
	uint32_t 	ulFMile;
	uint16_t  	uiTick;
	
	uint8_t 	ucHour;
	uint8_t 	ucMinute;
	uint8_t 	ucSecond;
	uint8_t 	ucTimePos;
	
} BIKE_STATUS,*pBIKE_STATUS;
	
typedef struct {
	uint8_t 	ucBike[4];
	uint16_t  	uiSysVoltage;
	uint16_t  	uiVolScale;
	uint16_t  	uiTempScale;
	uint16_t  	uiSpeedScale;
	uint16_t  	uiYXT_SpeedScale;
	uint16_t  	uiSingleTrip;
	uint32_t 	ulMile;
	uint8_t 	ucSum;
} BIKE_CONFIG,*pBIKE_CONFIG;
	
extern BIKE_STATUS sBike;
extern BIKE_CONFIG sConfig;
extern volatile uint16_t uiSysTick;

uint16_t Get_SysTick(void);
uint16_t Get_ElapseTick(uint16_t uiPreTick);
void LRFlashTask(void);
/******************************************************************************/

#if ( PCB_VER == 0200 )
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
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_5
#elif ( PCB_VER == 0100 )
	#define SPEEDV_ADC_CH	ADC1_CHANNEL_3
	#define SPEEDV_ADC_SCH	ADC1_SCHMITTTRIG_CHANNEL3

	#define V72_PORT		GPIOA
	#define V72_PIN			GPIO_PIN_2
	#define V48_PORT		GPIOD
	#define V48_PIN			GPIO_PIN_3

	#define SPMODE1_PORT	GPIOD
	#define SPMODE1_PIN		GPIO_PIN_4
	#define SPMODE2_PORT	GPIOD
	#define SPMODE2_PIN		GPIO_PIN_5
	#define SPMODE3_PORT	GPIOA
	#define SPMODE3_PIN		GPIO_PIN_1
	#define SPMODE4_PORT	GPIOA
	#define SPMODE4_PIN		GPIO_PIN_3
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_5
//	#define CRZLight_PORT	
//	#define CRZLight_PIN	
#elif ( PCB_VER == 0013 )
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
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOC
	#define TurnRight_PIN	GPIO_PIN_3
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_5
#elif ( PCB_VER == 0041 )	//for LCD8794GCT
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
	
	#define NearLight_PORT	GPIOC
	#define NearLight_PIN	GPIO_PIN_7
	#define TurnRight_PORT	GPIOD
	#define TurnRight_PIN	GPIO_PIN_4
	#define TurnLeft_PORT	GPIOC
	#define TurnLeft_PIN	GPIO_PIN_3
	
	#define POut_PORT		GPIOC
	#define POut_PIN		GPIO_PIN_5
#endif

#define READ_TURN_LEFT()	GPIO_Read(TurnLeft_PORT , TurnLeft_PIN	)
#define READ_TURN_RIGHT()	GPIO_Read(TurnRight_PORT, TurnRight_PIN )
#define FEED_DOG()			IWDG_ReloadCounter()

/******************************************************************************/

#endif
