#ifndef  __LOGIC_IC_H
#define  __LOGIC_IC_H

#include "DataType/DataType.h"


#ifdef __cplusplus
extern "C" {
#endif
  

/*****************************************************************************
 * 74HC405x 多路X通道模拟开关
 ****************************************************************************/
 
/**
  * @brief  74HC595硬件控制接口初始化
  * @param  Data 私有数据
  * @param  SetINH INH设置
  * @param  SetSEL0 SEL0设置
  * @param  SetSEL1 SEL1设置
  * @param  SetSEL2 SEL2设置
  * @retval None
  */
void HC405x_HwCtrlInterFaces(void    *Data,
                             void    (*SetINH) (void *Data, uBit8 uState),
                             void    (*SetSEL0)(void *Data, uBit8 uState),
                             void    (*SetSEL1)(void *Data, uBit8 uState),
                             void    (*SetSEL2)(void *Data, uBit8 uState));
  

/**
  * @brief  405X使能(INH脚低电平有效)
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC405x_Enable(uBit8 isEnable);


/**
  * @brief  405X通道选择
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC405x_SetChannel(uBit8 uChannel);


/*****************************************************************************
 * 74HC595 串转并芯片
 ****************************************************************************/


typedef enum
{
    HC595_8_BIT   = 8,
    HC595_16_BIT  = 16,
    HC595_24_BIT  = 24,
    HC595_32_BIT  = 32,
    
}HC595_PARALLEL_BIT;


/**
  * @brief  74HC595硬件控制接口初始化
  * @param  Data 私有数据
  * @param  SetSHCP SHCP设置
  * @param  SetSTCP STCP设置
  * @param  SetDATA DATA设置
  * @param  HC_DelayUs 微秒级延时函数接口
  * @param  uParBitNum 并行位数
  * @retval None
  */
void HC595_HwCtrlInterFaces(void    *pData,
                            void    (*SetSHCP)(void *pData, uBit8 uState),
                            void    (*SetSTCP)(void *pData, uBit8 uState),
                            void    (*SetDATA)(void *pData, uBit8 uState),
                            void    (*HC_DelayUs)(uBit32 Us),
                            uBit8 uParBitNum);


/**
  * @brief  并型数据输出
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC595_OutputParallelData(uBit32 iSerialData);

  
#ifdef __cplusplus
}
#endif


#endif /* __LOGIC_IC_H */
