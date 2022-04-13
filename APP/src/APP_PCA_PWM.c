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
#include	"STC8G_PCA.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_NVIC.h"

/*************   ����˵��   ***************

�����̻���STC8G1K08-20PIN���б�д���ԣ�STC8Gϵ��оƬ��ͨ�òο�.

���3·�仯��PWM�ź�, ����"������"������.
PWM0  Ϊ8λPWM.
PWM1  Ϊ7λPWM.
PWM2  Ϊ10λPWM.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/


//========================================================================
//                               ���س�������	
//========================================================================


//========================================================================
//                               ���ر�������
//========================================================================

static u16	pwm0,pwm1,pwm2;
static bit	B_PWM0_Dir,B_PWM1_Dir,B_PWM2_Dir;	//����, 0Ϊ+, 1Ϊ-.

//========================================================================
//                               ���غ�������
//========================================================================


//========================================================================
//                            �ⲿ�����ͱ�������
//========================================================================


//========================================================================
// ����: PCA_PWM_init
// ����: �û���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void PCA_PWM_init(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	P1_MODE_IO_PU(GPIO_Pin_All);		//P1 ����Ϊ׼˫���
	P3_MODE_IO_PU(GPIO_Pin_7);			//P3.7 ����Ϊ׼˫���
	//--------------------------------------------
	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_RUN      = DISABLE;				//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);			//���ù���Counter

	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 128 << 8;				//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_7bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 64 << 8;					//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_10bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 32 << 8;					//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA2,&PCA_InitStructure);

	NVIC_PCA_Init(PCA_Counter,PCA_Mode_PWM,Priority_0);
	NVIC_PCA_Init(PCA0,PCA_Mode_PWM,Priority_0);
	NVIC_PCA_Init(PCA1,PCA_Mode_PWM,Priority_0);
	NVIC_PCA_Init(PCA2,PCA_Mode_PWM,Priority_0);
	CR = 1;							//����PCA
	//--------------------------------------------
	pwm0 = 128;
	pwm1 = 64;
	pwm2 = 512;
	B_PWM0_Dir = 0;
	B_PWM1_Dir = 0;
	B_PWM2_Dir = 0;

	UpdatePcaPwm(PCA0,pwm0);
	UpdatePcaPwm(PCA1,pwm1);
	UpdatePcaPwm(PCA2,pwm2);
}


//========================================================================
// ����: Sample_PCA_PWM
// ����: �û�Ӧ�ó���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void Sample_PCA_PWM(void)
{
	if(B_PWM0_Dir)
	{
		if(--pwm0 <= 16)	B_PWM0_Dir = 0;	//8λPWM
	}
	else	if(++pwm0 >= 240)	B_PWM0_Dir = 1;	//8λPWM
	UpdatePcaPwm(PCA0,pwm0);

	if(B_PWM1_Dir)
	{
		if(--pwm1 <= 8)		B_PWM1_Dir = 0;	//7λPWM
	}
	else	if(++pwm1 >= 120)	B_PWM1_Dir = 1;	//7λPWM
	UpdatePcaPwm(PCA1,pwm1);

	if(B_PWM2_Dir)
	{
		if(--pwm2 <= 24)		B_PWM2_Dir = 0;	//10λPWM
	}
	else	if(++pwm2 >= 1000)	B_PWM2_Dir = 1;	//10λPWM
	UpdatePcaPwm(PCA2,pwm2);
}



