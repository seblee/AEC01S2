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

#include "STC8G_H_UART.h"

//========================================================================
//                               本地变量声明
//========================================================================

// u8 xdata *TX1_Buffer;
// u8 xdata *RX1_Buffer;
// u8 xdata *TX2_Buffer;
// u8 xdata *RX2_Buffer;

#ifdef UART1
COMx_Define COM1;
u8 xdata    TX1_Buffer[COM_TX1_Lenth];  //发送缓冲
u8 xdata    RX1_Buffer[COM_RX1_Lenth];  //接收缓冲
#endif
#ifdef UART2
COMx_Define COM2;
u8 xdata    TX2_Buffer[COM_TX2_Lenth];  //发送缓冲
u8 xdata    RX2_Buffer[COM_RX2_Lenth];  //接收缓冲
#endif
#ifdef UART3
COMx_Define COM3;
u8 xdata    TX3_Buffer[COM_TX3_Lenth];  //发送缓冲
u8 xdata    RX3_Buffer[COM_RX3_Lenth];  //接收缓冲
#endif
#ifdef UART4
COMx_Define COM4;
u8 xdata    TX4_Buffer[COM_TX4_Lenth];  //发送缓冲
u8 xdata    RX4_Buffer[COM_RX4_Lenth];  //接收缓冲
#endif

//========================================================================
// 函数: UART_Configuration
// 描述: UART初始化程序.
// 参数: UARTx: UART组号, COMx结构参数,请参考UART.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
u8 UART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
#if defined(UART1) || defined(UART2) || defined(UART3) || defined(UART4)
    u8  i;
    u32 j;
#else
    UARTx = NULL;
    COMx  = NULL;
#endif

#ifdef UART1
    if (UARTx == UART1) {
        COM1.id         = 1;
        COM1.TX_read    = 0;
        COM1.TX_write   = 0;
        COM1.B_TX_busy  = 0;
        COM1.RX_Cnt     = 0;
        COM1.RX_TimeOut = 0;
        COM1.B_RX_OK    = 0;

        // if (TX1_Buffer) {free(TX1_Buffer);TX1_Buffer = NULL;}  // free memory
        // if (RX1_Buffer) {free(RX1_Buffer);RX1_Buffer = NULL;}  // free memory

        // TX1_Buffer = (u8 *)calloc(COM_TX1_Lenth, sizeof(u8));
        // RX1_Buffer = (u8 *)calloc(COM_RX1_Lenth, sizeof(u8));
        for (i = 0; i < COM_TX1_Lenth; i++)
            TX1_Buffer[i] = 0;
        for (i = 0; i < COM_RX1_Lenth; i++)
            RX1_Buffer[i] = 0;

        SCON = (SCON & 0x3f) | COMx->UART_Mode;                                            //模式设置
        if ((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx)) {  //可变波特率
            j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;                                     //按1T计算
            if (j >= 65536UL)
                return FAIL;  //错误
            j = 65536UL - j;
            if (COMx->UART_BRT_Use == BRT_Timer1) {
                TR1 = 0;
                AUXR &= ~0x01;      // S1 BRT Use Timer1;
                TMOD &= ~(1 << 6);  // Timer1 set As Timer
                TMOD &= ~0x30;      // Timer1_16bitAutoReload;
                AUXR |= (1 << 6);   // Timer1 set as 1T mode
                TH1 = (u8)(j >> 8);
                TL1 = (u8)j;
                ET1 = 0;            //禁止中断
                TMOD &= ~0x40;      //定时
                INT_CLKO &= ~0x02;  //不输出时钟
                TR1 = 1;
            } else if (COMx->UART_BRT_Use == BRT_Timer2) {
                AUXR &= ~(1 << 4);  // Timer stop
                AUXR |= 0x01;       // S1 BRT Use Timer2;
                AUXR &= ~(1 << 3);  // Timer2 set As Timer
                AUXR |= (1 << 2);   // Timer2 set as 1T mode
                TH2 = (u8)(j >> 8);
                TL2 = (u8)j;
                IE2 &= ~(1 << 2);  //禁止中断
                AUXR |= (1 << 4);  // Timer run enable
            } else
                return FAIL;  //错误
        } else if (COMx->UART_Mode == UART_ShiftRight) {
            if (COMx->BaudRateDouble == ENABLE)
                AUXR |= (1 << 5);  //固定波特率SysClk/2
            else
                AUXR &= ~(1 << 5);                  //固定波特率SysClk/12
        } else if (COMx->UART_Mode == UART_9bit) {  //固定波特率SysClk*2^SMOD/64
            if (COMx->BaudRateDouble == ENABLE)
                PCON |= (1 << 7);  //固定波特率SysClk/32
            else
                PCON &= ~(1 << 7);  //固定波特率SysClk/64
        }
        UART1_RxEnable(COMx->UART_RxEnable);  // UART接收使能

        return SUCCESS;
    }
