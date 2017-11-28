#ifndef  __DS18B20_H
#define  __DS18B20_H  


#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * DS18B20 相关控制接口
 ****************************************************************************/

/**
  * @brief  DS18B20 硬件接口初始化
  * @param  pf_DS_SetBus    总线状态设置
  * @param  pf_DS_GetBus    总线状态获取
  * @param  pf_DS_DelayUs   微秒级延时函数
  * @retval 0-成功 非0-失败
  */
uBit32 DS18B20_InitInterfaces(void (*pf_DS_SetBus) (bool bStates),
                              bool (*pf_DS_GetBus) (void),
                              void (*pf_DS_DelayUs)(uBit32 ulUs));


/**
  * @brief  温度转换启动函数
  * @param  None
  * @retval Note
  */
void DS18B20_StartConvertTemp(void);


/**
  * @brief  温度数值获取函数
  * @param  None
  * @retval fTempVal  转换后的温度值,单位:摄氏度
  */
float DS18B20_GetTempVal(void);




#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */
