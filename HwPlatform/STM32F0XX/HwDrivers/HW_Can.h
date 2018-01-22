#ifndef  __HW_CAN_H
#define  __HW_CAN_H

#include "chip.h"

#define HW_CANAF_MAX_ID_GROUP  (0x10)  //最大的ID过滤器组组数
#define HW_CANAF_ID_EXT_FRAME  (1)     //扩展帧
#define HW_CANAF_ID_STD_FRAME  (0)     //标准帧


//CAN节点定义
#define HW_CAN_NODE_0      (0)
#define HW_CAN_NODE_1      (1)


#ifdef __cplusplus
extern "C" {
#endif
    
/*****************************************************************************
 * CAN 基本功能接口函数
 ****************************************************************************/

/**
  * @brief  CAN总线初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval None
  */
void HW_CAN_Init(uint8_t uCanNode, uint32_t ulBitRate);


/**
  * @brief  CAN使能
  * @param  uCanNode CAN节点号
  * @param  bIsEnable 使能标志
  * @retval None
  */
void HW_CAN_Enable(uint8_t uCanNode, bool bIsEnable);


/**
  * @brief  数据发送
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_SendMsg(uint8_t uCanNode);


/**
  * @brief  数据接收
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_RecvMsg(uint8_t uCanNode);


#ifdef __cplusplus
}
#endif


#endif /* __HW_CAN_H */
