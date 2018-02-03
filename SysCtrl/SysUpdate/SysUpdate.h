#ifndef  __SYS_UPDATE_H
#define  __SYS_UPDATE_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
/*****************************************************************************
 * 系统Bootloader上电处理接口
 ****************************************************************************/

/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UPDATE_Init(void);


/**
  * @brief  系统更新初始化
  * @param  None
  * @retval 0-成功 非0-失败
  * @note   本函数决定是跳转到APP还是执行Bootloader
  */
uBit32 SYS_UPDATE_JumToApp(void);

    
/*****************************************************************************
 * 系统更新相关管理接口
 ****************************************************************************/

/**
  * @brief  系统APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateAPP(uBit8 *pBuff, uBit32 ulSize);


/**
  * @brief  系统副APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateSubAPP(uBit8 *pBuff, uBit32 ulSize);


/**
  * @brief  系统Bootloader更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateBootloader(uBit8 *pBuff, uBit32 ulSize);


/**
  * @brief  系统主APP存在标志清除
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_ClearAppFlag(void);


/**
  * @brief  系统副APP存在标志清除
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_ClearSubAppFlag(void);


/*****************************************************************************
 * Bootloader LED显示线程接口
 ****************************************************************************/

/**
  * @brief  LED灯设置
  * @param  ulLedPort Led端口
  * @param  ulLedPin Led引脚
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UPDATE_SetMainWorkLed(uBit32 ulLedPort, uBit32 ulLedPin);


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void SYS_UPDATE_ShowMainWorkLed(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_UPDATE_H */
