#ifndef	_STC15Fxxxx_H
#define	_STC15Fxxxx_H

#include "config.h"
#include "compiler.h"

/*  BYTE Registers  */
#define P0_ADDR 0x80
#define SP_ADDR 0x81
#define DPL_ADDR 0x82
#define DPH_ADDR 0x83
#define S4CON_ADDR 0x84
#define S4BUF_ADDR 0x85
#define PCON_ADDR 0x87
SFR(P0,P0_ADDR);
SFR(SP,SP_ADDR);
SFR(DPL,DPL_ADDR);
SFR(DPH,DPH_ADDR);
SFR(S4CON,S4CON_ADDR);
SFR(S4BUF,S4BUF_ADDR);
SFR(PCON,PCON_ADDR);

#define TCON_ADDR 0x88
#define TMOD_ADDR 0x89
#define TL0_ADDR 0x8A
#define TL1_ADDR 0x8B
#define TH0_ADDR 0x8C
#define TH1_ADDR 0x8D
#define AUXR_ADDR 0x8E
#define WAKE_CLKO_ADDR 0x8F
#define INT_CLKO_ADDR 0x8F
#define  AUXR2_ADDR 0x8F
SFR(TCON,TCON_ADDR);
SFR(TMOD,TMOD_ADDR);
SFR(TL0,TL0_ADDR);
SFR(TL1,TL1_ADDR);
SFR(TH0,TH0_ADDR);
SFR(TH1,TH1_ADDR);
SFR(AUXR,AUXR_ADDR);
SFR(WAKE_CLKO,WAKE_CLKO_ADDR);
SFR(INT_CLKO,INT_CLKO_ADDR);
SFR(AUXR2,AUXR2_ADDR);

#define RL_TL0_ADDR 0x8A
#define RL_TL1_ADDR 0x8B
#define RL_TH0_ADDR 0x8C
#define RL_TH1_ADDR 0x8D
SFR(RL_TL0,RL_TL0_ADDR);
SFR(RL_TL1,RL_TL1_ADDR);
SFR(RL_TH0,RL_TH0_ADDR);
SFR(RL_TH1,RL_TH1_ADDR);


#define P1_ADDR 0x90
#define P1M1_ADDR 0x91	//P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull		实际上1T的都一样
#define P1M0_ADDR 0x92	//					=10--->pure input,	11--->open drain
#define P0M1_ADDR 0x93	//P0M1.n,P0M0.n 	=00--->Standard,	01--->push-pull
#define P0M0_ADDR 0x94	//					=10--->pure input,	11--->open drain
#define P2M1_ADDR 0x95	//P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
#define P2M0_ADDR 0x96	//					=10--->pure input,	11--->open drain
#define CLK_DIV_ADDR 0x97
#define PCON2_ADDR 0x97
SFR(P1,P1_ADDR);
SFR(P1M1,P1M1_ADDR);
SFR(P1M0,P1M0_ADDR);
SFR(P0M1,P0M1_ADDR);
SFR(P0M0,P0M0_ADDR);
SFR(P2M1,P2M1_ADDR);
SFR(P2M0,P2M0_ADDR);
SFR(CLK_DIV,CLK_DIV_ADDR);
SFR(PCON2,PCON2_ADDR);

#define SCON_ADDR 0x98
#define SBUF_ADDR 0x99
#define S2CON_ADDR 0x9A	//
#define S2BUF_ADDR 0x9B	//
#define P1ASF_ADDR 0x9D	//只写，模拟输入(AD或LVD)选择
SFR(SCON,SCON_ADDR);
SFR(SBUF,SBUF_ADDR);
SFR(S2CON,S2CON_ADDR);
SFR(S2BUF,S2BUF_ADDR);
SFR(P1ASF,P1ASF_ADDR);

#define P2_ADDR 0xA0
#define BUS_SPEED_ADDR 0xA1
#define AUXR1_ADDR 0xA2
#define P_SW1_ADDR 0xA2
SFR(P2,P2_ADDR);
SFR(BUS_SPEED,BUS_SPEED_ADDR);
SFR(AUXR1,AUXR1_ADDR);
SFR(P_SW1,P_SW1_ADDR);

#define IE_ADDR 0xA8
#define SADDR_ADDR 0xA9
#define WKTCL_ADDR 0xAA	//唤醒定时器低字节
#define WKTCH_ADDR 0xAB	//唤醒定时器高字节
#define S3CON_ADDR 0xAC
#define S3BUF_ADDR 0xAD
#define IE2_ADDR 0xAF	//STC12C5A60S2系列
SFR(IE,IE_ADDR);
SFR(SADDR,SADDR_ADDR);
SFR(WKTCL,WKTCL_ADDR);
SFR(WKTCH,WKTCH_ADDR);
SFR(S3CON,S3CON_ADDR);
SFR(S3BUF,S3BUF_ADDR);
SFR(IE2,IE2_ADDR);

#define P3_ADDR 0xB0
#define P3M1_ADDR 0xB1	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
#define P3M0_ADDR 0xB2	//					=10--->pure input,	11--->open drain
#define P4M1_ADDR 0xB3	//P4M1.n,P4M0.n 	=00--->Standard,	01--->push-pull
#define P4M0_ADDR 0xB4	//					=10--->pure input,	11--->open drain
#define IP2_ADDR 0xB5	//STC12C5A60S2系列
#define IPH2_ADDR 0xB6	//STC12C5A60S2系列
#define IPH_ADDR 0xB7
SFR(P3,P3_ADDR);
SFR(P3M1,P3M1_ADDR);
SFR(P3M0,P3M0_ADDR);
SFR(P4M1,P4M1_ADDR);
SFR(P4M0,P4M0_ADDR);
SFR(IP2,IP2_ADDR);
SFR(IPH2,IPH2_ADDR);
SFR(IPH,IPH_ADDR);

#define IP_ADDR 0xB8
#define SADEN_ADDR 0xB9
#define P_SW2_ADDR 0xBA
#define ADC_CONTR_ADDR 0xBC	//带AD系列
#define ADC_RES_ADDR 0xBD	//带AD系列
#define ADC_RESL_ADDR 0xBE	//带AD系列
SFR(IP,IP_ADDR);
SFR(SADEN,SADEN_ADDR);
SFR(P_SW2,P_SW2_ADDR);
SFR(ADC_CONTR,ADC_CONTR_ADDR);
SFR(ADC_RES,ADC_RES_ADDR);
SFR(ADC_RESL,ADC_RESL_ADDR);

#define P4_ADDR 0xC0
#define WDT_CONTR_ADDR 0xC1
#define IAP_DATA_ADDR 0xC2
#define IAP_ADDRH_ADDR 0xC3
#define IAP_ADDRL_ADDR 0xC4
#define IAP_CMD_ADDR 0xC5
#define IAP_TRIG_ADDR 0xC6
#define IAP_CONTR_ADDR 0xC7
SFR(P4,P4_ADDR);
SFR(WDT_CONTR,WDT_CONTR_ADDR);
SFR(IAP_DATA,IAP_DATA_ADDR);
SFR(IAP_ADDRH,IAP_ADDRH_ADDR);
SFR(IAP_ADDRL,IAP_ADDRL_ADDR);
SFR(IAP_CMD,IAP_CMD_ADDR);
SFR(IAP_TRIG,IAP_TRIG_ADDR);
SFR(IAP_CONTR,IAP_CONTR_ADDR);

