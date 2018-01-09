#ifndef  __HAL_CAN_H
#define  __HAL_CAN_H

#include "DataType/DataType.h"


//CAN数据报文参数定义
#define HAL_CAN_DATA_MAX_LEN                    (8)                 //CAN最大数据区长度

//数据帧类型定义
#define HAL_CAN_TYPE_DEF                        (0x00)              //默认数据类型(标准帧,数据帧)
#define HAL_CAN_TYPE_REMOTE                     (0x01)              //遥控帧(相对于数据帧)
#define HAL_CAN_TYPE_EXT_FRAME                  (0x02)              //扩展帧(相对于标准帧)


//数据帧报文结构定义
typedef struct
{
    uBit8  uType;                                       //帧类型及格式 
    uBit8  uDLC;                                        //数据场长度(范围:0~8)
    uBit8  uReserver[2];                                //预留位(为了字节对齐)
    uBit32 ulID;                                        //CAN报文ID
    uBit8  uData[HAL_CAN_DATA_MAX_LEN];                 //报文数据区
    
}HAL_CAN_FRAME_T;



#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CAN总线初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval None
  */
void HAL_CAN_Init(uBit8 uCanNode, uBit32 ulBitRate);


/**
  * @brief  CAN打开
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 HAL_CAN_Open(uBit8 uCanNode);


/**
  * @brief  CAN关闭
  * @param  uCanNode CAN节点号
  * @retval 0-成功  非0-失败
  */
uBit32 HAL_CAN_Close(uBit8 uCanNode);


/**
  * @brief  CAN数据发送
  * @param  uCanNode CAN节点号
  * @param  pSendFrame 发送帧数据
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_CAN_Send(uBit8 uCanNode, HAL_CAN_FRAME_T *pSendFrame);


/**
  * @brief  CAN数据接收
  * @param  uCanNode CAN节点号
  * @param  pRecvFrame 接收帧数据
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_CAN_Recv(uBit8 uCanNode, HAL_CAN_FRAME_T *pRecvFrame);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_CAN_H */
