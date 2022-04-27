/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef __APP_H_
#define __APP_H_

//========================================================================
//                                头文件
//========================================================================

#include "APP_AD_UART2.h"
#include "APP_AHT21.h"
#include "APP_EEPROM.h"
#include "APP_I2C_PS.h"
#include "APP_INT_UART1.h"
#include "APP_Lamp.h"
#include "APP_PCA.h"
#include "APP_PWM.h"
#include "APP_PWM15bit.h"
#include "APP_RTC.h"
#include "APP_SPI_PS.h"
#include "APP_WDT.h"
//========================================================================
//                               本地常量声明
//========================================================================

#define DIS_DOT   0x20
#define DIS_BLACK 0x10
#define DIS_      0x11

//========================================================================
//                            外部函数和变量声明
//========================================================================

extern u8  hour, minute, second;  // RTC变量
extern u16 msecond;

void APP_config(void);
// void DisplayScan(void);

#endif