#define ISP_DATA_ADDR 0xC2
#define ISP_ADDRH_ADDR 0xC3
#define ISP_ADDRL_ADDR 0xC4
#define ISP_CMD_ADDR 0xC5
#define ISP_TRIG_ADDR 0xC6
#define ISP_CONTR_ADDR 0xC7
SFR(ISP_DATA,ISP_DATA_ADDR);
SFR(ISP_ADDRH,ISP_ADDRH_ADDR);
SFR(ISP_ADDRL,ISP_ADDRL_ADDR);
SFR(ISP_CMD,ISP_CMD_ADDR);
SFR(ISP_TRIG,ISP_TRIG_ADDR);
SFR(ISP_CONTR,ISP_CONTR_ADDR);

#define P5_ADDR 0xC8	//
#define P5M1_ADDR 0xC9	//	P5M1.n,P5M0.n 	=00--->Standard,	01--->push-pull
#define P5M0_ADDR 0xCA	//					=10--->pure input,	11--->open drain
#define P6M1_ADDR 0xCB	//	P5M1.n,P5M0.n 	=00--->Standard,	01--->push-pull
#define P6M0_ADDR 0xCC	//					=10--->pure input,	11--->open drain
#define SPSTAT_ADDR 0xCD	//
#define SPCTL_ADDR 0xCE	//
#define SPDAT_ADDR 0xCF	//
SFR(P5,P5_ADDR);
SFR(P5M1,P5M1_ADDR);
SFR(P5M0,P5M0_ADDR);
SFR(P6M1,P6M1_ADDR);
SFR(P6M0,P6M0_ADDR);
SFR(SPSTAT,SPSTAT_ADDR);
SFR(SPCTL,SPCTL_ADDR);
SFR(SPDAT,SPDAT_ADDR);

#define PSW_ADDR 0xD0
#define T4T3M_ADDR 0xD1
#define T4H_ADDR 0xD2
#define T4L_ADDR 0xD3
#define T3H_ADDR 0xD4
#define T3L_ADDR 0xD5
#define T2H_ADDR 0xD6
#define T2L_ADDR 0xD7
SFR(PSW,PSW_ADDR);
SFR(T4T3M,T4T3M_ADDR);
SFR(T4H,T4H_ADDR);
SFR(T4L,T4L_ADDR);
SFR(T3H,T3H_ADDR);
SFR(T3L,T3L_ADDR);
SFR(T2H,T2H_ADDR);
SFR(T2L,T2L_ADDR);

#define TH4_ADDR 0xD2
#define TL4_ADDR 0xD3
#define TH3_ADDR 0xD4
#define TL3_ADDR 0xD5
#define TH2_ADDR 0xD6
#define TL2_ADDR 0xD7
SFR(TH4,TH4_ADDR);
SFR(TL4,TL4_ADDR);
SFR(TH3,TH3_ADDR);
SFR(TL3,TL3_ADDR);
SFR(TH2,TH2_ADDR);
SFR(TL2,TL2_ADDR);

#define RL_T4H_ADDR 0xD2
#define RL_T4L_ADDR 0xD3
#define RL_T3H_ADDR 0xD4
#define RL_T3L_ADDR 0xD5
#define RL_T2H_ADDR 0xD6
#define RL_T2L_ADDR 0xD7
SFR(RL_T4H,RL_T4H_ADDR);
SFR(RL_T4L,RL_T4L_ADDR);
SFR(RL_T3H,RL_T3H_ADDR);
SFR(RL_T3L,RL_T3L_ADDR);
SFR(RL_T2H,RL_T2H_ADDR);
SFR(RL_T2L,RL_T2L_ADDR);

#define CCON_ADDR 0xD8	//
#define CMOD_ADDR 0xD9	//
#define CCAPM0_ADDR 0xDA	//PCA模块0的工作模式寄存器。
#define CCAPM1_ADDR 0xDB	//PCA模块1的工作模式寄存器。
#define CCAPM2_ADDR 0xDC	//PCA模块2的工作模式寄存器。
SFR(CCON,CCON_ADDR);
SFR(CMOD,CMOD_ADDR);
SFR(CCAPM0,CCAPM0_ADDR);
SFR(CCAPM1,CCAPM1_ADDR);
SFR(CCAPM2,CCAPM2_ADDR);

#define ACC_ADDR 0xE0
#define P7M1_ADDR 0xE1
#define P7M0_ADDR 0xE2
SFR(ACC,ACC_ADDR);
SFR(P7M1,P7M1_ADDR);
SFR(P7M0,P7M0_ADDR);

#define P6_ADDR 0xE8
#define CL_ADDR 0xE9	//
#define CCAP0L_ADDR 0xEA	//PCA模块0的捕捉/比较寄存器低8位。
#define CCAP1L_ADDR 0xEB	//PCA模块1的捕捉/比较寄存器低8位。
#define CCAP2L_ADDR 0xEC	//PCA模块2的捕捉/比较寄存器低8位。
SFR(P6,P6_ADDR);
SFR(CL,CL_ADDR);
SFR(CCAP0L,CCAP0L_ADDR);
SFR(CCAP1L,CCAP1L_ADDR);
SFR(CCAP2L,CCAP2L_ADDR);

#define B_ADDR 0xF0
#define PCA_PWM0_ADDR 0xF2	//PCA模块0 PWM寄存器。
#define PCA_PWM1_ADDR 0xF3	//PCA模块1 PWM寄存器。
#define PCA_PWM2_ADDR 0xF4	//PCA模块2 PWM寄存器。
SFR(B,B_ADDR);
SFR(PCA_PWM0,PCA_PWM0_ADDR);
SFR(PCA_PWM1,PCA_PWM1_ADDR);
SFR(PCA_PWM2,PCA_PWM2_ADDR);

#define P7_ADDR 0xF8
#define CH_ADDR 0xF9
#define CCAP0H_ADDR 0xFA		//PCA模块0的捕捉/比较寄存器高8位。
#define CCAP1H_ADDR 0xFB		//PCA模块1的捕捉/比较寄存器高8位。
#define CCAP2H_ADDR 0xFC		//PCA模块2的捕捉/比较寄存器高8位。
SFR(P7,P7_ADDR);
SFR(CH,CH_ADDR);
SFR(CCAP0H,CCAP0H_ADDR);
SFR(CCAP1H,CCAP1H_ADDR);
SFR(CCAP2H,CCAP2H_ADDR);


/*  BIT Registers  */
/*  PSW   */
SBIT(CY,PSW_ADDR,7);
SBIT(AC,PSW_ADDR,6);
SBIT(F0,PSW_ADDR,5);
SBIT(RS1,PSW_ADDR,4);
SBIT(RS0,PSW_ADDR,3);
SBIT(OV,PSW_ADDR,2);
SBIT(F1,PSW_ADDR,1);
SBIT(P,PSW_ADDR,0);

/*  TCON  */
SBIT(TF1,TCON_ADDR,7);	//定时器1溢出中断标志位
SBIT(TR1,TCON_ADDR,6);	//定时器1运行控制位
SBIT(TF0,TCON_ADDR,5);	//定时器0溢出中断标志位
SBIT(TR0,TCON_ADDR,4);	//定时器0运行控制位
SBIT(IE1,TCON_ADDR,3);	//外中断1标志位
SBIT(IT1,TCON_ADDR,2);	//外中断1信号方式控制位，1：下降沿中断，0：上升下降均中断。
SBIT(IE0,TCON_ADDR,1);	//外中断0标志位
SBIT(IT0,TCON_ADDR,0);	//外中断0信号方式控制位，1：下降沿中断，0：上升下降均中断。

