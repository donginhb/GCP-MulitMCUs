/**
  ******************************************************************************
  * @file    Eeprom.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.10
  * @brief   Eeprom drivers
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Eeprom.h"
#include "at24cxx.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define EEPROM_CHECK_BLOCK_SIZE     (16)

static uBit32 m_ulDataPinNo = 0;    //数据引脚编号
static uBit32 m_ulClockPinNo = 0;   //时钟引脚编号


//设置SDA状态
static void SET_SDA(void *data, uBit8 state)
{
    GPIO_SetOutputState(m_ulDataPinNo, state);
    
}

//设置SCL状态
static void SET_SCL(void *data, uBit8 state)
{
    GPIO_SetOutputState(m_ulClockPinNo, state);
    
}

//获取SDA状态
static uBit8 GET_SDA(void *data)
{
    
    return GPIO_GetOutputState(m_ulDataPinNo);
}

//获取SCL状态
static uBit8 GET_SCL(void *data)
{
    
    return GPIO_GetOutputState(m_ulClockPinNo);
}

//简单的延时函数
static void I2C_Delay(void *data)
{
    SysTime_SimpleDelay(100);   //大约是10US
    
}


/*****************************************************************************
 * EEPROM相关控制接口
 ****************************************************************************/

/**
  * @brief  EEPROM 读写有效校验
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 EEPROM_CheckRW(void)
{
    uBit32 ulRet = 0;
    
    uBit8 uBackupBuff[EEPROM_CHECK_BLOCK_SIZE] = {0};
    uBit8 uRecvBuff[EEPROM_CHECK_BLOCK_SIZE]   = {0};
    uBit8 uWriteBuff[EEPROM_CHECK_BLOCK_SIZE]  = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                                                  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    
    //备份原有数据
    EEPROM_ReadBuff(0, uBackupBuff, EEPROM_CHECK_BLOCK_SIZE);
    
    //写入测试数据
    EEPROM_WriteBuff(0, uWriteBuff, EEPROM_CHECK_BLOCK_SIZE);
    
    //读回测试数据
    EEPROM_ReadBuff(0, uRecvBuff, EEPROM_CHECK_BLOCK_SIZE);
    
    //数据比较
    for (int i = 0; i < EEPROM_CHECK_BLOCK_SIZE; i++)
    {
        if (uRecvBuff[i] != uWriteBuff[i])
        {
            ulRet = 1;
            break;
        }
    }
    
    //恢复备份的数据
    EEPROM_WriteBuff(0, uBackupBuff, EEPROM_CHECK_BLOCK_SIZE);
    
    return ulRet;
}


/**
  * @brief  EEPROM初始化
  * @param  ulDataPinNO 数据控制脚编号
  * @param  ulClockPinNO 时钟控制脚编号
  * @retval 0-成功 非0-失败
  */
uBit32 EEPROM_Init(uBit32 ulDataPinNO, uBit32 ulClockPinNO)
{
     m_ulDataPinNo  = ulDataPinNO ;
     m_ulClockPinNo = ulClockPinNO;
     
     AT24Cxx_InitInterface(NULL, SET_SDA, SET_SCL, GET_SDA, GET_SCL, I2C_Delay, AT24C02_PAGE_SIZE);
     
     return EEPROM_CheckRW();
}


/**
  * @brief  EEPROM数据读取
  * @param  nAddr     存储地址
  * @param  pReadBuff 接收缓冲区
  * @param  ulSize    要读取的长度
  * @retval None
  */
void EEPROM_ReadBuff(uBit16 nAddr, uBit8 *pReadBuff, uBit32 ulSize)
{
    AT24Cxx_ReadMultiBytes(nAddr, pReadBuff, ulSize);
    
}


/**
  * @brief  EEPROM数据写入
  * @param  nAddr     存储地址
  * @param  pReadBuff 写入缓冲区
  * @param  ulSize    要写入的长度
  * @retval None
  */
void EEPROM_WriteBuff(uBit16 nAddr, const uBit8 *pWriteBuff, uBit32 ulSize)
{
    AT24Cxx_WriteMultiBytes(nAddr, pWriteBuff, ulSize);
    
}

