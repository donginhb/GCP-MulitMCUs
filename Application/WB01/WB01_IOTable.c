/**
  ******************************************************************************
  * @file    GCIOCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
/***********************************<INCLUDES>**********************************/
#include "WB01_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //主轴电机检测
    {GPIO_PORT_C,  5},  //上限位
    {GPIO_PORT_A,  4},  //下限位
    {GPIO_PORT_A,  5},  //停止位
    
    //霍尔检测
    {GPIO_PORT_A, 13},
    
    //进货门电机
    {GPIO_PORT_C,  1},  //开门开关
    {GPIO_PORT_C,  2},  //关门开关
    
    //出货门电机
    {GPIO_PORT_C,  3},  //开门开关
    {GPIO_PORT_C,  4},  //关门开关
    
    //进出货门红外检测
    {GPIO_PORT_C,  6},
    
    //人体红外检测
    {GPIO_PORT_C,  7},
    
    //货道电机限位检测
    {GPIO_PORT_C,  0},
    
    //光感检测
    {GPIO_PORT_A, 14},
    
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {GPIO_PORT_A,  8},
    
    //主轴电机控制
#if 0
    {GPIO_PORT_B,  8},  //正转
    {GPIO_PORT_B,  9},  //反转
    {GPIO_PORT_B, 15},  //刹车
#else 
    {GPIO_PORT_B,  9},  //步进电机方向信号
    {GPIO_PORT_B, 15},  //脱机信号
#endif
    
    //传输皮带电机
    {GPIO_PORT_B, 13},  
    
    //进货门电机
    {GPIO_PORT_A,  7},
    {GPIO_PORT_A,  6},
    
    //出货门电机
    {GPIO_PORT_B,  5},
    {GPIO_PORT_B,  4},
    
    //货道电机控制(74HC595)
    {GPIO_PORT_C,  8},  //HC_SCK
    {GPIO_PORT_C,  9},  //HC_RCK
    {GPIO_PORT_C, 10},  //HC_OE
    {GPIO_PORT_C, 11},  //HC_SI
    
    //光栅供电开关
    {GPIO_PORT_C, 13},
    
};


//控制IO表
GPIO_CTRL_TABLE g_WB01_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
