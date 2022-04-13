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

#include	"APP.h"
#include	"STC8H_PWM.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_NVIC.h"

/*************	����˵��	**************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8Hϵ��оƬ��ͨ�òο�.

�߼�PWM��ʱ�� PWM5,PWM6,PWM7,PWM8 ÿ��ͨ�����ɶ���ʵ��PWM���.

4��ͨ��PWM������Ҫ���ö�Ӧ����ڣ���ͨ��ʾ�����۲�������ź�.

PWM���ں�ռ�ձȿ����Զ������ã���߿ɴ�65535.

����ʱ, ѡ��ʱ�� 24MHZ (�û�����"config.h"�޸�Ƶ��).

******************************************/


//========================================================================
//                               ���س�������	
//========================================================================


//========================================================================
//                               ���ر�������
//========================================================================

PWMx_Duty PWMB_Duty;
bit PWM5_Flag;
bit PWM6_Flag;
bit PWM7_Flag;
bit PWM8_Flag;

//========================================================================
//                               ���غ�������
//========================================================================


//========================================================================
//                            �ⲿ�����ͱ�������
//========================================================================


//========================================================================
// ����: PWMB_Output_init
// ����: �û���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void PWMB_Output_init(void)
{
	PWMx_InitDefine		PWMx_InitStructure;
	
	PWMB_Duty.PWM5_Duty = 128;
	PWMB_Duty.PWM6_Duty = 256;
	PWMB_Duty.PWM7_Duty = 512;
	PWMB_Duty.PWM8_Duty = 1024;

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = PWMB_Duty.PWM5_Duty;	//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO5P;					//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM5, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = PWMB_Duty.PWM6_Duty;	//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO6P;					//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM6, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = PWMB_Duty.PWM7_Duty;	//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO7P;					//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM7, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = PWMB_Duty.PWM8_Duty;	//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO8P;					//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM8, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

	PWMx_InitStructure.PWM_Period   = 2047;							//����ʱ��,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;								//��������������, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;				//�����ʹ��, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);				//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
	P0_MODE_IO_PU(GPIO_Pin_LOW);		//P0.0,P0.1,P0.2,P0.3 ����Ϊ׼˫���
}

//========================================================================
// ����: Sample_PWMB_Output
// ����: �û�Ӧ�ó���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void Sample_PWMB_Output(void)
{
	if(!PWM5_Flag)
	{
		PWMB_Duty.PWM5_Duty++;
		if(PWMB_Duty.PWM5_Duty >= 2047) PWM5_Flag = 1;
	}
	else
	{
		PWMB_Duty.PWM5_Duty--;
		if(PWMB_Duty.PWM5_Duty <= 0) PWM5_Flag = 0;
	}

	if(!PWM6_Flag)
	{
		PWMB_Duty.PWM6_Duty++;
		if(PWMB_Duty.PWM6_Duty >= 2047) PWM6_Flag = 1;
	}
	else
	{
		PWMB_Duty.PWM6_Duty--;
		if(PWMB_Duty.PWM6_Duty <= 0) PWM6_Flag = 0;
	}

	if(!PWM7_Flag)
	{
		PWMB_Duty.PWM7_Duty++;
		if(PWMB_Duty.PWM7_Duty >= 2047) PWM7_Flag = 1;
	}
	else
	{
		PWMB_Duty.PWM7_Duty--;
		if(PWMB_Duty.PWM7_Duty <= 0) PWM7_Flag = 0;
	}

	if(!PWM8_Flag)
	{
		PWMB_Duty.PWM8_Duty++;
		if(PWMB_Duty.PWM8_Duty >= 2047) PWM8_Flag = 1;
	}
	else
	{
		PWMB_Duty.PWM8_Duty--;
		if(PWMB_Duty.PWM8_Duty <= 0) PWM8_Flag = 0;
	}
	
	UpdatePwm(PWMB, &PWMB_Duty);
}



