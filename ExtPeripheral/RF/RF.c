/**
  ******************************************************************************
  * @file    RF.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.18
  * @brief   Radio frequency communication application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "RF.h"
#include "NRF24L01.h"
#include "DataType/DataType.h"
//#include "SysPeripheral/SPI/SPI.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit32 m_ulIRQNo  = 0;
static uBit32 m_ulCSNNo = 0;
static uBit32 m_ulCENo   = 0;


/**
  * @brief  数码管DIO引脚设置
  * @param  None
  * @retval IRQ电平状态
  */
static bool GetIRQ(void)
{
    
    return GPIO_GetInputState(m_ulIRQNo);
}


/**
  * @brief  数码管CLK引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetCSN(bool bState)
{
    GPIO_SetOutputState(m_ulCSNNo, bState);
    
}


/**
  * @brief  数码管STB引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetCE(bool bState)
{
    GPIO_SetOutputState(m_ulCENo, bState);
    
}



/*****************************************************************************
 * RF相关控制接口
 ****************************************************************************/


/**
  * @brief  数码管控制引脚编号设置
  * @param  uSpiNode SPI节点号
  * @param  ulIRQNo  IRQ引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCSNNo  CSN引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCENo   CE引脚编号(IO资源表中输出IO组的位置)
  * @retval 0-成功 非0-失败
  */
uBit32 RF_SetCtrlPinNO(uBit8 uSpiNode, uBit32 ulIRQNo, uBit32 ulCSNNo, uBit32 ulCENo)
{
    m_ulIRQNo = ulIRQNo;
    m_ulCSNNo = ulCSNNo;
    m_ulCENo = ulCENo;
    
    nRF24L01_HwCtrlInterFaces(NULL,
                                 SetCSN,
                                 SetCE,
                                 GetIRQ,
                                 SysTime_DelayUs);
    
    return 0;
}
