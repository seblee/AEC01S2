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

#include "STC8G_H_ADC.h"

//========================================================================
// 函数: void	ADC_Inilize(ADC_InitTypeDef *ADCx)
// 描述: ADC初始化程序.
// 参数: ADCx: 结构参数,请参考adc.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
void ADC_Inilize(ADC_InitTypeDef *ADCx)
{
    ADCCFG = (ADCCFG & ~ADC_SPEED_2X16T) | ADCx->ADC_Speed;  //设置ADC工作时钟频率
    ADC_Justify(ADCx->ADC_AdjResult);                        // AD转换结果对齐方式

    if (ADCx->ADC_SMPduty > 31)
        return;  //错误
    if (ADCx->ADC_CsSetup > 1)
        return;  //错误
    if (ADCx->ADC_CsHold > 3)
        return;  //错误

    EAXSFR();                                                                         /* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展SFR(XSFR) */
    ADCTIM = (ADCx->ADC_CsSetup << 7) | (ADCx->ADC_CsHold << 5) | ADCx->ADC_SMPduty;  //设置 ADC 内部时序，ADC采样时间建议设最大值
    EAXRAM();                                                                         /* MOVX A,@DPTR/MOVX @DPTR,A指令的操作对象为扩展RAM(XRAM) */
}

//========================================================================
// 函数: void	ADC_PowerControl(u8 pwr)
// 描述: ADC电源控制程序.
// 参数: pwr: 电源控制,ENABLE或DISABLE.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
void ADC_PowerControl(u8 pwr)
{
    if (pwr == ENABLE)
        ADC_CONTR |= 0x80;
    else
        ADC_CONTR &= 0x7f;
}

//========================================================================
// 函数: u16	Get_ADCResult(u8 channel)
// 描述: 查询法读一次ADC转换结果.
// 参数: channel: 选择要转换的ADC通道.
// 返回: ADC转换结果.
// 版本: V1.0, 2012-10-22
//========================================================================
u16 Get_ADCResult(u8 channel)  // channel = 0~15
{
    u16 adc;
    u8  i;

    if (channel > ADC_CH15)
        return 4096;  //错误,返回4096,调用的程序判断
    ADC_RES  = 0;
    ADC_RESL = 0;

    ADC_CONTR = (ADC_CONTR & 0xf0) | ADC_START | channel;
    NOP(10);  //对ADC_CONTR操作后等待会儿再访问

    for (i = 0; i < 250; i++) {  //超时返回，正常i等于10以内就可以转换完成
        if (ADC_CONTR & ADC_FLAG) {
            ADC_CONTR &= ~ADC_FLAG;
            if (ADCCFG & (1 << 5)) {  //转换结果右对齐。
                adc = ((u16)ADC_RES << 8) | ADC_RESL;
            } else {  //转换结果左对齐。
#if ADC_RES_12BIT == 1
                adc = (u16)ADC_RES;
                adc = (adc << 4) | ((ADC_RESL >> 4) & 0x0f);
#else
                adc = (u16)ADC_RES;
                adc = (adc << 2) | ((ADC_RESL >> 6) & 0x03);
#endif
            }
            return adc;
        }
    }
    return 4096;  //错误,返回4096,调用的程序判断
}
