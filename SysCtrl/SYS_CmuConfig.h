#ifndef  __SYS_CMU_CONFIG_H
#define  __SYS_CMU_CONFIG_H

#include "../DataType/DataType.h"
#include "SysConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/**
  * @brief  系统初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void);


/**
  * @brief  CMU通信端口设置
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_SetCmuCom(uBit32 ulComMode, uBit8 uComNode, uBit32 ulBaudRate);


/**
  * @brief  CMU默认串口通信设置
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_SetCmuDefUartCom(void);


/**
  * @brief  CMU默认CAN通信设置
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_SetCmuDefCanCom(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_CMU_CONFIG_H */
