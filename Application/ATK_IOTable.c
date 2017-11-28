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
#include "ATK_IOTable.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    //按键
    {4,   4},
    {4,   3},
    {4,   2},
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    //LED灯
    {1,   5},
    {4,   5},
    
    //BEEP
    {1,   8},
    
    //IIC
    {1,   6},//SCL
    {1,   7},//SDA
    
    {6,  11},//DS18B20
};


//控制IO表
GPIO_CTRL_TABLE g_ATKIOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