/*  P0  */
SBIT(P00,P0_ADDR,0);
SBIT(P01,P0_ADDR,1);
SBIT(P02,P0_ADDR,2);
SBIT(P03,P0_ADDR,3);
SBIT(P04,P0_ADDR,4);
SBIT(P05,P0_ADDR,5);
SBIT(P06,P0_ADDR,6);
SBIT(P07,P0_ADDR,7);

/*  P1  */
SBIT(P10,P1_ADDR,0);
SBIT(P11,P1_ADDR,1);
SBIT(P12,P1_ADDR,2);
SBIT(P13,P1_ADDR,3);
SBIT(P14,P1_ADDR,4);
SBIT(P15,P1_ADDR,5);
SBIT(P16,P1_ADDR,6);
SBIT(P17,P1_ADDR,7);

SBIT(RXD2,P1_ADDR,0);
SBIT(TXD2,P1_ADDR,1);
SBIT(CCP1,P1_ADDR,0);
SBIT(CCP0,P1_ADDR,1);
SBIT(SPI_SS,P1_ADDR,2);
SBIT(SPI_MOSI,P1_ADDR,3);
SBIT(SPI_MISO,P1_ADDR,4);
SBIT(SPI_SCLK,P1_ADDR,5);

SBIT(SPI_SS_2,P2_ADDR,4);
SBIT(SPI_MOSI_2,P2_ADDR,3);
SBIT(SPI_MISO_2,P2_ADDR,2);
SBIT(SPI_SCLK_2,P2_ADDR,1);

SBIT(SPI_SS_3,P5_ADDR,4);
SBIT(SPI_MOSI_3,P4_ADDR,0);
SBIT(SPI_MISO_3,P4_ADDR,1);
SBIT(SPI_SCLK_3,P4_ADDR,3);

/*  P2  */
SBIT(P20,P2_ADDR,0);
SBIT(P21,P2_ADDR,1);
SBIT(P22,P2_ADDR,2);
SBIT(P23,P2_ADDR,3);
SBIT(P24,P2_ADDR,4);
SBIT(P25,P2_ADDR,5);
SBIT(P26,P2_ADDR,6);
SBIT(P27,P2_ADDR,7);

/*  P3  */
SBIT(P30,P3_ADDR,0);
SBIT(P31,P3_ADDR,1);
SBIT(P32,P3_ADDR,2);
SBIT(P33,P3_ADDR,3);
SBIT(P34,P3_ADDR,4);
SBIT(P35,P3_ADDR,5);
SBIT(P36,P3_ADDR,6);
SBIT(P37,P3_ADDR,7);

SBIT(RXD,P3_ADDR,0);
SBIT(TXD,P3_ADDR,1);
SBIT(INT0,P3_ADDR,2);
SBIT(INT1,P3_ADDR,3);
SBIT(T0,P3_ADDR,4);
SBIT(T1,P3_ADDR,5);
SBIT(WR,P3_ADDR,6);
SBIT(RD,P3_ADDR,7);
SBIT(CCP2,P3_ADDR,7);

SBIT(CLKOUT0,P3_ADDR,5);
SBIT(CLKOUT1,P3_ADDR,4);

/*  P4  */
SBIT(P40,P4_ADDR,0);
SBIT(P41,P4_ADDR,1);
SBIT(P42,P4_ADDR,2);
SBIT(P43,P4_ADDR,3);
SBIT(P44,P4_ADDR,4);
SBIT(P45,P4_ADDR,5);
SBIT(P46,P4_ADDR,6);
SBIT(P47,P4_ADDR,7);

/*  P5  */
SBIT(P50,P5_ADDR,0);
SBIT(P51,P5_ADDR,1);
SBIT(P52,P5_ADDR,2);
SBIT(P53,P5_ADDR,3);
SBIT(P54,P5_ADDR,4);
SBIT(P55,P5_ADDR,5);
SBIT(P56,P5_ADDR,6);
SBIT(P57,P5_ADDR,7);

/*  SCON  */
SBIT(SM0,SCON_ADDR,7);	//SM0/FE		SM0 SM1 = 00 ~ 11: 方式0~3
SBIT(SM1,SCON_ADDR,6);	//
SBIT(SM2,SCON_ADDR,5);	//多机通讯
SBIT(REN,SCON_ADDR,4);	//接收允许
SBIT(TB8,SCON_ADDR,3);	//发送数据第8位
SBIT(RB8,SCON_ADDR,2);	//接收数据第8位
SBIT(TI,SCON_ADDR,1);	//发送中断标志位
SBIT(RI,SCON_ADDR,0);	//接收中断标志位

/*  IE   */
SBIT(EA,IE_ADDR,7);	//中断允许总控制位
SBIT(ELVD,IE_ADDR,6);	//低压监测中断允许位
SBIT(EADC,IE_ADDR,5);	//ADC 中断 允许位
SBIT(ES,IE_ADDR,4);	//串行中断 允许控制位
SBIT(ET1,IE_ADDR,3);	//定时中断1允许控制位
SBIT(EX1,IE_ADDR,2);	//外部中断1允许控制位
SBIT(ET0,IE_ADDR,1);	//定时中断0允许控制位
SBIT(EX0,IE_ADDR,0);	//外部中断0允许控制位


/*  IP   */ 
/*
SBIT(PPCA,IP_ADDR,7);	//PCA 中断 优先级设定位
SBIT(PLVD,IP_ADDR,6); 	//低压中断 优先级设定位
SBIT(PADC,IP_ADDR,5); 	//ADC 中断 优先级设定位
SBIT(PS,IP_ADDR,4);	//串行中断0优先级设定位
SBIT(PT1,IP_ADDR,3);	//定时中断1优先级设定位
SBIT(PX1,IP_ADDR,2);	//外部中断1优先级设定位
SBIT(PT0,IP_ADDR,1);	//定时中断0优先级设定位
SBIT(PX0,IP_ADDR,0);	//外部中断0优先级设定位
*/

SBIT(ACC0,ACC_ADDR,0);
SBIT(ACC1,ACC_ADDR,1);
SBIT(ACC2,ACC_ADDR,2);
SBIT(ACC3,ACC_ADDR,3);
SBIT(ACC4,ACC_ADDR,4);
SBIT(ACC5,ACC_ADDR,5);
SBIT(ACC6,ACC_ADDR,6);
SBIT(ACC7,ACC_ADDR,7);

SBIT(B0,B_ADDR,0);
SBIT(B1,B_ADDR,1);
SBIT(B2,B_ADDR,2);
SBIT(B3,B_ADDR,3);
SBIT(B4,B_ADDR,4);
SBIT(B5,B_ADDR,5);
SBIT(B6,B_ADDR,6);
SBIT(B7,B_ADDR,7);


//							7     6     5    4    3    2    1     0    Reset Value
//sfr IE2       at 0xAF		-     -     -    -    -    -   ESPI  ES2   0000,0000B	//Auxiliary Interrupt   
#define		SPI_INT_ENABLE()		IE2 |=  2	//允许SPI中断
#define		SPI_INT_DISABLE()		IE2 &= ~2	//允许SPI中断
#define		UART2_INT_ENABLE()		IE2 |=  1	//允许串口2中断
#define		UART2_INT_DISABLE()		IE2 &= ~1	//允许串口2中断

