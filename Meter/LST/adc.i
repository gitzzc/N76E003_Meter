
#line 1 "Code\adc.c" /0
  
#line 1 "..\Include\N76E003.h" /0




 
 
 sfr P0					= 0x80;
 sfr SP					= 0x81;
 sfr DPL					= 0x82;
 sfr DPH					= 0x83;
 sfr RCTRIM0			= 0x84;
 sfr RCTRIM1			= 0x85;	
 sfr RWK         = 0x86;
 sfr PCON        = 0x87;
 
 sfr TCON        = 0x88;
 sfr TMOD        = 0x89;
 sfr TL0         = 0x8A;
 sfr TL1         = 0x8B;
 sfr TH0         = 0x8C;
 sfr TH1         = 0x8D;
 sfr CKCON       = 0x8E;
 sfr WKCON       = 0x8F;
 
 sfr P1          = 0x90;
 sfr SFRS        = 0x91;  
 sfr CAPCON0     = 0x92;
 sfr CAPCON1     = 0x93;
 sfr CAPCON2     = 0x94;
 sfr CKDIV       = 0x95;
 sfr CKSWT       = 0x96;  
 sfr CKEN        = 0x97;  
 
 sfr SCON        = 0x98;
 sfr SBUF        = 0x99;
 sfr SBUF_1      = 0x9A;
 sfr EIE         = 0x9B;
 sfr EIE1        = 0x9C;
 sfr CHPCON      = 0x9F;  
 
 sfr P2          = 0xA0;
 sfr AUXR1       = 0xA2;
 sfr BODCON0     = 0xA3;  
 sfr IAPTRG      = 0xA4;  
 sfr IAPUEN      = 0xA5;	 
 sfr IAPAL       = 0xA6;
 sfr IAPAH       = 0xA7;
 
 sfr IE          = 0xA8;
 sfr SADDR       = 0xA9;
 sfr WDCON       = 0xAA;  
 sfr BODCON1     = 0xAB;  
 sfr P3M1        = 0xAC;
 sfr P3S         = 0xAC;  
 sfr P3M2        = 0xAD;
 sfr P3SR        = 0xAD;  
 sfr IAPFD       = 0xAE;
 sfr IAPCN       = 0xAF;
 
 sfr P3          = 0xB0;
 sfr P0M1        = 0xB1;
 sfr P0S         = 0xB1;  
 sfr P0M2        = 0xB2;
 sfr P0SR        = 0xB2;  
 sfr P1M1        = 0xB3;
 sfr P1S         = 0xB3;  
 sfr P1M2        = 0xB4;
 sfr P1SR        = 0xB4;  
 sfr P2S         = 0xB5; 
 sfr IPH         = 0xB7;
 sfr PWMINTC			= 0xB7;	 
 
 sfr IP          = 0xB8;
 sfr SADEN       = 0xB9;
 sfr SADEN_1     = 0xBA;
 sfr SADDR_1     = 0xBB;
 sfr I2DAT       = 0xBC;
 sfr I2STAT      = 0xBD;
 sfr I2CLK       = 0xBE;
 sfr I2TOC       = 0xBF;
 
 sfr I2CON       = 0xC0;
 sfr I2ADDR      = 0xC1;
 sfr ADCRL       = 0xC2;
 sfr ADCRH       = 0xC3;
 sfr T3CON       = 0xC4;
 sfr PWM4H       = 0xC4;  
 sfr RL3         = 0xC5;
 sfr PWM5H       = 0xC5;	 
 sfr RH3         = 0xC6;
 sfr PIOCON1     = 0xC6;  
 sfr TA          = 0xC7;
 
 sfr T2CON       = 0xC8;
 sfr T2MOD       = 0xC9;
 sfr RCMP2L      = 0xCA;
 sfr RCMP2H      = 0xCB;
 sfr TL2         = 0xCC; 
 sfr PWM4L       = 0xCC;  
 sfr TH2         = 0xCD;
 sfr PWM5L       = 0xCD;  
 sfr ADCMPL      = 0xCE;
 sfr ADCMPH      = 0xCF;
 
 sfr PSW         = 0xD0;
 sfr PWMPH       = 0xD1;
 sfr PWM0H				= 0xD2;
 sfr PWM1H				= 0xD3;
 sfr PWM2H				= 0xD4;
 sfr PWM3H				= 0xD5;
 sfr PNP					= 0xD6;
 sfr FBD					= 0xD7;
 
 sfr PWMCON0			= 0xD8;
 sfr PWMPL       = 0xD9;
 sfr PWM0L				= 0xDA;
 sfr PWM1L				= 0xDB;
 sfr PWM2L				= 0xDC;
 sfr PWM3L				= 0xDD;
 sfr PIOCON0			= 0xDE;
 sfr PWMCON1     = 0xDF;
 
 sfr ACC         = 0xE0;
 sfr ADCCON1     = 0xE1;
 sfr ADCCON2     = 0xE2;
 sfr ADCDLY      = 0xE3;
 sfr C0L         = 0xE4;
 sfr C0H         = 0xE5;
 sfr C1L         = 0xE6;
 sfr C1H         = 0xE7;
 
 sfr ADCCON0     = 0xE8;
 sfr PICON       = 0xE9;
 sfr PINEN       = 0xEA;
 sfr PIPEN       = 0xEB;
 sfr PIF         = 0xEC;
 sfr C2L         = 0xED;
 sfr C2H         = 0xEE;
 sfr EIP         = 0xEF;
 
 sfr B           = 0xF0;
 sfr CAPCON3			= 0xF1;
 sfr CAPCON4			= 0xF2;
 sfr SPCR        = 0xF3;
 sfr SPCR2				= 0xF3;  
 sfr SPSR        = 0xF4;
 sfr SPDR        = 0xF5;
 sfr AINDIDS			= 0xF6;
 sfr EIPH        = 0xF7;
 
 sfr SCON_1      = 0xF8;
 sfr PDTEN       = 0xF9;  
 sfr PDTCNT      = 0xFA;  
 sfr PMEN        = 0xFB;
 sfr PMD         = 0xFC;
 sfr EIP1        = 0xFE;
 sfr EIPH1       = 0xFF;
 
 
 
 sbit SM0_1      = SCON_1^7;
 sbit FE_1       = SCON_1^7; 
 sbit SM1_1      = SCON_1^6; 
 sbit SM2_1      = SCON_1^5; 
 sbit REN_1      = SCON_1^4; 
 sbit TB8_1      = SCON_1^3; 
 sbit RB8_1      = SCON_1^2; 
 sbit TI_1       = SCON_1^1; 
 sbit RI_1       = SCON_1^0; 
 
 
 sbit ADCF       = ADCCON0^7;
 sbit ADCS       = ADCCON0^6;
 sbit ETGSEL1    = ADCCON0^5;
 sbit ETGSEL0    = ADCCON0^4;
 sbit ADCHS3     = ADCCON0^3;
 sbit ADCHS2     = ADCCON0^2;
 sbit ADCHS1     = ADCCON0^1;
 sbit ADCHS0     = ADCCON0^0;
 
 
 sbit PWMRUN     = PWMCON0^7;
 sbit LOAD       = PWMCON0^6;
 sbit PWMF       = PWMCON0^5;
 sbit CLRPWM     = PWMCON0^4;
 
 
 
 sbit CY         = PSW^7;
 sbit AC         = PSW^6;
 sbit F0         = PSW^5;
 sbit RS1        = PSW^4;
 sbit RS0        = PSW^3;
 sbit OV         = PSW^2;
 sbit P          = PSW^0;
 
 
 sbit TF2        = T2CON^7;
 sbit TR2        = T2CON^2;
 sbit CM_RL2     = T2CON^0;
 
 
 sbit I2CEN      = I2CON^6;
 sbit STA        = I2CON^5;
 sbit STO        = I2CON^4;
 sbit SI         = I2CON^3;
 sbit AA         = I2CON^2;
 sbit I2CPX	= I2CON^0;
 
 
 sbit PADC       = IP^6;
 sbit PBOD       = IP^5;
 sbit PS         = IP^4;
 sbit PT1        = IP^3;
 sbit PX1        = IP^2;
 sbit PT0        = IP^1;
 sbit PX0        = IP^0;
 
 
 sbit P30		= P3^0;
 
 
 
 sbit EA         = IE^7;
 sbit EADC       = IE^6;
 sbit EBOD       = IE^5;
 sbit ES         = IE^4;
 sbit ET1        = IE^3;
 sbit EX1        = IE^2;
 sbit ET0        = IE^1;
 sbit EX0        = IE^0;
 
 
 sbit P20        = P2^0;
 
 
 sbit SM0        = SCON^7;
 sbit FE         = SCON^7; 
 sbit SM1        = SCON^6; 
 sbit SM2        = SCON^5; 
 sbit REN        = SCON^4; 
 sbit TB8        = SCON^3; 
 sbit RB8        = SCON^2; 
 sbit TI         = SCON^1; 
 sbit RI         = SCON^0; 
 
 
 sbit P17	= P1^7;
 sbit P16	= P1^6;
 sbit TXD_1	= P1^6; 
 sbit P15	= P1^5;
 sbit P14	= P1^4;
 sbit SDA	= P1^4;    
 sbit P13	= P1^3;
 sbit SCL	= P1^3;  
 sbit P12        = P1^2; 
 sbit P11        = P1^1;
 sbit P10        = P1^0;
 
 
 sbit TF1				= TCON^7;
 sbit TR1				= TCON^6;
 sbit TF0				= TCON^5;
 sbit TR0				= TCON^4;
 sbit IE1				= TCON^3;
 sbit IT1				= TCON^2;
 sbit IE0				= TCON^1;
 sbit IT0				= TCON^0;
 
 
 
 sbit P07				= P0^7;
 sbit RXD				= P0^7;
 sbit P06				= P0^6;
 sbit TXD				= P0^6;
 sbit P05				= P0^5;
 sbit P04				= P0^4;
 sbit STADC				= P0^4;
 sbit P03				= P0^3;
 sbit P02				= P0^2;
 sbit RXD_1				= P0^2;
 sbit P01				= P0^1;
 sbit MISO				= P0^1;
 sbit P00				= P0^0;
 sbit MOSI				= P0^0;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 1 "Code\adc.c" /0
 
  
