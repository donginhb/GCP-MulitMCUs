/**
  ******************************************************************************
  * @file    GCPVersion.h
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 当前版本: GC10_0_2_D
  *  
  * 系统版本: GC10_0_1_D
  * 修改时间: 2017.11.28
  * 修改描述: 1.初步搭建好通用控制平台工程,目前已经支持LPC17XX,LPC43XX以及STM32F10X;
  *           2.将SysPeripheral中的SysCtrl修改成CoreCtrl,在一级目录下将SysInit修改成
  *             SysCtrl,在SysCtrl目录下,实现系统版本管理,系统初始化,功能配置等功能;
  *           3.初步移植进CMU模块,并可成功调用;
  *
  * 系统版本: GC10_0_2_D
  * 修改时间: 2017.11.29
  * 修改描述: 1.修复LPC17XX和LPC43XX在意外对UART的NODE进行多次初始化后,DMA发送会
  *           导致卡死的问题;
  *
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/

#ifndef __GCP_VERSION_H
#define __GCP_VERSION_H


//APP版本定义
#define GCP_VER_STR                     "GC10XXXD"      //项目版本
#define GCP_VER_NO0                     (0)             //大版本号
#define GCP_VER_NO1                     (2)             //小版本号
#define GCP_VER_PART                    (0)             //组件号(预留)
#define GCP_VER_FLAG                    'D'             //版本标示


#endif /* __SCL_VERSION_H */