//                                          7     6     5    4    3    2    1    0    Reset Value
//sfr IP      at 0xB8 //中断优先级低位      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
//--------
SBIT(PPCA,IP_ADDR,7);	//PCA 模块中断优先级
SBIT(PLVD,IP_ADDR,6);	//低压监测中断优先级
SBIT(PADC,IP_ADDR,5);	//ADC 中断优先级
SBIT(PS,IP_ADDR,4);	//串行中断0优先级设定位
SBIT(PT1,IP_ADDR,3);	//定时中断1优先级设定位
SBIT(PX1,IP_ADDR,2);	//外部中断1优先级设定位
SBIT(PT0,IP_ADDR,1);	//定时中断0优先级设定位
SBIT(PX0,IP_ADDR,0);	//外部中断0优先级设定位

//                                           7      6      5     4     3     2    1     0        Reset Value
//sfr IPH   at 0xB7 //中断优先级高位       PPCAH  PLVDH  PADCH  PSH  PT1H  PX1H  PT0H  PX0H   0000,0000
//sfr IP2   at 0xB5 //                       -      -      -     -     -     -   PSPI   PS2   xxxx,xx00
//sfr IPH2  at 0xB6 //                       -      -      -     -     -     -   PSPIH  PS2H  xxxx,xx00
#define		PPCAH	0x80
#define		PLVDH	0x40
#define		PADCH	0x20
#define		PSH		0x10
#define		PT1H	0x08
#define		PX1H	0x04
#define		PT0H	0x02
#define		PX0H	0x01

#define		PCA_InterruptFirst()	PPCA = 1
#define		LVD_InterruptFirst()	PLVD = 1
#define		ADC_InterruptFirst()	PADC = 1
#define		UART1_InterruptFirst()	PS   = 1
#define		Timer1_InterruptFirst()	PT1  = 1
#define		INT1_InterruptFirst()	PX1  = 1
#define		Timer0_InterruptFirst()	PT0  = 1
#define		INT0_InterruptFirst()	PX0  = 1



/*************************************************************************************************/
#define		S1_DoubleRate()		PCON |= 0x80
#define		S1_SHIFT()			SCON &= 0x3f
#define		S1_8bit()			SCON |= 0x40
#define		S1_9bit()			SCON |= 0xc0
#define		S1_RX_Enable()		SCON |= 0x10
#define		S1_USE_P30P31()		P_SW1 &=  ~0xc0					//UART1 使用P30 P31口	默认
#define		S1_USE_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 使用P36 P37口
#define		S1_USE_P16P17()		P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 使用P16 P17口
#define		S1_TXD_RXD_SHORT()	PCON2 |= (1<<4)	//将TXD与RXD连接中继输出
#define		S1_TXD_RXD_OPEN()	PCON2 |= (1<<4)	//将TXD与RXD连接中继断开	默认
#define S1_BRT_UseTimer2()	AUXR |= 1;
#define S1_BRT_UseTimer1()	AUXR &= ~1;

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S2CON at 0x9A		S2SM0    -    S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      00000000B		 //S2 Control

#define		S2_8bit()			S2CON &= ~(1<<7)		//串口2模式0，8位UART，波特率 = 定时器2的溢出率 / 4
#define		S2_9bit()			S2CON |=  (1<<7)		//串口2模式1，9位UART，波特率 = 定时器2的溢出率 / 4
#define		S2_RX_Enable()		S2CON |=  (1<<4)		//允许串2接收

#define		S2_MODE0()			S2CON &= ~(1<<7)		//串口2模式0，8位UART，波特率 = 定时器2的溢出率 / 4
#define		S2_MODE1()			S2CON |=  (1<<7)		//串口2模式1，9位UART，波特率 = 定时器2的溢出率 / 4
#define		S2_RX_EN()			S2CON |=  (1<<4)							//允许串2接收
#define		S2_RX_Disable()		S2CON &= ~(1<<4)							//禁止串2接收
#define		TI2					(S2CON & 2) != 0
#define		RI2					(S2CON & 1) != 0
#define		SET_TI2()			S2CON |=  2
#define		CLR_TI2()			S2CON &= ~2
#define		CLR_RI2()			S2CON &= ~1
#define		S2TB8_SET()			S2CON |=  8
#define		S2TB8_CLR()			S2CON &= ~8
#define		S2_Int_en()			IE2   |=  1		//串口2允许中断
#define		S2_USE_P10P11()		P_SW2 &= ~1		//UART2 使用P1口	默认
#define		S2_USE_P46P47()		P_SW2 |=  1		//UART2 使用P4口

#define		S3_USE_P00P01()		P_SW2 &= ~2		//UART3 使用P0口	默认
#define		S3_USE_P50P51()		P_SW2 |=  2		//UART3 使用P5口
#define		S4_USE_P02P03()		P_SW2 &= ~4		//UART4 使用P0口	默认
#define		S4_USE_P52P53()		P_SW2 |=  4		//UART4 使用P5口


/**********************************************************/

#define		Timer0_16bitAutoReload()	TMOD &= ~0x03					//16位自动重装
#define		Timer0_16bit()				TMOD  = (TMOD & ~0x03) | 0x01	//16位
#define		Timer0_8bitAutoReload()		TMOD  = (TMOD & ~0x03) | 0x02	//8位自动重装
#define		Timer0_16bitAutoRL_NoMask()	TMOD |=  0x03		//16位自动重装不可屏蔽中断
#define		Timer0_AsCounterP32()		TMOD |=  4		//时器0用做计数器
#define		Timer0_AsTimer()			TMOD &= ~4		//时器0用做定时器
#define		Timer0_ExtControlP34()		TMOD |=  4		//时器0由外部INT0高电平允许定时计数
#define Timer0_Run()	 			TR0 = 1				//允许定时器0计数
#define Timer0_Stop()	 			TR0 = 0				//禁止定时器0计数
#define Timer0_InterruptEnable()	ET0 = 1				//允许Timer1中断.
#define Timer0_InterruptDisable()	ET0 = 0				//禁止Timer1中断.

#define		Timer1_16bitAutoReload()	TMOD &= ~0x30					//16位自动重装
#define		Timer1_16bit()				TMOD  = (TMOD & ~0x30) | 0x10	//16位
#define		Timer1_8bitAutoReload()		TMOD  = (TMOD & ~0x30) | 0x20	//8位自动重装
#define		Timer1_16bitAutoRL_NoMask()	TMOD |=  0x30		//16位自动重装不可屏蔽中断
#define		Timer1_AsCounterP33()		TMOD |=  (1<<6)		//时器1用做计数器
#define		Timer1_AsTimer()			TMOD &= ~(1<<6)		//时器1用做定时器
#define		Timer1_ExtControlP35()		TMOD |=  (1<<7)		//时器1由外部INT1高电平允许定时计数
#define Timer1_Run()	 			TR1 = 1				//允许定时器1计数
#define Timer1_Stop()	 			TR1 = 0				//禁止定时器1计数
#define Timer1_InterruptEnable()	ET1 = 1				//允许Timer1中断.
#define Timer1_InterruptDisable()	ET1 = 0				//禁止Timer1中断.


//						   7     6       5      4     3      2      1      0    Reset Value
//sfr AUXR  at 0x8E		T0x12 T1x12 UART_M0x6  BRTR S2SMOD BRTx12 EXTRAM S1BRS  0000,0000	//Auxiliary Register 