#line 1 "..\Include\SFR_Macro.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 2 "Code\adc.c" /0
 
  
#line 1 "..\Include\Function_define.h" /0




 
 
  
#line 1 "C:\Keil_v5\C51\Inc\intrins.h" /0






 
 
 
 
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 
 
#line 7 "..\Include\Function_define.h" /0
 
  
#line 1 "C:\Keil_v5\C51\Inc\stdio.h" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
 
 
 
#line 8 "..\Include\Function_define.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 


 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 


 
 
#line 206 "..\Include\Function_define.h" /1
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 
#line 221 "..\Include\Function_define.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 240 "..\Include\Function_define.h" /1
  
  
  
  
 
#line 245 "..\Include\Function_define.h" /0
 
#line 246 "..\Include\Function_define.h" /1
  
  
  
  
 
#line 251 "..\Include\Function_define.h" /0
 
#line 252 "..\Include\Function_define.h" /1
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 
#line 267 "..\Include\Function_define.h" /0
 
#line 268 "..\Include\Function_define.h" /1
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 
#line 283 "..\Include\Function_define.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
 
#line 3 "Code\adc.c" /0
 
  
#line 1 "..\Include\Common.h" /0
 typedef bit                   BIT;
 typedef unsigned char         UINT8;
 typedef unsigned int          UINT16;
 typedef unsigned long         UINT32;
 
 typedef unsigned char         uint8_t;
 typedef unsigned int          uint16_t;
 typedef unsigned long         uint32_t;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 void  InitialUART0_Timer1(UINT32 u32Baudrate);  
 void  InitialUART0_Timer3(UINT32 u32Baudrate);  
 void  InitialUART1_Timer3(UINT32 u32Baudrate);
 void  Send_Data_To_UART0(UINT8 c);
 UINT8 Receive_Data_From_UART0(void);
 void  Send_Data_To_UART1(UINT8 c);
 UINT8 Receive_Data_From_UART1(void);
 void  InitialUART1(UINT32 u32Baudrate);
 
 extern bit BIT_TMP;
