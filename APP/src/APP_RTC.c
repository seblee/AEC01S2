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
#include	"STC8G_H_I2C.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Switch.h"

/*************	����˵��	**************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

��дRTC, ICΪPCF8563.

��STC��MCU��IO��ʽ����8λ����ܡ�

ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������, �û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.

8λ�������ʾʱ��(Сʱ-����-��).

����ɨ�谴������Ϊ17~32.

����ֻ֧�ֵ�������, ��֧�ֶ��ͬʱ����, ���������в���Ԥ֪�Ľ��.

�����³���1���,����10��/����ٶ��ṩ�ؼ����. �û�ֻ��Ҫ���KeyCode�Ƿ��0���жϼ��Ƿ���.

����ʱ���:
����17: Сʱ+.
����18: Сʱ-.
����19: ����+.
����20: ����-.

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

//========================================================================
//                               ���س�������	
//========================================================================


//========================================================================
//                               ���ر�������
//========================================================================


//========================================================================
//                               ���غ�������
//========================================================================

void IO_KeyScan(void);   //50ms call
void DisplayRTC(void);
void ReadRTC(void);
void WriteRTC(void);

//========================================================================
//                            �ⲿ�����ͱ�������
//========================================================================


//========================================================================
// ����: RTC_init
// ����: �û���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void RTC_init(void)
{
	u8  i;

	I2C_InitTypeDef		I2C_InitStructure;
	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//����ѡ��   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;						//I2C����ʹ��,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;					//����ʹ���Զ�����,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 16;								//�����ٶ�=Fosc/2/(Speed*2+4),      0~63
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);		//����ģʽ, I2C_Mode_Master, I2C_Mode_Slave; �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	
	P0_MODE_IO_PU(GPIO_Pin_All);		//P0 ����Ϊ׼˫���
	P1_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);		//P1.4,P1.5 ����Ϊ׼˫���
	P6_MODE_IO_PU(GPIO_Pin_All);		//P6 ����Ϊ׼˫���
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 ����Ϊ׼˫���
	I2C_SW(I2C_P14_P15);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
	display_index = 0;
	
	for(i=0; i<8; i++)  LED8[i] = 0x10; //�ϵ�����
    
	ReadRTC();
	F0 = 0;
	if(second >= 60)    F0 = 1; //����
	if(minute >= 60)    F0 = 1; //����
	if(hour   >= 24)    F0 = 1; //����
	if(F0)  //�д���, Ĭ��12:00:00
	{
		second = 0;
		minute = 0;
		hour  = 12;
		WriteRTC();
	}

	DisplayRTC();
	LED8[2] = DIS_;
	LED8[5] = DIS_;

	KeyHoldCnt = 0; //�����¼�ʱ
	KeyCode = 0;    //���û�ʹ�õļ���

	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;
	cnt50ms = 0;
}

//========================================================================
// ����: Sample_RTC
// ����: �û�Ӧ�ó���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void Sample_RTC(void)
{
	DisplayScan();
	
	if(++msecond >= 1000)   //1�뵽
	{
		msecond = 0;
		ReadRTC();
		DisplayRTC();
	}

	if(++cnt50ms >= 50)     //50msɨ��һ�����м���
	{
		cnt50ms = 0;
		IO_KeyScan();
	}
	
	if(KeyCode != 0)        //�м�����
	{
		if(KeyCode == 17)   //hour +1
		{
			if(++hour >= 24)    hour = 0;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 18)   //hour -1
		{
			if(--hour >= 24)    hour = 23;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 19)   //minute +1
		{
			second = 0;
			if(++minute >= 60)  minute = 0;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 20)   //minute -1
		{
			second = 0;
			if(--minute >= 60)  minute = 59;
			WriteRTC();
			DisplayRTC();
		}

		KeyCode = 0;
	}
}

//========================================================================
// ����: DisplayRTC
// ����: ��ʾʱ�Ӻ���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void DisplayRTC(void)
{
    if(hour >= 10)  LED8[0] = hour / 10;
    else            LED8[0] = DIS_BLACK;
    LED8[1] = hour % 10;
    LED8[2] = DIS_;
    LED8[3] = minute / 10;
    LED8[4] = minute % 10;
    LED8[6] = second / 10;
    LED8[7] = second % 10;
}

//========================================================================
// ����: ReadRTC
// ����: ��RTC����.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void ReadRTC(void)
{
    u8  tmp[3];

    I2C_ReadNbyte(2, tmp, 3);
    second = ((tmp[0] >> 4) & 0x07) * 10 + (tmp[0] & 0x0f);
    minute = ((tmp[1] >> 4) & 0x07) * 10 + (tmp[1] & 0x0f);
    hour   = ((tmp[2] >> 4) & 0x03) * 10 + (tmp[2] & 0x0f);
}

//========================================================================
// ����: WriteRTC
// ����: дRTC����.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void WriteRTC(void)
{
    u8  tmp[3];

    tmp[0] = ((second / 10) << 4) + (second % 10);
    tmp[1] = ((minute / 10) << 4) + (minute % 10);
    tmp[2] = ((hour / 10) << 4) + (hour % 10);
    I2C_WriteNbyte(2, tmp, 3);
}


/*****************************************************
    ���м�ɨ�����
    ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�

   Y     P04      P05      P06      P07
          |        |        |        |
X         |        |        |        |
P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
          |        |        |        |
P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
          |        |        |        |
P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
          |        |        |        |
P03 ---- K12 ---- K13 ---- K14 ---- K15 ----
          |        |        |        |
******************************************************/


//========================================================================
// ����: IO_KeyDelay
// ����: ����ɨ���ӳٳ���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void IO_KeyDelay(void)
{
    u8 i;
    i = 60;
    while(--i)  ;
}

//========================================================================
// ����: IO_KeyScan
// ����: ����ɨ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void IO_KeyScan(void)    //50ms call
{
    u8  j;

    j = IO_KeyState1;   //������һ��״̬

    P0 = 0xf0;  //X�ͣ���Y
    IO_KeyDelay();
    IO_KeyState1 = P0 & 0xf0;

    P0 = 0x0f;  //Y�ͣ���X
    IO_KeyDelay();
    IO_KeyState1 |= (P0 & 0x0f);
    IO_KeyState1 ^= 0xff;   //ȡ��
    
    if(j == IO_KeyState1)   //�������ζ����
    {
        j = IO_KeyState;
        IO_KeyState = IO_KeyState1;
        if(IO_KeyState != 0)    //�м�����
        {
            F0 = 0;
            if(j == 0)  F0 = 1; //��һ�ΰ���
            else if(j == IO_KeyState)
            {
                if(++IO_KeyHoldCnt >= 20)   //1����ؼ�
                {
                    IO_KeyHoldCnt = 18;
                    F0 = 1;
                }
            }
            if(F0)
            {
                j = T_KeyTable[IO_KeyState >> 4];
                if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0)) 
                    KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;    //������룬17~32
            }
        }
        else    IO_KeyHoldCnt = 0;
    }
    P0 = 0xff;
}