#define Timer0_1T()					AUXR |= (1<<7)	//Timer0 clodk = fo
#define Timer0_12T()				AUXR &= ~(1<<7)	//Timer0 clodk = fo/12	12分频,	default
#define Timer1_1T()					AUXR |= (1<<6)	//Timer1 clodk = fo
#define Timer1_12T()				AUXR &= ~(1<<6)	//Timer1 clodk = fo/12	12分频,	default
#define		S1_M0x6()					AUXR |=  (1<<5)	//UART Mode0 Speed is 6x Standard
#define		S1_M0x1()					AUXR &= ~(1<<5)	//default,	UART Mode0 Speed is Standard
#define Timer2_Run()	 			AUXR |= (1<<4)	//允许定时器2计数
#define Timer2_Stop()	 			AUXR &= ~(1<<4)	//禁止定时器2计数
#define		Timer2_AsCounterP31()		AUXR |=  (1<<3)	//时器2用做计数器
#define		Timer2_AsTimer()			AUXR &= ~(1<<3)	//时器2用做定时器
#define Timer2_1T()					AUXR |= (1<<2)	//Timer0 clodk = fo
#define Timer2_12T()				AUXR &= ~(1<<2)	//Timer0 clodk = fo/12	12分频,	default
#define Timer2_InterruptEnable()	IE2  |= (1<<2)	//允许Timer2中断.
#define Timer2_InterruptDisable()	IE2 &= ~(1<<2)	//禁止Timer2中断.

#define ExternalRAM_enable()		AUXR |= 2		//允许外部XRAM，禁止使用内部1024RAM
#define InternalRAM_enable()		AUXR &= ~2		//禁止外部XRAM，允许使用内部1024RAM

#define		T0_pulseP34_enable()		AUXR2 |=  1		//允许 T0 溢出脉冲在T0(P3.5)脚输出，Fck0 = 1/2 T0 溢出率，T0可以1T或12T。
#define		T0_pulseP34_disable()		AUXR2 &= ~1
#define		T1_pulseP35_enable()		AUXR2 |=  2		//允许 T1 溢出脉冲在T1(P3.4)脚输出，Fck1 = 1/2 T1 溢出率，T1可以1T或12T。
#define		T1_pulseP35_disable()		AUXR2 &= ~2
#define		T2_pulseP30_enable()		AUXR2 |=  4		//允许 T2 溢出脉冲在T1(P3.0)脚输出，Fck2 = 1/2 T2 溢出率，T2可以1T或12T。
#define		T2_pulseP30_disable()		AUXR2 &= ~4

#define		T0_pulseP35(n)		ET0=0,Timer0_AsTimer(),Timer0_1T(),Timer0_16bitAutoReload(),TH0=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL0=(65536-(n/2+MAIN_Fosc/2)/(n))%256,AUXR2 |= bit0,TR0=1		//fx=fosc/(2*M)/n,M=1 or M=12
#define		T1_pulseP34(n)		ET1=0,Timer1_AsTimer(),Timer1_1T(),Timer1_16bitAutoReload(),TH1=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL1=(65536-(n/2+MAIN_Fosc/2)/(n))%256,AUXR2 |= bit1,TR1=1		//fx=fosc/(2*M)/n,M=1 or M=12
#define		T2_pulseP30(n)		Timer2_InterruptDisable(),Timer2_AsTimer(),Timer2_1T(),TH2=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL2=(65536-(n/2+MAIN_Fosc/2)/(n))%256,AUXR2 |= bit2,Timer2_Run()	//fx=fosc/(2*M)/n,M=1 or M=12

#define		Timer0_Load(n)		TH0 = (n) / 256,	TL0 = (n) % 256
#define		Timer1_Load(n)		TH1 = (n) / 256,	TL1 = (n) % 256
#define		Timer2_Load(n)		TH2 = (n) / 256,	TL2 = (n) % 256

#define		Timer0_Load_us(n)	TH0=(65536-MainFosc_KHZ*(n)/1000)/256,TL0=(65536-MainFosc_KHZ*(n)/1000)%256
#define		Timer1_Load_us(n)	TH1=(65536-MainFosc_KHZ*(n)/1000)/256,TL1=(65536-MainFosc_KHZ*(n)/1000)%256
#define		Timer2_Load_us(n)	TH2=(65536-MainFosc_KHZ*(n)/1000)/256,TL2=(65536-MainFosc_KHZ*(n)/1000)%256


//sfr WDT_CONTR at 0xC1 //Watch-Dog-Timer Control register
//                                      7     6     5      4       3      2   1   0     Reset Value
//                                  WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
#define D_WDT_FLAG			(1<<7)
#define D_EN_WDT			(1<<5)
#define D_CLR_WDT			(1<<4)	//auto clear
#define D_IDLE_WDT			(1<<3)	//WDT counter when Idle
#define D_WDT_SCALE_2		0
#define D_WDT_SCALE_4		1
#define D_WDT_SCALE_8		2		//T=393216*N/fo
#define D_WDT_SCALE_16		3
#define D_WDT_SCALE_32		4
#define D_WDT_SCALE_64		5
#define D_WDT_SCALE_128		6
#define D_WDT_SCALE_256		7

#define	WDT_reset(n)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (n)		//初始化WDT，喂狗


//						  7     6      5    4     3      2    1     0     Reset Value
//sfr PCON   at 0x87	SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control 
//SMOD		//串口双倍速
//SMOD0
#define		LVDF		(1<<5)	//P4.6低压检测标志
//POF
//GF1
//GF0
//#define D_PD		2		//set 1,power down mode
//#define D_IDLE		1		//set 1,idle mode
#define		MCU_IDLE()			PCON |= 1	//MCU 进入 IDLE 模式
#define		MCU_POWER_DOWN()	PCON |= 2	//MCU 进入 睡眠 模式


//sfr ISP_CMD   at 0xC5
#define		ISP_STANDBY()	ISP_CMD = 0		//ISP空闲命令（禁止）
#define		ISP_READ()		ISP_CMD = 1		//ISP读出命令
#define		ISP_WRITE()		ISP_CMD = 2		//ISP写入命令
#define		ISP_ERASE()		ISP_CMD = 3		//ISP擦除命令

//sfr ISP_TRIG  at 0xC6
#define ISP_TRIG()	ISP_TRIG = 0x5A,	ISP_TRIG = 0xA5		//ISP触发命令

//							  7    6    5      4    3    2    1     0    Reset Value
//sfr IAP_CONTR at 0xC7		IAPEN SWBS SWRST CFAIL  -   WT2  WT1   WT0   0000,x000	//IAP Control Register
#define ISP_EN			(1<<7)
#define ISP_SWBS		(1<<6)
#define ISP_SWRST		(1<<5)
#define ISP_CMD_FAIL	(1<<4)
#define ISP_WAIT_1MHZ	7
#define ISP_WAIT_2MHZ	6
#define ISP_WAIT_3MHZ	5
#define ISP_WAIT_6MHZ	4
#define ISP_WAIT_12MHZ	3
#define ISP_WAIT_20MHZ	2
#define ISP_WAIT_24MHZ	1
#define ISP_WAIT_30MHZ	0

#if (MAIN_Fosc >= 24000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_30MHZ
#elif (MAIN_Fosc >= 20000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_24MHZ
#elif (MAIN_Fosc >= 12000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_20MHZ
#elif (MAIN_Fosc >= 6000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_12MHZ
#elif (MAIN_Fosc >= 3000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_6MHZ
#elif (MAIN_Fosc >= 2000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_3MHZ
#elif (MAIN_Fosc >= 1000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_2MHZ
#else
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_1MHZ
#endif