#line 4 "Code\adc.c" /0
 
  
#line 1 "..\Include\Delay.h" /0
 void Timer0_Delay100us(UINT32 u32CNT);
 void Timer0_Delay1ms(UINT32 u32CNT);
 void Timer1_Delay1ms(UINT32 u32CNT);
 void Timer1_Delay10ms(UINT32 u32CNT);
 void Timer2_Delay500us(UINT32 u32CNT);
 void Timer3_Delay100ms(UINT32 u32CNT);
 
 void Timer0_Delay40ms(UINT32 u32CNT);
 void Timer3_Delay10us(UINT32 u32CNT);
#line 5 "Code\adc.c" /0
 
 
  
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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 7 "Code\adc.c" /0
 
  
#line 1 "Code\adc.h" /0
 
 
 
 
 
 
 
 
 
 extern unsigned char xdata ucADC_ChannelBuf[];
 extern unsigned int xdata uiADC_SampleBuf[2][32];
 extern unsigned int uiBandgap_Voltage,uiVDD_Voltage;
 
 void READ_BANDGAP();
 void ADC_Init(void);
 
 
#line 8 "Code\adc.c" /0
 
 
  unsigned char xdata ucADC_ChannelBuf[] = {0x00,0x08};
  unsigned int xdata uiADC_SampleBuf[sizeof(ucADC_ChannelBuf)][32];
 
  unsigned int uiBandgap_Voltage,uiVDD_Voltage;
 
 void ADC_Bypass (void)							 
 {
 unsigned char ozc;
 for (ozc=0;ozc<0x03;ozc++)
 {
  ADCF = 0;
  ADCS = 1;
 while(ADCF == 0);
 }
 }
 
 void READ_BANDGAP()								 
 {												 
  unsigned char  BandgapHigh,BandgapLow;
  unsigned int Bandgap_Value;						 
 
  BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=0x01 ;EA=BIT_TMP;
 IAPAL = 0x0C;
 IAPAH = 0x00;
 IAPCN = 0x04;
  BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=0x01 ;EA=BIT_TMP;
 BandgapHigh = IAPFD;
 
 IAPAL = 0x0d;
 IAPAH = 0x00;
 IAPCN = 0x04;
  BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=0x01 ;EA=BIT_TMP;
 BandgapLow = IAPFD;
 BandgapLow = BandgapLow&0x0F;
  BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~0x01;EA=BIT_TMP;
 
 Bandgap_Value = ((unsigned int)BandgapHigh<<4)+BandgapLow;
 uiBandgap_Voltage = 3072UL*Bandgap_Value/0x0fff;
 }
 
 void ADC_Init(void)
 {
  ADCCON0|=0x08;ADCCON0&=0xF8;ADCCON1|=0x01;				 
 ADC_Bypass();
 READ_BANDGAP();
 
  ADCCON0&=0xF0;P1M1|=0x80;P1M2&=~0x80;AINDIDS=0x00;AINDIDS|=0x01;ADCCON1|=0x01;
  ADCCON0&=0xF0;ADCCON0|=0x01;P3M1|=0x01;P3M2&=~0x01;AINDIDS=0x00;AINDIDS|=0x02;ADCCON1|=0x01;
  ADCCON0&=0xF0;ADCCON0|=0x02;P0M1|=0x80;P0M2&=~0x80;AINDIDS=0x00;AINDIDS|=0x04;ADCCON1|=0x01;
  EADC = 1;
 }
 


 
 void ADC_ISR (void) interrupt 11
 {
 static unsigned char ucIndex=0,ucChannel=0;
  unsigned char ucAdcrh,ucAdclh;
 
  ADCF = 0;                                
 ucAdcrh = ADCRH;
 ucAdclh = ADCRL;
 
 uiADC_SampleBuf[ucChannel][ucIndex++] = ((unsigned int)ucAdcrh<<8) | ucAdclh;
 if ( ucIndex >= 32 ){
 ucIndex = 0;
 if ( ++ucChannel >= (sizeof(ucADC_ChannelBuf)/sizeof(ucADC_ChannelBuf[0])) )
 ucChannel = 0;
 ADCCON0 = (ADCCON0&0xF0) | ucADC_ChannelBuf[ucChannel];
 }
 }
 
 
