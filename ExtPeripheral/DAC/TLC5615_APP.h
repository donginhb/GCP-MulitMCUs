#ifndef  __TLC5615_H
#define  __TLC5615_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * TLC5615数模转换IC控制接口
 ****************************************************************************/

/**
  * @brief  TM1617 接口初始化
  * @param  pf_SetDIO 数据引脚控制接口
  * @param  pf_SetCLK 时钟引脚控制接口
  * @param  pf_SetCS 片选引脚控制接口
  * @retval 0-成功  非0-失败
  */
uBit32 TLC5615_InitInterface(void (*pf_SetDIO)(bool bState), 
                             void (*pf_SetCLK)(bool bState), 
                             void (*pf_SetCS)(bool bState));


/**
  * @brief  TLC5615模拟量输出设置
  * @param  ulWriteData 数字量数据,范围为[0,0x3FF],对应[0,2Vef]
  * @retval 0-成功 非0-失败
  */
uBit32 TLC5615_SetAnalogOutput(uBit32 ulWriteData);


#ifdef __cplusplus
}
#endif

#endif /* __TLC5615_H */
