
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


#ifndef __CNCSYS_INTERFACE_H
#define __CNCSYS_INTERFACE_H


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
  
  
  int CNCSYS_InitInterface(CNCSYS_INTERFACE* pCNCSYSInterface);

  
  
  
  uBit32  CNCSYS_MainCtrl();

  
  
    /*
    函 数 名：int SYS_Init()
    功    能：系统初始化
    参    数：无
    返 回 值：无
    调用位置：系统上电时调用
    注意事项：无
    */
    int CNCSYS_Init();  
    
    
    /*
    函 数 名：void SYS_IPOInTime0();
    功    能：插补器插补中断服务函数
    参    数：无
    返 回 值：无
    调用位置：定时中断中调用
    注意事项：无
    */
    void SYS_IPOInTime0();

  
    /*
    函 数 名：void SYS_MainCtrl()
    功    能：系统模块主线程
    参    数：无
    返 回 值：无
    调用位置：大循环中调用
    注意事项：无
    */
    void SYS_MainCtrl();

    
    //系统复位
    //uBit32 SYS_Reset();
    
    /*
    函 数 名：void SYS_SetSysTickCount()
    功    能：更新系统tick
    参    数：无
    返 回 值：无
    调用位置：大循环中调用
    注意事项：无
    */
    void SYS_SetSysTickCount(void);
    
    /*
    函 数 名：void SYS_SetSysTickTimeCylce(uBit32 ulTimeCycle)
    功    能：设置系统tick时钟周期
    参    数：ulTimeCycle--时钟周期，单位us
    返 回 值：无
    调用位置：更新插补周期成功后调用
    注意事项：无
    */
    void SYS_SetSysTickTimeCylce(uBit32 ulTimeCycle);
    
    /*
    函 数 名：double64 SYS_GetSysTickCount(void);
    功    能：获取系统tick,单位ms
    参    数：无
    返 回 值：当前系统tick值
    调用位置：大循环中调用
    注意事项：无
    */
    double64 SYS_GetSysTickCount(void);


    /*
    函 数 名：const SYS_STATE_DATA* SYS_GetStateReadAddr()
    功    能：获取系统状态数据起始地址
    参    数：无
    返 回 值：无
    调用位置：大循环中调用
    注意事项：无
    */
    const SYS_STATE_DATA* SYS_GetStateReadAddr();
    
    //获取系统报警状态
    uBit32 SYS_GetAlarmStatus(); 
    //获取系统报警位状态
    Bit32 SYS_GetAlarmBit(uBit32 ulAlarmBit);
    //设置系统报警位
    void SYS_SetAlarmBit(uBit32 ulAlarmBit);
    //清除系统报警位
    void SYS_ClrAlarmBit(uBit32 ulAlarmBit);
    //获取系统运行状态字
    uBit32 SYS_GetSysStatus();    
    //设置系统运行状态位
      void SYS_SetStatusBit(uBit32 ulStatusBit);
    //清除系统运行状态位
    void SYS_ClrStatusBit(uBit32 ulStatusBit);
        

    //检测系统是否正在复位，正在复位返回1，否则返回0
    //Bit32 SYS_CheckResetState();
    

    
        /*
    函 数 名：Bit32 SPM_GetAxisMapTable(Bit32 nCh, Bit32 iAxisNo)
    功    能：获取通道逻辑轴号的物理轴号
    参    数：    nCh        --通道序号，从0开始
                iAxisNo    --轴序号，从0开始
    返 回 值：物理轴号，-1：错误
    调用位置：
    注意事项：
    */
    Bit32 SPM_GetAxisMapTable(Bit32 nCh, Bit32 iAxisNo);

    /*
    函 数 名：Bit32 SPM_GetAxisType(Bit32 nCh, Bit32 iAxisNo)
    功    能：获取轴的类型
    参    数：    nCh        --通道序号，从0开始
                iAxisNo    --轴序号，从0开始
    返 回 值：轴类型
                #define AXIS_TYPE_NULL            (0)            //未安装
                #define AXIS_TYPE_LINE            (1)            //直线轴
                #define AXIS_TYPE_ROTATE        (2)            //旋转轴(角度值任意)
                #define AXIS_TYPE_ROTATE_360    (3)            //旋转轴(角度值为0-360)
    调用位置：
    注意事项：
    */
    Bit32 SPM_GetAxisType(Bit32 nCh, Bit32 iAxisNo);
    
    
    
void SYS_InitStateData();


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

#endif /* __CNCSYS_INTERFACE_H */