#endif
#ifdef UART2
    if (UARTx == UART2) {
        COM2.id         = 2;
        COM2.TX_read    = 0;
        COM2.TX_write   = 0;
        COM2.B_TX_busy  = 0;
        COM2.RX_Cnt     = 0;
        COM2.RX_TimeOut = 0;
        COM2.B_RX_OK    = 0;

        // if(TX1_Buffer){free(TX1_Buffer);TX1_Buffer = NULL;} //free memory
        // if(RX1_Buffer){free(RX1_Buffer);RX1_Buffer = NULL;} //free memory

        // TX2_Buffer = (u8*)calloc(COM_TX2_Lenth,sizeof(u8));
        // RX2_Buffer = (u8*)calloc(COM_RX2_Lenth,sizeof(u8));
        for (i = 0; i < COM_TX2_Lenth; i++)
            TX2_Buffer[i] = 0;
        for (i = 0; i < COM_RX2_Lenth; i++)
            RX2_Buffer[i] = 0;

        if ((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx)) {  //可变波特率
            if (COMx->UART_Mode == UART_9bit_BRTx)
                S2CON |= (1 << 7);  // 9bit
            else
                S2CON &= ~(1 << 7);                     // 8bit
            j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;  //按1T计算
            if (j >= 65536UL)
                return FAIL;  //错误
            j = 65536UL - j;
            AUXR &= ~(1 << 4);  // Timer stop
            AUXR &= ~(1 << 3);  // Timer2 set As Timer
            AUXR |= (1 << 2);   // Timer2 set as 1T mode
            TH2 = (u8)(j >> 8);
            TL2 = (u8)j;
            IE2 &= ~(1 << 2);  //禁止中断
            AUXR |= (1 << 4);  // Timer run enable
        } else
            return FAIL;                      //模式错误
        UART2_RxEnable(COMx->UART_RxEnable);  // UART接收使能

        return SUCCESS;
    }
#endif
#ifdef UART3
    if (UARTx == UART3) {
        COM3.id         = 3;
        COM3.TX_read    = 0;
        COM3.TX_write   = 0;
        COM3.B_TX_busy  = 0;
        COM3.RX_Cnt     = 0;
        COM3.RX_TimeOut = 0;
        COM3.B_RX_OK    = 0;
        for (i = 0; i < COM_TX3_Lenth; i++)
            TX3_Buffer[i] = 0;
        for (i = 0; i < COM_RX3_Lenth; i++)
            RX3_Buffer[i] = 0;

        if ((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx)) {  //可变波特率
            if (COMx->UART_Mode == UART_9bit_BRTx)
                S3_9bit();  // 9bit
            else
                S3_8bit();                              // 8bit
            j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;  //按1T计算
            if (j >= 65536UL)
                return FAIL;  //错误
            j = 65536UL - j;
            if (COMx->UART_BRT_Use == BRT_Timer3) {
                S3_BRT_UseTimer3();  // S3 BRT Use Timer3;
                TH3 = (u8)(j >> 8);
                TL3 = (u8)j;
                T4T3M &= 0xf0;
                T4T3M |= 0x0a;  // Timer3 set As Timer, 1T mode, Start timer3
            } else if (COMx->UART_BRT_Use == BRT_Timer2) {
                AUXR &= ~(1 << 4);   // Timer stop
                S3_BRT_UseTimer2();  // S3 BRT Use Timer2;
                AUXR &= ~(1 << 3);   // Timer2 set As Timer
                AUXR |= (1 << 2);    // Timer2 set as 1T mode
                TH2 = (u8)(j >> 8);
                TL2 = (u8)j;
                IE2 &= ~(1 << 2);  //禁止中断
                AUXR |= (1 << 4);  // Timer run enable
            } else
                return FAIL;  //错误
        } else
            return FAIL;                      //模式错误
        UART3_RxEnable(COMx->UART_RxEnable);  // UART接收使能

        return SUCCESS;
    }
