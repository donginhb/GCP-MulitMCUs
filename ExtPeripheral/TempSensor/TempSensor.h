#ifndef  __TEMP_SENSOR_H
#define  __TEMP_SENSOR_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

/*****************************************************************************
 * 温度传感器相关控制接口
 ****************************************************************************/

/**
  * @brief  温度模块接口初始化
  * @param  ulBusPinNO 总线IO在资源表中的编号
  * @param  pf_DelayUs 微秒级延时函数
  * @retval 0-成功 非0-失败
  */
uBit32 TEMP_InitInterface(uBit32 ulBusPinNO, void (*pf_DelayUs)(uBit32 ulUs));


/**
  * @brief  温度转化
  * @param  None
  * @retval None
  */
void TEMP_StartConvert(void);


/**
  * @brief  获取温度值
  * @param  None
  * @retval 实际上的温度值
  */
float TEMP_GetValue(void);


#ifdef __cplusplus
}
#endif

#endif /* __TEMP_SENSOR_H */
