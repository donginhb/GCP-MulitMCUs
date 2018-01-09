#ifndef  __CAN_DATA_STRUCT_DEF_H
#define  __CAN_DATA_STRUCT_DEF_H

#include "DataType/DataType.h"
#include "HAL/HAL_Can.h"

//CAN硬件参数定义
#define CAN_NODE_COUNT                      (2)                         //CAN节点数(每个节点分配一个独立的环形缓存)


//CAN缓冲区大小定义
#define CAN_TX_RING_BUFF_LEN                (8)                         //发送缓冲区深度
#define CAN_RX_RING_BUFF_LEN                (16)                        //接收缓冲区深度


//CAN数据类型定义
typedef HAL_CAN_FRAME_T                     CAN_FRAME_T;                //CAN 数据帧类型


//CAN数据报文参数定义
#define CAN_DATA_MAX_LEN                    (HAL_CAN_DATA_MAX_LEN)      //CAN最大数据区长度
#define CAN_MAX_EXT_ID                      (0x1FFFFFFF)                //最大扩展帧ID(29位)
#define CAN_MAX_STD_ID                      (0x000007FF)                //最大标准帧ID
#define CAN_EXT_ID_MASK                     (CAN_MAX_EXT_ID)            //CAN扩展帧ID掩码
#define CAN_STD_ID_MASK                     (CAN_MAX_STD_ID)            //CAN标准帧ID掩码

#define CAN_TYPE_DEF                        (HAL_CAN_TYPE_DEF)          //默认数据类型(标准帧,数据帧)
#define CAN_TYPE_REMOTE                     (HAL_CAN_TYPE_REMOTE)       //遥控帧
#define CAN_TYPE_EXT_FRAME                  (HAL_CAN_TYPE_EXT_FRAME)    //扩展帧


#endif /* __CAN_DATA_STRUCT_DEF_H */
