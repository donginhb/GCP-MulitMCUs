#ifndef __CMU_INTERFACE_H
#define __CMU_INTERFACE_H

#include "CMU/Simplify/CMU_UartInterface.h"
#include "CMU/Simplify/CMU_CanInterface.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CMU 通信相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uUartNode 串口节点
  * @param  pUartInterface 串口接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_UART_SetInterface(uBit8 uUartNode, CMU_UART_INTERFACE *pUartInterface);


/**
  * @brief  CMU UART 接口设置
  * @param  uCanNode CAN节点
  * @param  pCanInterface CAN接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_CAN_SetInterface(uBit8 uCanNode, CMU_CAN_INTERFACE *pCanInterface);


/*****************************************************************************
 * CMU 初始化相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU模块初始化
  * @param  uComType 端口类型
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_Init(uBit8 uComType);


/*****************************************************************************
 * CMU 主任务处理相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU通信管理
  * @param  None
  * @retval None
  */
void CMU_MainProc(void);


#ifdef __cplusplus
}
#endif


#endif /* __CMU_INTERFACE_H */