/* ADC Register */
//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR at 0xBC		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	//AD 转换控制寄存器 
//sfr ADC_RES  at 0xBD		ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	  0000,0000	//A/D 转换结果高8位 
//sfr ADC_RESL at 0xBE												  ADCV.1 ADCV.0	  0000,0000	//A/D 转换结果低2位
//sfr ADC_CONTR  at 0xBC	//直接用MOV操作，不要用与或


//sfr SPCTL  at 0xCE	SPI控制寄存器
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00

#define	SPI_SSIG_None()		SPCTL |=  (1<<7)		//1: 忽略SS脚
#define	SPI_SSIG_Enable()	SPCTL &= ~(1<<7)		//0: SS脚用于决定主从机
#define	SPI_Enable()		SPCTL |=  (1<<6)		//1: 允许SPI
#define	SPI_Disable()		SPCTL &= ~(1<<6)		//0: 禁止SPI
#define	SPI_LSB_First()		SPCTL |=  (1<<5)		//1: LSB先发
#define	SPI_MSB_First()		SPCTL &= ~(1<<5)		//0: MSB先发
#define	SPI_Master()		SPCTL |=  (1<<4)		//1: 设为主机
#define	SPI_Slave()			SPCTL &= ~(1<<4)		//0: 设为从机
#define	SPI_SCLK_NormalH()	SPCTL |=  (1<<3)		//1: 空闲时SCLK为高电平
#define	SPI_SCLK_NormalL()	SPCTL &= ~(1<<3)		//0: 空闲时SCLK为低电平
#define	SPI_PhaseH()		SPCTL |=  (1<<2)		//1: 
#define	SPI_PhaseL()		SPCTL &= ~(1<<2)		//0: 
#define	SPI_Speed(n)		SPCTL = (SPCTL & ~3) | (n)	//设置速度,0 -- fosc/4,1 -- fosc/16,2 -- fosc/64,3 -- fosc/128

//sfr SPDAT  at 0xCF //SPI Data Register                                                     0000,0000
//sfr SPSTAT  at 0xCD	//SPI状态寄存器
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI传输完成标志。写入1清0。
#define	WCOL	0x40		//SPI写冲突标志。写入1清0。

#define		SPI_USE_P12P13P14P15()	AUXR1 &= 0x0c	//将SPI切换到P12(SS) P13(MOSI) P14(MISO) P15(SCLK)(上电默认)。
#define		SPI_USE_P24P23P22P21()	AUXR1 = (AUXR1 & ~0x0c) | 0x04	//将SPI切换到P24(SS) P23(MOSI) P22(MISO) P21(SCLK)。
#define		SPI_USE_P54P40P41P43()	AUXR1 = (AUXR1 & ~0x0c) | 0x08	//将SPI切换到P54(SS) P40(MOSI) P41(MISO) P43(SCLK)。


/*
;PCA_PWMn:    7       6     5   4   3   2     1       0
;			EBSn_1	EBSn_0	-	-	-	-	EPCnH	EPCnL
;B5-B2:		保留
;B1(EPCnH):	在PWM模式下，与CCAPnH组成9位数。
;B0(EPCnL):	在PWM模式下，与CCAPnL组成9位数。
*/
#define		PWM0_NORMAL()	PCA_PWM0 &= ~3				//PWM0正常输出(默认)
#define		PWM0_OUT_0()	PCA_PWM0 |=  3				//PWM0一直输出0
#define		PWM0_OUT_1()	PCA_PWM0 &= ~3,CCAP0H = 0;	//PWM0一直输出1

#define		PWM1_NORMAL()	PCA_PWM1 &= ~3				//PWM0正常输出(默认)
#define		PWM1_OUT_0()	PCA_PWM1 |=  3				//PWM0一直输出0
#define		PWM1_OUT_1()	PCA_PWM1 &= ~3,CCAP1H = 0;	//PWM1一直输出1

#define		PWM2_NORMAL()	PCA_PWM2 &= ~3				//PWM1正常输出(默认)
#define		PWM2_OUT_0()	PCA_PWM2 |=  3				//PWM2一直输出0
#define		PWM2_OUT_1()	PCA_PWM2 &= ~3,CCAP2H = 0;	//PWM2一直输出1


//						7     6     5     4     3     2     1     0     Reset Value
//sfr CCON   at 0xD8	CF    CR    -     -     -    CCF2  CCF1  CCF0   00xx,xx00	//PCA 控制寄存器。
SBIT(CCF0,CCON_ADDR,0);	//PCA 模块0中断标志，由硬件置位，必须由软件清0。
SBIT(CCF1,CCON_ADDR,1);	//PCA 模块1中断标志，由硬件置位，必须由软件清0。
SBIT(CCF2,CCON_ADDR,2);	//PCA 模块2中断标志，由硬件置位，必须由软件清0。
SBIT(CR,CCON_ADDR,6);	//1: 允许PCA计数器计数，必须由软件清0。
SBIT(CF,CCON_ADDR,7);	//PCA计数器溢出（CH，CL由FFFFH变为0000H）标志。PCA计数器溢出后由硬件置位，必须由软件清0。

//					 7     6     5     4     3     2     1     0    Reset Value
//sfr CMOD  at 0xD9	CIDL   -     -     -   CPS2   CPS1  CPS0  ECF   0xxx,0000	//PCA 工作模式寄存器。
#define PCA_IDLE_OFF()		CMOD |= (1<<7)				//IDLE状态PCA停止计数。
#define PCA_IDLE_ON()		CMOD &= ~(1<<7)				//IDLE状态PCA继续计数。
#define PCA_CLK_12T()		CMOD &= ~0x0E				//PCA计数脉冲选择外部晶振/12。	fosc/12
#define PCA_CLK_2T()		CMOD = (CMOD & ~0x0E) + 2	//PCA计数脉冲选择外部晶振/2。	fosc/2
#define PCA_CLK_T0()		CMOD = (CMOD & ~0x0E) + 4	//PCA计数脉冲选择Timer0中断，Timer0可通过AUXR寄存器设置成工作在12T或1T模式。
#define PCA_CLK_ECI()		CMOD = (CMOD & ~0x0E) + 6	//PCA计数脉冲选择从ECI/P3.4脚输入的外部时钟，最大 fosc/2。
#define PCA_CLK_1T()		CMOD = (CMOD & ~0x0E) + 8	//PCA计数脉冲选择外部晶振。		Fosc/1
#define PCA_CLK_4T()		CMOD = (CMOD & ~0x0E) + 10	//PCA计数脉冲选择外部晶振/4。	Fosc/4
#define PCA_CLK_6T()		CMOD = (CMOD & ~0x0E) + 12	//PCA计数脉冲选择外部晶振/6。	Fosc/6
#define PCA_CLK_8T()		CMOD = (CMOD & ~0x0E) + 14	//PCA计数脉冲选择外部晶振/8。	Fosc/8
#define PCA_INT_ENABLE()	CMOD |= 1					//PCA计数器溢出中断允许位，1---允许CF（CCON.7）产生中断。
#define PCA_INT_DISABLE()	CMOD &= ~1					//PCA计数器溢出中断禁止。

