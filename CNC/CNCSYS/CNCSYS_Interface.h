
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_Interface.h

摘    要：系统管理模块接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月17日

使用说明：
                1、升级数据格式说明如下：
                    (1) 升级数据块包括：块头+数据
                    (2) 块头包含的信息有：当前块序号，当前块数据长度
                    (3) 第一块升级数据块具有两个作用：请求升级；发送升级文件头
                    (4) 最后一块升级数据块进行数据校验
*/


#ifndef __CNC_SYS_INTERFACE_H
#define __CNC_SYS_INTERFACE_H


#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/AXIS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"

#include "Version/Version.h"


typedef struct _CNCSYS_InterFace {

    uBit32 (*pf_CNCSYS_UpdateIPOTimeCycle)(uBit32 ulTimeCycleUs);

   // void （*pf_CAN_ExportInterface)(CAN_INTERFACE *pCanInterface);
    
}CNCSYS_INTERFACE;





#ifdef __cplusplus
extern "C" {
#endif
  
#if 0  //2017.12.05 Duhanfeng 暂时屏蔽
int CNCSYS_InitInterface(CNCSYS_INTERFACE* pCNCSYSInterface);
#endif

  
uBit32  CNCSYS_MainCtrl();

    
/**
  * @brief  CNC初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
int CNCSYS_Init(void);


void CNCSYS_IPOInSysTick(void);


    
/*****************************************************************************
 * CNC系统状态相关控制接口
 ****************************************************************************/

/**
  * @brief  系统状态数据初始化
  * @param  None
  * @retval None
  */
void CNCSYS_InitStateData(void);


/**
  * @brief  系统状态数据读取地址获取
  * @param  None
  * @retval 系统状态数据读取地址(指针)
  */
const SYS_STATE_DATA* CNCSYS_GetStateReadAddr(void);


/**
  * @brief  系统报警状态位设置
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void CNCSYS_SetAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位清除
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void CNCSYS_ClrAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
Bit32 CNCSYS_GetAlarmBit(uBit32 ulAlarmBit);


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
uBit32 CNCSYS_GetAlarmStatus(void);


/**
  * @brief  系统状态位获取
  * @param  Node
  * @retval 系统运行状态位
  */
uBit32 CNCSYS_GetSysStatus(void);


/**
  * @brief  系统状态位设置
  * @param  ulStatusBit 系统运行状态位
  * @retval Node
  */
void CNCSYS_SetStatusBit(uBit32 ulStatusBit);


/**
  * @brief  系统状态位清除
  * @param  ulStatusBit 系统运行状态位
  * @retval Node
  */
void CNCSYS_ClrStatusBit(uBit32 ulStatusBit);



#ifdef __cplusplus
}
#endif

#endif /* __CNC_SYS_INTERFACE_H */
