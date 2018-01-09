#ifndef  __GCP_EEEOR_MANA_H
#define  __GCP_EEEOR_MANA_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 系统报警状态相关控制接口
 ****************************************************************************/

/**
  * @brief  系统报警状态位设置
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void GCP_SetAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位清除
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void GCP_ClrAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
Bit32 GCP_GetAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
uBit32 GCP_GetAlarmStatus(void);



#ifdef __cplusplus
}
#endif

#endif /* __GCP_EEEOR_MANA_H */
