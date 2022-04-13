/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8H_PWM.h"
#include	"STC8G_H_ADC.h"
#include	"STC8G_PCA.h"
#include	"STC8G_H_Exti.h"
#include	"STC8G_H_I2C.h"
#include	"STC8G_H_SPI.h"
#include	"STC8G_H_Timer.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_Compare.h"
#include	"STC8G_PWM15bit.h"
#include	"STC8G_H_Delay.h"
#include	"STC8G_H_Switch.h"
#include	"STC8G_H_WDT.h"
#include	"STC8G_H_NVIC.h"
#include	"app.h"

//========================================================================
//                                IO������
//========================================================================
void	GPIO_config(void)
{
	P0_MODE_IO_PU(GPIO_Pin_All);		//P0 ����Ϊ׼˫���
	P1_MODE_IN_HIZ(GPIO_Pin_LOW);		//P1.0~P1.3 ����Ϊ��������
	P1_MODE_OUT_OD(GPIO_Pin_4 | GPIO_Pin_5);		//P1.4,P1.5 ����Ϊ©����·
	P2_MODE_IO_PU(GPIO_Pin_All);		//P2 ����Ϊ׼˫���
	P3_MODE_IO_PU(GPIO_Pin_LOW);		//P3.0~P3.3 ����Ϊ׼˫���
	P3_MODE_IO_PU(GPIO_Pin_HIGH);		//P3.4~P3.7 ����Ϊ׼˫���
//	P3_MODE_IN_HIZ(GPIO_Pin_6 | GPIO_Pin_7);		//P3.6,P3.7 ����Ϊ��������
	P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_7);	//P4.0,P4.6,P4.7 ����Ϊ׼˫���
	P6_MODE_IO_PU(GPIO_Pin_All);		//P6 ����Ϊ׼˫���
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 ����Ϊ׼˫���
	
	P1_PULL_UP_ENABLE(GPIO_Pin_4 | GPIO_Pin_5);	//P1.4,P1.5 �����ڲ�����
}

//========================================================================
//                               ��ʱ������
//========================================================================
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);					//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

//	//��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ20000HZ���жϺ�����P6.6ȡ�����10KHZ�����ź�.
//	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 20000);			//��ֵ,
//	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
//	Timer_Inilize(Timer1,&TIM_InitStructure);					//��ʼ��Timer1	  Timer0,Timer1,Timer2,Timer3,Timer4
//	NVIC_Timer1_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

//	//��ʱ��2��16λ�Զ���װ, �ж�Ƶ��Ϊ10000HZ���жϺ�����P6.5ȡ�����5KHZ�����ź�.
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 10000);				//��ֵ
//	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
//	Timer_Inilize(Timer2,&TIM_InitStructure);					//��ʼ��Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
//	NVIC_Timer2_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�

//	//��ʱ��3��16λ�Զ���װ, �ж�Ƶ��Ϊ100HZ���жϺ�����P6.4ȡ�����50HZ�����ź�.
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (100*12));		//��ֵ
//	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
//	Timer_Inilize(Timer3,&TIM_InitStructure);					//��ʼ��Timer3	  Timer0,Timer1,Timer2,Timer3,Timer4
//	NVIC_Timer3_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�

//	//��ʱ��4��16λ�Զ���װ, �ж�Ƶ��Ϊ50HZ���жϺ�����P6.3ȡ�����25HZ�����ź�.
//	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
//	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (50*12));		//��ֵ
//	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
//	Timer_Inilize(Timer4,&TIM_InitStructure);					//��ʼ��Timer4	  Timer0,Timer1,Timer2,Timer3,Timer4
//	NVIC_Timer4_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
}

//========================================================================
//                              ADC��ʼ��
//========================================================================
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//��ʼ��
	ADC_PowerControl(ENABLE);						//ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

//========================================================================
//                              ���ڳ�ʼ��
//========================================================================
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

//	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
////	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2, ���Բ���ѡ��)
//	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
//	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
//	UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������2 USART1,USART2,USART3,USART4
//	NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

//========================================================================
//                               I2C��ʼ��
//========================================================================
void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;
	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//����ѡ��   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;						//I2C����ʹ��,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;					//����ʹ���Զ�����,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 16;								//�����ٶ�=Fosc/2/(Speed*2+4),      0~63
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);		//����ģʽ, I2C_Mode_Master, I2C_Mode_Slave; �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

