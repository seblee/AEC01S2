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

#include "APP_Lamp.h"
#include "STC8G_H_GPIO.h"

/***************	功能说明	****************

本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

程序使用P6口来演示跑马灯，输出低驱动。

下载时, 选择时钟 24MHz (可以在配置文件"config.h"中修改).

******************************************/

//========================================================================
//                               本地常量声明
//========================================================================

u8 code ledNum[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//========================================================================
//                               本地变量声明
//========================================================================

u8 ledIndex;

//========================================================================
//                               本地函数声明
//========================================================================

//========================================================================
//                            外部函数和变量声明
//========================================================================

//========================================================================
// 函数: Lamp_init
// 描述: 用户初始化程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2020-09-28
//========================================================================
void Lamp_init(void)
{
    P4_MODE_IO_PU(GPIO_Pin_0);    // P4.0设置为准双向口
    P6_MODE_IO_PU(GPIO_Pin_All);  // P6 设置为准双向口
}

//========================================================================
// 函数: Sample_Lamp
// 描述: 用户应用程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2020-09-23
//========================================================================
void Sample_Lamp(void)
{
    P40 = 0;                  // LED Power On
    P6  = ~ledNum[ledIndex];  //输出低驱动
    ledIndex++;
    if (ledIndex > 7) {
        ledIndex = 0;
    }
}
