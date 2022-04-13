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
#include	"STC8G_H_GPIO.h"
#include	"STC8G_PWM15bit.h"

/*************   ����˵��   ***************

�����̻���STC8G2K64���б�д���ԣ�STC8Gϵ�д�15λ��ǿ��PWMоƬ��ͨ�òο�.

���4·PWM�źţ�
PWM00,PWM01  ���"������"Ч��PWM.
PWM02  ѭ�����1����PWM��1���Ӹߵ�ƽ��1���ӵ͵�ƽ.
PWM03  �̶�PWM.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/


//========================================================================
//                               ���س�������	
//========================================================================


//========================================================================
//                               ���ر�������
//========================================================================

u16	pwm0,pwm1,pwm2;
bit	B_PWM0_Dir,B_PWM1_Dir;	//����, 0Ϊ+, 1Ϊ-.

//========================================================================
//                               ���غ�������
//========================================================================


//========================================================================
//                            �ⲿ�����ͱ�������
//========================================================================


//========================================================================
// ����: PWM15bit_init
// ����: �û���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void PWM15bit_init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	PWM15_InitTypeDef		PWM15_InitStructure;

	GPIO_InitStructure.Pin  = GPIO_Pin_All;			//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//��ʼ��

	PWM15_InitStructure.PWM_Enable    = ENABLE;		//PWMʹ��,  ENABLE, DISABLE
	PWM15_InitStructure.PWM_Period    = 0x1000;		//PWM����,  0~0x7fff
	PWM15_InitStructure.PWM_Clock_Sel = PWMn_CLK_SYS;		//ʱ��Դѡ��,  PWMn_CLK_SYS, PWMn_CLK_TM2
	PWM15_InitStructure.PWM_Clock_PS  = 0;				//ϵͳʱ�ӷ�Ƶ����(PS+1��Ƶ),  0~15
	PWM15_InitStructure.PWM_CInt      = DISABLE;	//�����������ж�ʹ��,  ENABLE, DISABLE
	PWM15_InitStructure.PWM_Counter   = ENABLE;		//������ʹ��,  ENABLE, DISABLE
	PWM15_Init(PWM0,&PWM15_InitStructure);
	
	PWM15Duty(PWM00,0x000,0x200);		//PWM_ID, ����͵�ƽλ��, ����ߵ�ƽλ��
	PWM15Duty(PWM01,0x000,0x400);		//PWM_ID, ����͵�ƽλ��, ����ߵ�ƽλ��
	PWM15Duty(PWM02,0x000,0x800);		//PWM_ID, ����͵�ƽλ��, ����ߵ�ƽλ��
	PWM15Duty(PWM03,0x200,0xc00);		//PWM_ID, ����͵�ƽλ��, ����ߵ�ƽλ��
	PWMChannelCtrl(PWM00,ENABLE,0,DISABLE,DISABLE,DISABLE);	//PWM_ID, ���ʹ��, ��ʼ��ƽ, PWM�ж�, �ڶ����������ж�, ��һ�������ж�
	PWMChannelCtrl(PWM01,ENABLE,0,DISABLE,DISABLE,DISABLE);	//PWM_ID, ���ʹ��, ��ʼ��ƽ, PWM�ж�, �ڶ����������ж�, ��һ�������ж�
	PWMChannelCtrl(PWM02,ENABLE,0,DISABLE,DISABLE,DISABLE);	//PWM_ID, ���ʹ��, ��ʼ��ƽ, PWM�ж�, �ڶ����������ж�, ��һ�������ж�
	PWMChannelCtrl(PWM03,ENABLE,0,DISABLE,DISABLE,DISABLE);	//PWM_ID, ���ʹ��, ��ʼ��ƽ, PWM�ж�, �ڶ����������ж�, ��һ�������ж�

	pwm0 = 0;
	pwm1 = 0;
	pwm2 = 0;
	B_PWM0_Dir = 0;
	B_PWM1_Dir = 0;
}

//========================================================================
// ����: Sample_PWM15bit
// ����: �û�Ӧ�ó���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
void Sample_PWM15bit(void)
{
	if(B_PWM0_Dir)
	{
		if(--pwm0 <= 0x100)	B_PWM0_Dir = 0;	//PWM00
	}
	else	if(++pwm0 >= 0xf00)	B_PWM0_Dir = 1;	//PWM00
	PWM15Duty(PWM00,0,pwm0);

	if(B_PWM1_Dir)
	{
		if(--pwm1 <= 0x100)		B_PWM1_Dir = 0;	//PWM01
	}
	else	if(++pwm1 >= 0xf00)	B_PWM1_Dir = 1;	//PWM01
	PWM15Duty(PWM01,0,pwm1);

	if(pwm2 >= 3000)
	{
		pwm2 = 0;
	}
	else	if(pwm2 >= 2000)
	{
		PWMLevelSet(PWM02,ENABLE,DISABLE);	//PWM_ID, ǿ������͵�ƽ, ǿ������ߵ�ƽ
	}
	else	if(pwm2 >= 1000)
	{
		PWMLevelSet(PWM02,DISABLE,ENABLE);	//PWM_ID, ǿ������͵�ƽ, ǿ������ߵ�ƽ
	}
	else
	{
		PWMLevelSet(PWM02,DISABLE,DISABLE);	//PWM_ID, ǿ������͵�ƽ, ǿ������ߵ�ƽ
	}
	pwm2++;
}


