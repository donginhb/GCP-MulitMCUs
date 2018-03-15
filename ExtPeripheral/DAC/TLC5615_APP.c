/**
  ******************************************************************************
  * @file    TLC5615_APP.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.03.15
  * @brief   Digital-to-Analog Converter IC Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "TLC5615_APP.h"
#include "TLC5615.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit32 m_ulDataPinNO  = 0;
static uBit32 m_ulClockPinNO = 0;
static uBit32 m_ulCsPinNO   = 0;


/**
  * @brief  DIO引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_DIO(bool bState)
{
    GPIO_SetOutputState(m_ulDataPinNO, bState);
    
}


/**
  * @brief  CLK引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_CLK(bool bState)
{
    GPIO_SetOutputState(m_ulClockPinNO, bState);
    
}


/**
  * @brief  STB引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_CS(bool bState)
{
    GPIO_SetOutputState(m_ulCsPinNO, bState);
    
}


/*****************************************************************************
 * TLC5615数模转换IC控制接口
 ****************************************************************************/

/**
  * @brief  TLC5615 接口初始化
  * @param  ulDataPinNO  数据引脚编号(IO资源表中输出IO组的位置)
  * @param  ulClockPinNO 时钟引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCsPinNO    片选引脚编号(IO资源表中输出IO组的位置)
  * @retval 0-成功  非0-失败
  */
uBit32 TLC5615_APP_InitInterface(uBit32 ulDataPinNO, uBit32 ulClockPinNO, uBit32 ulCsPinNO)
{
    m_ulDataPinNO  = ulDataPinNO ;
    m_ulClockPinNO = ulClockPinNO;
    m_ulCsPinNO   = ulCsPinNO  ;
    
    return TLC5615_InitInterface(TLC5615_DIO, TLC5615_CLK, TLC5615_CS);
}


/**
  * @brief  TLC5615模拟量输出设置
  * @param  ulWriteData 数字量数据,范围为[0,0x3FF],对应[0,2Vef]
  * @retval 0-成功 非0-失败
  */
uBit32 TLC5615_APP_SetAnalogOutput(uBit32 ulWriteData)
{
    
    return TLC5615_SetAnalogOutput(ulWriteData);
}
