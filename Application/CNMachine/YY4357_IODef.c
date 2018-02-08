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
#include "YY4357_IODef.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"



//输入IO表
static GPIO_DATA m_InputIOTable[] = 
{
    {6,  8 },   //JOY_CENTER
    {6,  10},   //JOY_UP
    {6,  11},   //JOY_DOWN
    {6,  12},   //JOY_LEFT
    {6,  13},   //JOY_RIGHT
    
};


//输出IO表
static GPIO_DATA m_OutputIOTable[] = 
{
    {6,  24},
    {6,  25},
    {6,  26},
    {6,  27},
    {6,  28},
    //{4,  12},
    //{4,  13},
    //{4,  14},
    
};


//控制IO表
GPIO_CTRL_TABLE g_YY4357_IOTable = 
{
    m_OutputIOTable,
    m_InputIOTable,
    sizeof(m_OutputIOTable)/sizeof(GPIO_DATA),
    sizeof(m_InputIOTable)/sizeof(GPIO_DATA),
    
};