//					    7      6       5        4       3       2       1      0    Reset Value
//sfr AUXR1 at 0xA2	  S1_S1  S1_S0  CCP_S1   CCP_S0  SPI_S1   SPI_S0    -     DPS   0100,0000	//Auxiliary Register 1

#define		PCA_USE_P12P11P10P37()	AUXR1 &= ~0x30		//将PCA/PWM切换到P12(ECI) P11(CCP0) P10(CCP1) P37(CCP2)(上电默认)。
#define		PCA_USE_P34P35P36P37()	AUXR1 = (AUXR1 & ~0x30) | 0x10	//将PCA/PWM切换到P34(ECI) P35(CCP0) P36(CCP1) P37(CCP2)。
#define		PCA_USE_P24P25P26P27()	AUXR1 = (AUXR1 & ~0x30) | 0x20	//将PCA/PWM切换到P24(ECI) P25(CCP0) P26(CCP1) P27(CCP2)。

#define		DPS_SEL1()		AUXR1 |=  1		//1：选择DPTR1。
#define		DPS_SEL0()		AUXR1 &= ~1		//0：选择DPTR0(上电默认)。


/*									7     6      5      4     3     2     1     0     Reset Value
//sfr CCAPM0 at 0xDA	PWM 寄存器  -   ECOM0  CAPP0  CAPN0  MAT0  TOG0  PWM0  ECCF0   x000,0000	//PCA 模块0 
//sfr CCAPM1 at 0xDB	PWM 寄存器  -   ECOM1  CAPP1  CAPN1  MAT1  TOG1  PWM1  ECCF1   x000,0000	//PCA 模块1
//sfr CCAPM2 at 0xDC	PWM 寄存器  -   ECOM2  CAPP2  CAPN2  MAT2  TOG2  PWM2  ECCF2   x000,0000	//PCA 模块2
;ECOMn = 1:	允许比较功能。
;CAPPn = 1:	允许上升沿触发捕捉功能。
;CAPNn = 1:	允许下降沿触发捕捉功能。
;MATn  = 1:	当匹配情况发生时，允许CCON中的CCFn置位。
;TOGn  = 1:	当匹配情况发生时，CEXn将翻转。(CEX0/PCA0/PWM0/P3.7,CEX1/PCA1/PWM1/P3.5)
;PWMn  = 1:	将CEXn设置为PWM输出。
;ECCFn = 1:	允许CCON中的CCFn触发中断。
;ECOMn CAPPn CAPNn MATn TOGn PWMn ECCFn
;  0     0     0    0    0    0     0		00H	未启用任何功能。
;  x     1     0    0    0    0     x	 	20H	16位CEXn上升沿触发捕捉功能。
;  x     0     1    0    0    0     x	 	10H	16位CEXn下降沿触发捕捉功能。
;  x     1     1    0    0    0     x	 	30H	16位CEXn/PCAn边沿（上、下沿）触发捕捉功能。
;  1     0     0    1    0    0     x	 	48H	16位软件定时器。
;  1     0     0    1    1    0     x	 	4CH	16位高速脉冲输出。
;  1     0     0    0    0    1     0		42H	8位PWM。无中断
;  1     1     0    0    0    1     1		63H	8位PWM。低变高可产生中断
;  1     0     1    0    0    1     1		53H	8位PWM。高变低可产生中断
;  1     1     1    0    0    1     1		73H	8位PWM。低变高或高变低均可产生中断
;*******************************************************************
;*******************************************************************/
#define		PCA0_none()				CCAPM0 = 0
#define		PCA0_PWM(nbit)			CCAPM0 = 0x42,PCA_PWM0 = (PCA_PWM0 & 0x0c) | ((8-nbit)<<6)
#define		PCA0_PWM_rise_int(nbit) CCAPM0 = 0x63,PCA_PWM0 = (PCA_PWM0 & 0x0c) | ((8-nbit)<<6)
#define		PCA0_PWM_fall_int(nbit) CCAPM0 = 0x53,PCA_PWM0 = (PCA_PWM0 & 0x0c) | ((8-nbit)<<6)
#define		PCA0_PWM_edge_int(nbit) CCAPM0 = 0x73,PCA_PWM0 = (PCA_PWM0 & 0x0c) | ((8-nbit)<<6)
#define		PCA0_capture_rise()		CCAPM0 = (0x20 + 1)
#define		PCA0_capture_fall()		CCAPM0 = (0x10 + 1)
#define		PCA0_capture_edge()		CCAPM0 = (0x30 + 1)
#define		PCA0_16bit_Timer()		CCAPM0 = (0x48 + 1)
#define		PCA0_High_Pulse()		CCAPM0 = (0x4C + 1)

#define		PCA1_none()				CCAPM1 = 0
#define		PCA1_PWM(nbit)			CCAPM1 = 0x42,PCA_PWM1 = (PCA_PWM1 & 0x0c) | ((8-nbit)<<6)
#define		PCA1_PWM_rise_int(nbit) CCAPM1 = 0x63,PCA_PWM1 = (PCA_PWM1 & 0x0c) | ((8-nbit)<<6)
#define		PCA1_PWM_fall_int(nbit) CCAPM1 = 0x53,PCA_PWM1 = (PCA_PWM1 & 0x0c) | ((8-nbit)<<6)
#define		PCA1_PWM_edge_int(nbit) CCAPM1 = 0x73,PCA_PWM1 = (PCA_PWM1 & 0x0c) | ((8-nbit)<<6)
#define		PCA1_capture_rise()		CCAPM1 = (0x20 + 1)
#define		PCA1_capture_fall()		CCAPM1 = (0x10 + 1)
#define		PCA1_capture_edge()		CCAPM1 = (0x30 + 1)
#define		PCA1_16bit_Timer()		CCAPM1 = (0x48 + 1)
#define		PCA1_High_Pulse()		CCAPM1 = (0x4C + 1)

#define		PCA2_none()				CCAPM2 = 0
#define		PCA2_PWM(nbit)			CCAPM2 = 0x42,PCA_PWM2 = (PCA_PWM2 & 0x0c) | ((8-nbit)<<6)
#define		PCA2_PWM_rise_int(nbit) CCAPM2 = 0x63,PCA_PWM2 = (PCA_PWM2 & 0x0c) | ((8-nbit)<<6)
#define		PCA2_PWM_fall_int(nbit) CCAPM2 = 0x53,PCA_PWM2 = (PCA_PWM2 & 0x0c) | ((8-nbit)<<6)
#define		PCA2_PWM_edge_int(nbit) CCAPM2 = 0x73,PCA_PWM2 = (PCA_PWM2 & 0x0c) | ((8-nbit)<<6)
#define		PCA2_capture_rise()		CCAPM2 = (0x20 + 1)
#define		PCA2_capture_fall()		CCAPM2 = (0x10 + 1)
#define		PCA2_capture_edge()		CCAPM2 = (0x30 + 1)
#define		PCA2_16bit_Timer()		CCAPM2 = (0x48 + 1)
#define		PCA2_High_Pulse()		CCAPM2 = (0x4C + 1)

/* Above is STC additional SFR or change */




/**********************************************************/
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

