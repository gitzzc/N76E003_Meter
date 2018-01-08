#include "stdlib.h"
#include "bike.h"
#include "TM16XX.H"

#define A12()   TM16XX[0] |=  0x0C;
#define A34()   TM16XX[2] |=  0x03;
#define B12()   TM16XX[0] |=  0x03;
#define B34()   TM16XX[0] |=  0xC0;
#define C12()   TM16XX[2] |=  0x30;
#define C34()   TM16XX[4] |=  0x0C;
#define D12()   TM16XX[2] |=  0xC0;
#define D34()   TM16XX[4] |=  0x30;
#define E12()   TM16XX[4] |=  0x03;
#define E34()   TM16XX[4] |=  0xC0;
#define F12()   TM16XX[0] |=  0x30;
#define F34()   TM16XX[2] |=  0x0C;
#define G12()   TM16XX[10] |=  0x30;
#define G34()   TM16XX[10] |=  0xC0;

#define T1()	TM16XX[10] |=  1<<0
#define T2()	TM16XX[6] |=  1<<3
#define T3()	TM16XX[6] |=  1<<2
#define T4()	TM16XX[6] |=  1<<1
#define T5()	TM16XX[6] |=  1<<0
#define U1()	TM16XX[10] |=  1<<1
#define U2()	TM16XX[6] |=  1<<7
#define U3()	TM16XX[6] |=  1<<6
#define U4()	TM16XX[6] |=  1<<5
#define U5()	TM16XX[6] |=  1<<4
#define X1()	TM16XX[10] |=  1<<2
#define X2()	TM16XX[8] |=  1<<3
#define X3()	TM16XX[8] |=  1<<2
#define X4()	TM16XX[8] |=  1<<1
#define X5()	TM16XX[8] |=  1<<0
#define V1()	TM16XX[10] |=  1<<3
#define V2()	TM16XX[8] |=  1<<7
#define V3()	TM16XX[8] |=  1<<6
#define V4()	TM16XX[8] |=  1<<5
#define V5()	TM16XX[8] |=  1<<4
#define S1()	TM16XX[12] |=  1<<0
#define S2()	TM16XX[12] |=  1<<1
#define S3()	TM16XX[12] |=  1<<2
#define S4()	TM16XX[12] |=  1<<3
#define S5()	TM16XX[12] |=  1<<5
#define S6()	TM16XX[12] |=  1<<4

unsigned char flashflag = 0;

void Display(BIKE_STATUS* bike)
{
	unsigned char i = 0;
  
	flashflag++;
	flashflag %= 10;
	
	for(i=0;i<ContainOf(TM16XX);i++)	TM16XX[i] = 0x00;
	
    if( bike->bLFlashType || flashflag >= 5 ){ if ( bike->bLeftFlash ) S4(); }
    if( bike->bRFlashType || flashflag >= 5 ){ if ( bike->bRightFlash) S6(); }
	if( bike->bNearLight 	) S3();
	if( bike->bFarLight 	) S1();
//	if( bike->bBT 			) S2();
//	if( bike->bHallERR 		) //S4	电机故障
//	if( bike->bWheelERR 	) //S5	转把故障
//	if( bike->bECUERR 		) //S6 	电机控制器故障
//	if( bike->bBraked  		) //S7 	刹车
	if( bike->bOverSpeed	) S5();	// 	超速

  /***************************Battery Area Display**********************************/
	switch ( GetBatStatus(sBike.uiBatVoltage) ){
    case 0:	break;
    case 1:
		if ( flashflag >= 5 ) 
			T1();break;
    case 2: T1();break;
    case 3: T1();T2();break;
    case 4: T1();T2();T3();break;
    case 5: T1();T2();T3();T4();break;
    case 6: T1();T2();T3();T4();T5();break;
    default:break; 
	}

	switch ( GetBatStatus(sBike.uiBatVoltage2) ){
    case 0:	break;
    case 1:
		if ( flashflag >= 5 ) 
			U1();break;
    case 2: U1();break;
    case 3: U1();U2();break;
    case 4: U1();U2();U3();break;
    case 5: U1();U2();U3();U4();break;
    case 6: U1();U2();U3();U4();U5();break;
    default:break; 
	}
	
	/*************************** Speed Display**********************************/
	if ( bike->bSpeedFlash == 0 || flashflag >= 5  ) {
		switch( bike->ucSpeed/10 ){
		case 0:A12();B12();C12();D12();E12();F12();break;
		case 1:E12();F12();break;
		case 2:A12();C12();D12();F12();G12();break;
		case 3:A12();D12();E12();F12();G12();break;
		case 4:B12();E12();F12();G12();break;
		case 5:A12();B12();D12();E12();G12();break;
		case 6:A12();B12();C12();D12();E12();G12();break;
		case 7:A12();E12();F12();break;
		case 8:A12();B12();C12();D12();E12();F12();G12();break;
		case 9:A12();B12();D12();E12();F12();G12();break;
		default:break;
		}	
		switch( bike->ucSpeed%10 ){
		case 0:A34();B34();C34();D34();E34();F34();break;
		case 1:E34();F34();break;
		case 2:A34();C34();D34();F34();G34();break;
		case 3:A34();D34();E34();F34();G34();break;
		case 4:B34();E34();F34();G34();break;
		case 5:A34();B34();D34();E34();G34();break;
		case 6:A34();B34();C34();D34();E34();G34();break;
		case 7:A34();E34();F34();break;
		case 8:A34();B34();C34();D34();E34();F34();G34();break;
		case 9:A34();B34();D34();E34();F34();G34();break;
		default:break;
		}
	}
	
	TM1624_Write_Data(TM16XX,14);
}