//========================================================================
//                               SPI��ʼ��
//========================================================================
void	SPI_config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Enable    = ENABLE;						//SPI����    ENABLE, DISABLE
	SPI_InitStructure.SPI_SSIG      = ENABLE;						//Ƭѡλ     ENABLE, DISABLE
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;					//��λ����   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;		//����ѡ��   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;			//ʱ����λ   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_2Edge;		//���ݱ���   SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_Speed     = SPI_Speed_4;			//SPI�ٶ�    SPI_Speed_4, SPI_Speed_16, SPI_Speed_64, SPI_Speed_128
	SPI_Init(&SPI_InitStructure);
	NVIC_SPI_Init(ENABLE,Priority_3);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

//========================================================================
//                             �Ƚ�����ʼ��
//========================================================================
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;						//�ṹ����
	CMP_InitStructure.CMP_EN = ENABLE;								//����Ƚ���		ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_P37;		//�Ƚ�����������ѡ��, CMP_P_P37: ѡ��P3.7��������, CMP_P_ADC: ��ADCģ���������������.
	CMP_InitStructure.CMP_N_Select     = CMP_N_GAP;		//�Ƚ������븺��ѡ��, CMP_N_GAP: ѡ���ڲ�BandGap����OP��ĵ�ѹ��������, CMP_N_P36: ѡ��P3.6��������.
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;			//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;			//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_Outpt_En     = ENABLE;			//����ȽϽ�����,ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 16;					//�ȽϽ���仯��ʱ������, 0~63
	CMP_Inilize(&CMP_InitStructure);				//��ʼ���Ƚ���
	NVIC_CMP_Init(RISING_EDGE|FALLING_EDGE,Priority_0);	//�ж�ʹ��, RISING_EDGE/FALLING_EDGE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}
//========================================================================
//                             �ⲿ�жϳ�ʼ��
//========================================================================
void	Exti_config(void)
{
	EXTI_InitTypeDef	Exti_InitStructure;							//�ṹ����

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//��ʼ��
	NVIC_INT0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT1,&Exti_InitStructure);				//��ʼ��
	NVIC_INT1_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	NVIC_INT2_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
	NVIC_INT3_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
	NVIC_INT4_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
}

//========================================================================
//                             �ⲿ�жϳ�ʼ��
//========================================================================

//========================================================================
//                               ���ܽ��л�
//========================================================================
void	Switch_config(void)
{
	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
	UART2_SW(UART2_SW_P46_P47);		//UART2_SW_P10_P11,UART2_SW_P46_P47
//	UART3_SW(UART3_SW_P00_P01);		//UART3_SW_P00_P01,UART3_SW_P50_P51
//	UART4_SW(UART4_SW_P02_P03);		//UART4_SW_P02_P03,UART4_SW_P52_P53
//	I2C_SW(I2C_P14_P15);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
//	COMP_SW(CMP_OUT_P34);					//CMP_OUT_P34,CMP_OUT_P41
	SPI_SW(SPI_P22_P23_P24_P25);	//SPI_P12_P13_P14_P15,SPI_P22_P23_P24_P25,SPI_P54_P40_P41_P43,SPI_P35_P34_P33_P32

	PWM1_SW(PWM1_SW_P60_P61);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
	PWM2_SW(PWM2_SW_P62_P63);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
	PWM3_SW(PWM3_SW_P64_P65);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
	PWM4_SW(PWM4_SW_P66_P67);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	PWM5_SW(PWM5_SW_P00);					//PWM5_SW_P20,PWM5_SW_P17,PWM5_SW_P00,PWM5_SW_P74
	PWM6_SW(PWM6_SW_P01);					//PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75
	PWM7_SW(PWM7_SW_P02);					//PWM7_SW_P22,PWM7_SW_P33,PWM7_SW_P02,PWM7_SW_P76
	PWM8_SW(PWM8_SW_P03);					//PWM8_SW_P23,PWM8_SW_P34,PWM8_SW_P03,PWM8_SW_P77

//	PCA_SW(PCA_P12_P11_P10_P37);	//PCA_P12_P11_P10_P37,PCA_P34_P35_P36_P37,PCA_P24_P25_P26_P27
}

//========================================================================
//                                ϵͳ��ʼ��
//========================================================================
void	SYS_Init(void)
{
//	GPIO_config();
	Timer_config();
//	ADC_config();
//	UART_config();
//	Exti_config();
//	I2C_config();
//	SPI_config();
//	CMP_config();
	Switch_config();
	EA = 1;
	
	APP_config();
}