/**********************************************************/
#define NOP1() NOP()
#define NOP2() NOP1(),NOP1()
#define NOP3() NOP2(),NOP1()
#define NOP4() NOP3(),NOP1()
#define NOP5() NOP4(),NOP1()
#define NOP6() NOP5(),NOP1()
#define NOP7() NOP6(),NOP1()
#define NOP8() NOP7(),NOP1()
#define NOP9() NOP8(),NOP1()
#define NOP10() NOP9(),NOP1()
#define NOP11() NOP10(),NOP1()
#define NOP12() NOP11(),NOP1()
#define NOP13() NOP12(),NOP1()
#define NOP14() NOP13(),NOP1()
#define NOP15() NOP14(),NOP1()
#define NOP16() NOP15(),NOP1()
#define NOP17() NOP16(),NOP1()
#define NOP18() NOP17(),NOP1()
#define NOP19() NOP18(),NOP1()
#define NOP20() NOP19(),NOP1()
#define NOP21() NOP20(),NOP1()
#define NOP22() NOP21(),NOP1()
#define NOP23() NOP22(),NOP1()
#define NOP24() NOP23(),NOP1()
#define NOP25() NOP24(),NOP1()
#define NOP26() NOP25(),NOP1()
#define NOP27() NOP26(),NOP1()
#define NOP28() NOP27(),NOP1()
#define NOP29() NOP28(),NOP1()
#define NOP30() NOP29(),NOP1()
#define NOP31() NOP30(),NOP1()
#define NOP32() NOP31(),NOP1()
#define NOP33() NOP32(),NOP1()
#define NOP34() NOP33(),NOP1()
#define NOP35() NOP34(),NOP1()
#define NOP36() NOP35(),NOP1()
#define NOP37() NOP36(),NOP1()
#define NOP38() NOP37(),NOP1()
#define NOP39() NOP38(),NOP1()
#define NOP40() NOP39(),NOP1()
// #define NOP(N) NOP##N()


/**********************************************/


/****************************************************************/


//sfr INT_CLKO at 0x8F	//附加的 SFR WAKE_CLKO (地址：0x8F)
/*
    7   6    5    4   3     2        1       0         Reset Value
    -  EX4  EX3  EX2  -   T2CLKO   T1CLKO  T0CLKO      0000,0000B
b6 -  EX4      : 外中断INT4允许
b5 -  EX3      : 外中断INT3允许
b4 -  EX2      : 外中断INT2允许
b2 - T1CLKO    : 允许 T2 溢出脉冲在P3.0脚输出，Fck1 = 1/2 T1 溢出率
b1 - T1CLKO    : 允许 T1 溢出脉冲在P3.4脚输出，Fck1 = 1/2 T1 溢出率
b0 - T0CLKO    : 允许 T0 溢出脉冲在P3.5脚输出，Fck0 = 1/2 T0 溢出率
*/

#define		LVD_InterruptEnable()		ELVD = 1
#define		LVD_InterruptDisable()		ELVD = 0


//sfr WKTCL at 0xAA	//STC11F\10和STC15系列 唤醒定时器低字节
//sfr WKTCH at 0xAB	//STC11F\10和STC15系列 唤醒定时器高字节
//	B7		B6	B5	B4	B3	B2	B1	B0		B7	B6	B5	B4	B3	B2	B1	B0
//	WKTEN				S11	S10	S9	S8		S7	S6	S5	S4	S3	S2	S1	S0	n * 560us
#define		WakeTimerDisable()		WKTCH &= 0x7f	//WKTEN = 0		禁止睡眠唤醒定时器
#define		WakeTimerSet(scale)		WKTCL = (scale) % 256,WKTCH = (scale) / 256 | 0x80	//WKTEN = 1	允许睡眠唤醒定时器


//sfr CLK_DIV at 0x97 //Clock Divder 系统时钟分频  -     -      -       -     -  CLKS2 CLKS1 CLKS0 xxxx,x000
#define		SYSTEM_CLK_1T()		CLK_DIV &= ~0x07	//default
#define		SYSTEM_CLK_2T()		CLK_DIV = (CLK_DIV & ~0x07) | 1
#define		SYSTEM_CLK_4T()		CLK_DIV = (CLK_DIV & ~0x07) | 2
#define		SYSTEM_CLK_8T()		CLK_DIV = (CLK_DIV & ~0x07) | 3
#define		SYSTEM_CLK_16T()	CLK_DIV = (CLK_DIV & ~0x07) | 4
#define		SYSTEM_CLK_32T()	CLK_DIV = (CLK_DIV & ~0x07) | 5
#define		SYSTEM_CLK_64T()	CLK_DIV = (CLK_DIV & ~0x07) | 6
#define		SYSTEM_CLK_128T()	CLK_DIV =  CLK_DIV          | 7

#define		MCLKO_P54_None()	CLK_DIV &= ~0xc0					//主时钟不输出
#define		MCLKO_P54_DIV1()	CLK_DIV  = (CLK_DIV & ~0xc0) | 0x40	//主时钟不分频输出
#define		MCLKO_P54_DIV2()	CLK_DIV  = (CLK_DIV & ~0xc0) | 0x80	//主时钟2分频输出
#define		MCLKO_P54_DIV4()	CLK_DIV  =  CLK_DIV | 0xc0			//主时钟4分频输出

#define		MCLKO_P34_None()	CLK_DIV &= ~0xc0					//主时钟不输出
#define		MCLKO_P34_DIV1()	CLK_DIV  = (CLK_DIV & ~0xc0) | 0x40	//主时钟不分频输出
#define		MCLKO_P34_DIV2()	CLK_DIV  = (CLK_DIV & ~0xc0) | 0x80	//主时钟2分频输出
#define		MCLKO_P34_DIV4()	CLK_DIV  =  CLK_DIV | 0xc0			//主时钟4分频输出

//sfr BUS_SPEED at 0xA1 //Stretch register      -   -  -  -   -   -  EXRTS1  EXRTSS0 xxxx,xx10
#define		BUS_SPEED_1T()	BUS_SPEED = 0
#define		BUS_SPEED_2T()	BUS_SPEED = 1
#define		BUS_SPEED_4T()	BUS_SPEED = 2
#define		BUS_SPEED_8T()	BUS_SPEED = 3

/*   interrupt vector */
#define		INT0_VECTOR		0
#define		TIMER0_VECTOR	1
#define		INT1_VECTOR		2
#define		TIMER1_VECTOR	3
#define		UART1_VECTOR	4
#define		ADC_VECTOR		5
#define		LVD_VECTOR		6
#define		PCA_VECTOR		7
#define		UART2_VECTOR	8
#define		SPI_VECTOR		9
#define		INT2_VECTOR		10
#define		INT3_VECTOR		11
#define		TIMER2_VECTOR	12
#define		INT4_VECTOR		16
#define		UART3_VECTOR	17
#define		UART4_VECTOR	18
#define		TIMER3_VECTOR	19
#define		TIMER4_VECTOR	20


#define	TRUE	1
#define	FALSE	0

//=============================================================

//========================================

#define	PolityLow			0	//低优先级中断
#define	PolityHigh			1	//高优先级中断

//========================================

#define		MCLKO_None	0
#define		MCLKO_DIV1	1
#define		MCLKO_DIV2	2
#define		MCLKO_DIV4	3

#define		ENABLE		1
#define		DISABLE		0

#define		STC15F_L2K08S2	8
#define		STC15F_L2K16S2	16
#define		STC15F_L2K24S2	24
#define		STC15F_L2K32S2	32
#define		STC15F_L2K40S2	40
#define		STC15F_L2K48S2	48
#define		STC15F_L2K56S2	56
#define		STC15F_L2K60S2	60
#define		IAP15F_L2K61S2	61

#endif
