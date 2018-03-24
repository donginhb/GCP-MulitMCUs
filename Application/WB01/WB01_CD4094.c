/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.02.01
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "WB01_CD4094.h"
#include "WB01_HardwareDef.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  简单的延时函数
  * @param  ulDelay 延时参数,无标准单位,入参范围[1-0xFFFFFFFF]
  * @retval None
  * @note   在96MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  *         在204MHz的系统中测试,入参10,大概延时1us.误差小于+5%;
  */
static void CD4094_SimpleDelay(void)
{
    for(volatile int i = 0; i < 1000; i++);
    
}



/*****************************************************************************
 * CD4094相关控制接口
 ****************************************************************************/

/**
  * @brief  CD4094 设置
  * @param  nValue 要写入的数据
  * @retval None
  */
void WB01_CD4094_WriteValue(uBit16 nValue)
{
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_OE, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_RCK, false);
    CD4094_SimpleDelay();
    
    for (int i = 0; i < 16; i++)
    {
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SCK, false);
        
        if (nValue & 0x8000)
        {
            GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SI, true);
        }
        else 
        {
            GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SI, false);
        }
        CD4094_SimpleDelay();
        
        nValue <<= 1;
        
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SCK, true);
        CD4094_SimpleDelay();
    }
      
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_RCK, true);
    CD4094_SimpleDelay();
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_RCK, false);
    
    
}



