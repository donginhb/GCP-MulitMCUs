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
  * 当前版本: GC10_0_5_D
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
  * 系统版本: GC10_0_3_D
  * 修改时间: 2017.12.02
  * 修改描述: 1.实现CMU中GPIO的控制接口,新增由IO编号来控制的接口,以避免原接口的
  *             限制(原接口最大只支持32个IO,且读取IO状态的接口效率低下);
  * 
  * 系统版本: GC10_0_4_D
  * 修改时间: 2017.12.05
  * 修改描述: 1.实现LPC17XX平台的定时器输出PWM(比较模式)以及PWM计数两个功能,并
  *             在SysPeripheral/PWM/PWM_MotorCtrl中实现PWM输出;
  * 
  * 系统版本: GC10_0_5_D
  * 修改时间: 2017.12.06
  * 修改描述: 1.成功移植入LPC17XX平台的CNC模块,并实现基本的运动控制功能;
  * 
  * 系统版本: GC10_0_6_D
  * 修改时间: 2017.12.09
  * 修改描述: 1.完善LPC17XX FLASH操作的接口;
  *           2.编写Bootloader,兼容单双核及Bootloader本身的烧录;
  *           3.一级目录下添加文件夹"Algorithm",主要用于常规的算法;
  *           4.在Algorithm路径下添加CRC库及JSON库;
  * 
  * 系统版本: GC10_0_7_D
  * 修改时间: 2017.12.18
  * 修改描述: 1.扩展CMU的相关接口
  *           2.修复LPC17XX EXTI 接口中,只能使能一路中断的BUG;
  * 
  * 系统版本: GC10_0_8_R
  * 修改时间: 2017.12.20
  * 修改描述: 1.增加LPC17XX ADC相关控制接口
  *           2.当前版本作为LPC17XX的发布版本
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/

#ifndef __GCP_VERSION_H
#define __GCP_VERSION_H


//APP版本定义
#define GCP_VER_STR                     "GC10XXXX"      //项目版本
#define GCP_VER_NO0                     (0)             //大版本号
#define GCP_VER_NO1                     (8)             //小版本号
#define GCP_VER_PART                    (0)             //组件号(预留)
#define GCP_VER_FLAG                    'R'             //版本标示


#endif /* __SCL_VERSION_H */
