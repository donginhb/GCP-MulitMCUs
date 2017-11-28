#ifndef  __HW_CAN_H
#define  __HW_CAN_H

#include "chip.h"

#define HW_CANAF_MAX_ID_GROUP  (0x10)  //最大的ID过滤器组组数
#define HW_CANAF_ID_EXT_FRAME  (1)     //扩展帧
#define HW_CANAF_ID_STD_FRAME  (0)     //标准帧

//CAN 过滤器ID范围类型定义
typedef struct 
{
    uint32_t ulLowerID;   //ID上限
    uint32_t ulUpperID;   //ID下限
    
}HW_CANAF_ID_RANGE_T;


//CAN 过滤器配置数据类型定义
typedef struct 
{
    HW_CANAF_ID_RANGE_T IDRange[HW_CANAF_MAX_ID_GROUP];   //过滤器规则组
    uint16_t nEntryNum;     //过滤器规则组数
    uint16_t nFrameMode;    //帧模式: 1-扩展帧  0-标准帧

}HW_CANAF_ID_GROUP_T;


//CAN节点数
typedef enum
{
    HW_CAN_NODE0 = 0,
    HW_CAN_NODE1,
    HW_CAN_NODE_NUM,
    
}HW_CAN_NODE;


typedef CAN_MSG_T HW_CAN_MSG_TYPE;


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
  * @brief  数据发送
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_SendMsg(uint8_t uCanNode, HW_CAN_MSG_TYPE *pMsgObj);


/**
  * @brief  数据接收
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_RecvMsg(uint8_t uCanNode, HW_CAN_MSG_TYPE *pMsgObj);


/*****************************************************************************
 * CAN 过滤器接口函数
 ****************************************************************************/

/**
  * @brief  CAN ID 接收过滤器组设置
  * @param  uCanNode CAN节点号
  * @param  GrpEntry 过滤器组
  * @retval None
  */
void HW_CAN_SetupRecvIDGroup(uint8_t uCanNode, HW_CANAF_ID_GROUP_T *GrpEntry);


/**
  * @brief  CAN ID 接收ID过滤器复位
  * @param  None
  * @retval None
  */
void HW_CAN_ResetRecvIDGroup(void);


#ifdef __cplusplus
}
#endif


#endif /* __HW_CAN_H */
