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

#ifndef __TASK_H
#define __TASK_H

#include "config.h"

//========================================================================
//                               本地变量声明
//========================================================================

typedef struct {
    u8  Run;                 //任务状态：Run/Stop
    u16 TIMCount;            //定时计数器
    u16 TRITime;             //重载计数器
    void (*TaskHook)(void);  //任务函数
} TASK_COMPONENTS;

//========================================================================
//                             外部函数和变量声明
//========================================================================

void Task_Marks_Handler_Callback(void);
void Task_Pro_Handler_Callback(void);

#endif
