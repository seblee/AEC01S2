/**
 * @file APP_AHT21.c
 * @author  xiaowine (xiaowine@sina.cn)
 * @brief
 * @version 01.00
 * @date    2022-04-21
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-04-21 <td>1.0     <td>wangh     <td>内容
 * </table>
 * ******************************************************************
 * *                   .::::
 * *                 .::::::::
 * *                ::::::::::
 * *             ..:::::::::::
 * *          '::::::::::::
 * *            .:::::::::
 * *       '::::::::::::::..        女神助攻,流量冲天
 * *            ..::::::::::::.     永不宕机,代码无bug
 * *          ``:::::::::::::::
 * *           ::::``:::::::::'        .:::
 * *          ::::'   ':::::'       .::::::::
 * *        .::::'      ::::     .:::::::'::::
 * *       .:::'       :::::  .:::::::::' ':::::
 * *      .::'        :::::.:::::::::'      ':::::
 * *     .::'         ::::::::::::::'         ``::::
 * * ...:::           ::::::::::::'              ``::
 * *```` ':.          ':::::::::'                  ::::.
 * *                   '.:::::'                    ':'````.
 * ******************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "APP_AHT21.h"
#include "STC8G_H_Delay.h"
#include "STC8G_H_GPIO.h"
#include "STC8G_H_Soft_I2C.h"
#include "STC8G_H_UART.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
u32          CT_data[2];
volatile int c1, temp1;
/* Private function prototypes -----------------------------------------------*/
static u8   AHT20_Read_Status(void);
static void AHT20_Start_Init(void);
static void AHT20_Read_CTdata(u32 *ct);
static void ath21GPIOConfig(void);
static void AHT20_Init(void);                //初始化AHT20
static void AHT20_Read_CTdata_crc(u32 *ct);  // CRC校验后，读取AHT20的温度和湿度数据
static u8   Calc_CRC8(u8 *message, u8 Num);
/* Public function prototypes ------------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

void aht21Init(void)
{
    ath21GPIOConfig();
    AHT20_Init();  //初始化AHT20
    delay_ms(10);
    if ((AHT20_Read_Status() & 0x18) != 0x18) {
        AHT20_Start_Init();  //重新初始化寄存器
        delay_ms(10);
    }
}

void Sample_ATH21(void)
{
    char       buff[100] = {0};
    static u16 err_CNT   = 0;
    // AHT20_Read_CTdata(CT_data);  //不经过CRC校验，直接读取AHT20的温度和湿度数据    推荐每隔大于1S读一次
    AHT20_Read_CTdata_crc(CT_data);  // crc校验后，读取AHT20的温度和湿度数据
    if ((CT_data[0] == 0) && (CT_data[1] == 0)) {
        err_CNT++;
        sprintf(buff, "err_CNT:%d\r\n", err_CNT);
        PrintString1(buff);
    } else {
        err_CNT = 0;
        c1      = CT_data[0] * 100 * 10 / 1024 / 1024;        //计算得到湿度值c1（放大了10倍）
        temp1   = CT_data[1] * 200 * 10 / 1024 / 1024 - 500;  //计算得到温度值t1（放大了10倍）
        sprintf(buff, "c1:%d,t1:%d\r\n", c1, temp1);
        PrintString1(buff);
    }
}

static void ath21GPIOConfig(void)
{
    P3_MODE_IO_PU(GPIO_Pin_2 | GPIO_Pin_3);  // P3.2 P3.3 设置为准双向口
}

static u8 AHT20_Read_Status(void)  //读取AHT20的状态寄存器
{
    u8 Byte_first;
    I2C_Start();
    I2C_WriteAbyte(0x71);
    I2C_Check_ACK();
    Byte_first = I2C_ReadAbyte();
    S_NoACK();
    I2C_Stop();
    return Byte_first;
}
static void JH_Reset_REG(u8 addr)
{
    u8 Byte_first, Byte_second, Byte_third;
    I2C_Start();
    I2C_WriteAbyte(0x70);  //原来是0x70
    I2C_Check_ACK();
    I2C_WriteAbyte(addr);
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_Stop();

    delay_ms(5);  //延时5ms左右
    I2C_Start();
    I2C_WriteAbyte(0x71);  //
    I2C_Check_ACK();
    Byte_first = I2C_ReadAbyte();
    S_ACK();
    Byte_second = I2C_ReadAbyte();
    S_ACK();
    Byte_third = I2C_ReadAbyte();
    S_NoACK();
    I2C_Stop();

    delay_ms(10);  //延时10ms左右
    I2C_Start();
    I2C_WriteAbyte(0x70);  ///
    I2C_Check_ACK();
    I2C_WriteAbyte(0xB0 | addr);  ////寄存器命令
    I2C_Check_ACK();
    I2C_WriteAbyte(Byte_second);
    I2C_Check_ACK();
    I2C_WriteAbyte(Byte_third);
    I2C_Check_ACK();
    I2C_Stop();

    Byte_second = 0x00;
    Byte_third  = 0x00;
}

static void AHT20_Start_Init(void)
{
    JH_Reset_REG(0x1b);
    JH_Reset_REG(0x1c);
    JH_Reset_REG(0x1e);
}

static void AHT20_Init(void)  //初始化AHT20
{
    I2C_Start();
    I2C_WriteAbyte(0x70);
    I2C_Check_ACK();
    I2C_WriteAbyte(0xa8);  // 0xA8进入NOR工作模式
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_Stop();

    delay_ms(10);  //延时10ms左右

    I2C_Start();
    I2C_WriteAbyte(0x70);
    I2C_Check_ACK();
    I2C_WriteAbyte(0xbe);  // 0xBE初始化命令，AHT20的初始化命令是0xBE,   AHT10的初始化命令是0xE1
    I2C_Check_ACK();
    I2C_WriteAbyte(0x08);  //相关寄存器bit[3]置1，为校准输出
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_Stop();
    delay_ms(10);  //延时10ms左右
}
static void AHT20_SendAC(void)  //向AHT20发送AC命令
{
    I2C_Start();
    I2C_WriteAbyte(0x70);
    I2C_Check_ACK();
    I2C_WriteAbyte(0xac);  // 0xAC采集命令
    I2C_Check_ACK();
    I2C_WriteAbyte(0x33);
    I2C_Check_ACK();
    I2C_WriteAbyte(0x00);
    I2C_Check_ACK();
    I2C_Stop();
}

static void AHT20_Read_CTdata(u32 *ct)  //没有CRC校验，直接读取AHT20的温度和湿度数据
{
    volatile u8 Byte_1th = 0;
    volatile u8 Byte_2th = 0;
    volatile u8 Byte_3th = 0;
    volatile u8 Byte_4th = 0;
    volatile u8 Byte_5th = 0;
    volatile u8 Byte_6th = 0;
    u32         RetuData = 0;
    u16         cnt      = 0;
    AHT20_SendAC();  //向AHT10发送AC命令
    delay_ms(80);    //延时80ms左右
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80))  //直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    {
        delay_ms(2);
        if (cnt++ >= 100) {
            break;
        }
    }
    I2C_Start();
    I2C_WriteAbyte(0x71);
    I2C_Check_ACK();
    Byte_1th = I2C_ReadAbyte();  //状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    S_ACK();
    Byte_2th = I2C_ReadAbyte();  //湿度
    S_ACK();
    Byte_3th = I2C_ReadAbyte();  //湿度
    S_ACK();
    Byte_4th = I2C_ReadAbyte();  //湿度/温度
    S_ACK();
    Byte_5th = I2C_ReadAbyte();  //温度
    S_ACK();
    Byte_6th = I2C_ReadAbyte();  //温度
    S_NoACK();
    I2C_Stop();

    RetuData = (RetuData | Byte_2th) << 8;
    RetuData = (RetuData | Byte_3th) << 8;
    RetuData = (RetuData | Byte_4th);
    RetuData = RetuData >> 4;
    ct[0]    = RetuData;  //湿度
    RetuData = 0;
    RetuData = (RetuData | Byte_4th) << 8;
    RetuData = (RetuData | Byte_5th) << 8;
    RetuData = (RetuData | Byte_6th);
    RetuData = RetuData & 0xfffff;
    ct[1]    = RetuData;  //温度
}

static void AHT20_Read_CTdata_crc(u32 *ct)  // CRC校验后，读取AHT20的温度和湿度数据
{
    volatile u8 Byte_1th = 0;
    volatile u8 Byte_2th = 0;
    volatile u8 Byte_3th = 0;
    volatile u8 Byte_4th = 0;
    volatile u8 Byte_5th = 0;
    volatile u8 Byte_6th = 0;
    volatile u8 Byte_7th = 0;
    u32         RetuData = 0;
    u16         cnt      = 0;
    // u8  CRCDATA=0;
    u8 CTDATA[6] = {0};  //用于CRC传递数组

    AHT20_SendAC();  //向AHT10发送AC命令
    delay_ms(80);    //延时80ms左右
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80))  //直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    {
        delay_ms(2);
        if (cnt++ >= 100) {
            break;
        }
    }

    I2C_Start();
    I2C_WriteAbyte(0x71);
    I2C_Check_ACK();
    CTDATA[0] = Byte_1th = I2C_ReadAbyte();  //状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    S_ACK();
    CTDATA[1] = Byte_2th = I2C_ReadAbyte();  //湿度
    S_ACK();
    CTDATA[2] = Byte_3th = I2C_ReadAbyte();  //湿度
    S_ACK();
    CTDATA[3] = Byte_4th = I2C_ReadAbyte();  //湿度/温度
    S_ACK();
    CTDATA[4] = Byte_5th = I2C_ReadAbyte();  //温度
    S_ACK();
    CTDATA[5] = Byte_6th = I2C_ReadAbyte();  //温度
    S_ACK();
    Byte_7th = I2C_ReadAbyte();  // CRC数据
    S_NoACK();                   //注意: 最后是发送NAK
    I2C_Stop();

    if (Calc_CRC8(CTDATA, 6) == Byte_7th) {
        RetuData = (RetuData | Byte_2th) << 8;
        RetuData = (RetuData | Byte_3th) << 8;
        RetuData = (RetuData | Byte_4th);
        RetuData = RetuData >> 4;
        ct[0]    = RetuData;  //湿度
        RetuData = 0;
        RetuData = (RetuData | Byte_4th) << 8;
        RetuData = (RetuData | Byte_5th) << 8;
        RetuData = (RetuData | Byte_6th);
        RetuData = RetuData & 0xfffff;
        ct[1]    = RetuData;  //温度

    } else {
        ct[0] = 0x00;
        ct[1] = 0x00;  //校验错误返回值，客户可以根据自己需要更改
    }                  // CRC数据
}

// CRC校验类型：CRC8/MAXIM
//多项式：X8+X5+X4+1
// Poly：0011 0001  0x31
//高位放到后面就变成 1000 1100 0x8c
// C现实代码：
static u8 Calc_CRC8(u8 *message, u8 Num)
{
    u8 i;
    u8 byte;
    u8 crc = 0xFF;
    for (byte = 0; byte < Num; byte++) {
        crc ^= (message[byte]);
        for (i = 8; i > 0; --i) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}