#endif
#ifdef UART4
    if (UARTx == UART4) {
        COM4.id         = 3;
        COM4.TX_read    = 0;
        COM4.TX_write   = 0;
        COM4.B_TX_busy  = 0;
        COM4.RX_Cnt     = 0;
        COM4.RX_TimeOut = 0;
        COM4.B_RX_OK    = 0;
        for (i = 0; i < COM_TX4_Lenth; i++)
            TX4_Buffer[i] = 0;
        for (i = 0; i < COM_RX4_Lenth; i++)
            RX4_Buffer[i] = 0;

        if ((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx)) {  //可变波特率
            if (COMx->UART_Mode == UART_9bit_BRTx)
                S4_9bit();  // 9bit
            else
                S4_8bit();                              // 8bit
            j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;  //按1T计算
            if (j >= 65536UL)
                return FAIL;  //错误
            j = 65536UL - j;
            if (COMx->UART_BRT_Use == BRT_Timer4) {
                S4_BRT_UseTimer4();  // S4 BRT Use Timer4;
                TH4 = (u8)(j >> 8);
                TL4 = (u8)j;
                T4T3M &= 0x0f;
                T4T3M |= 0xa0;  // Timer4 set As Timer, 1T mode, Start timer4
            } else if (COMx->UART_BRT_Use == BRT_Timer2) {
                AUXR &= ~(1 << 4);   // Timer stop
                S4_BRT_UseTimer2();  // S4 BRT Use Timer2;
                AUXR &= ~(1 << 3);   // Timer2 set As Timer
                AUXR |= (1 << 2);    // Timer2 set as 1T mode
                TH2 = (u8)(j >> 8);
                TL2 = (u8)j;
                IE2 &= ~(1 << 2);  //禁止中断
                AUXR |= (1 << 4);  // Timer run enable
            } else
                return FAIL;  //错误
        } else
            return FAIL;                      //模式错误
        UART4_RxEnable(COMx->UART_RxEnable);  // UART接收使能

        return SUCCESS;
    }
#endif
    return FAIL;  //错误
}

/*********************************************************/

/********************* UART1 函数 ************************/
#ifdef UART1
void TX1_write2buff(u8 dat)  //写入发送缓冲，指针+1
{
    SBUF = dat;
    // COM1.B_TX_busy = 1;  //标志忙
    while (!TI) {
    }
    TI = 0;
    // while (COM1.B_TX_busy)
    //     ;
}

void PrintString1(u8 *puts)
{
    for (; *puts != 0; puts++)
        TX1_write2buff(*puts);  //遇到停止符0结束
}

#endif

/********************* UART2 函数 ************************/
#ifdef UART2
void TX2_write2buff(u8 dat)  //写入发送缓冲，指针+1
{
    S2BUF          = dat;
    COM2.B_TX_busy = 1;  //标志忙
    while (COM2.B_TX_busy)
        ;
}

void PrintString2(u8 *puts)
{
    for (; *puts != 0; puts++)
        TX2_write2buff(*puts);  //遇到停止符0结束
}

#endif

/********************* UART3 函数 ************************/
#ifdef UART3
void TX3_write2buff(u8 dat)  //写入发送缓冲，指针+1
{
    S3BUF          = dat;
    COM3.B_TX_busy = 1;  //标志忙
    while (COM3.B_TX_busy)
        ;
}

void PrintString3(u8 *puts)
{
    for (; *puts != 0; puts++)
        TX3_write2buff(*puts);  //遇到停止符0结束
}

#endif

/********************* UART4 函数 ************************/
#ifdef UART4
void TX4_write2buff(u8 dat)  //写入发送缓冲，指针+1
{
    S4BUF          = dat;
    COM4.B_TX_busy = 1;  //标志忙
    while (COM4.B_TX_busy)
        ;
}

void PrintString4(u8 *puts)
{
    for (; *puts != 0; puts++)
        TX4_write2buff(*puts);  //遇到停止符0结束
}

#endif

/*********************************************************/
/*
void COMx_write2buff(COMx_Define *COMx, u8 dat)  //写入发送缓冲，指针+1
{
    if (COMx->id == 1)TX1_write2buff(dat);
    if (COMx->id == 2)TX2_write2buff(dat);
    if (COMx->id == 3)TX3_write2buff(dat);
    if (COMx->id == 4)TX4_write2buff(dat);
}

void PrintString(COMx_Define *COMx, u8 *puts)
{
    for (; *puts != 0; puts++)
        COMx_write2buff(COMx, *puts);  //遇到停止符0结束
}
*/
