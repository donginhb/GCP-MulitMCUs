/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CN_ComMan.h"
#include "CN_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"

#include <string.h>
#include <stdio.h>

#define CN_CMD_BUFF_LEN     (64)

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


static uBit8 uCmdBuff[CN_CMD_BUFF_LEN] = {0};   //定义指令缓冲区
     
     
/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/


/**
  * @brief  通信管理单元初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 CN_ComInit(void)
{
    
    return 0;
}


/**
  * @brief  通信处理
  * @param  None
  * @retval None
  */
void CN_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //如果成功接收到数据
    if (UART_RecvBuff(CN_UART_NODE, &uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            UART_SendBuff(CN_UART_NODE, (uBit8 *)uCmdBuff, ulRxIndex);
            ulRxIndex = 0;
            return;
        }
        
        ulRxIndex++;
    }
      
}