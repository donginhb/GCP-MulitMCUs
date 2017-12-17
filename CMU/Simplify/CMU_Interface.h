#ifndef __CMU_INTERFACE_H
#define __CMU_INTERFACE_H

#include "CMU/Simplify/AbstractionLayer/CMU_UartInterface.h"
#include "CMU/Simplify/AbstractionLayer/CMU_EnetInterface.h"
#include "CMU/Simplify/AbstractionLayer/CMU_CanInterface.h"
#include "CMU/Simplify/CMU_ExApi.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CMU 通信相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uCanNode CAN节点
  * @param  pCanInterface CAN接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_CAN_SetInterface(uBit8 uCanNode, CMU_CAN_INTERFACE *pCanInterface);


/**
  * @brief  CMU ENET 接口设置
  * @param  pEnetInterface 以太网接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_ENET_SetInterface(CMU_ENET_INTERFACE *pEnetInterface);


/**
  * @brief  CMU UART 接口设置
  * @param  uUartNode 串口节点
  * @param  pUartInterface 串口接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_UART_SetInterface(uBit8 uUartNode, CMU_UART_INTERFACE *pUartInterface);


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
 * CMU 外部函数配置相关控制接口
 ****************************************************************************/

/**
  * @brief  外部函数设置
  * @param  pCmuExternFunTable 外部函数指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE *pCmuExternFunTable);


/**
  * @brief  外部函数初始化
  * @param  None
  * @retval None
  * @note   应当在调用CMU_SetExternFun之后才调用此函数
  */
void CMU_InitExApi(void);


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
