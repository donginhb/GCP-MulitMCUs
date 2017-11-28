#ifndef  __TM1617_H
#define  __TM1617_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * TM1617数码管驱动IC控制接口
 ****************************************************************************/

/**
  * @brief  TM1617控制引脚编号设置
  * @param  pf_SetDIO 数据引脚控制接口
  * @param  pf_SetCLK 时钟引脚控制接口
  * @param  pf_SetSTB 片选引脚控制接口
  * @retval None
  */
void TM1617_SetCtrlPin(void (*pf_SetDIO)(bool bState), 
                       void (*pf_SetCLK)(bool bState), 
                       void (*pf_SetSTB)(bool bState));


/**
  * @brief  数码管数据显示函数
  * @param  uHexData 要显示的数据
  * @retval 0-成功 非0-失败
  */
uBit32 TM1617_DisNumber(uBit8 uHexData);


#ifdef __cplusplus
}
#endif

#endif /* __TM1617_H */
