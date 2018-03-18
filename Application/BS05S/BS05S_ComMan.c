/**
  ******************************************************************************
  * @file    BS05S_ComMan.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.13
  * @brief   Motor driver communication manage
  ******************************************************************************
  * @attention
  * 
  * 电机启动: MOTOR_START.
  * 电机停止: MOTOR_STOP.
  * 电机加速: SPEED_ADD.
  * 电机减速: SPEED_REDUCE.
  * 顺时针转: MOTOR_CW.
  * 逆时针转: MOTOR_ACW.
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "BS05S_ComMan.h"
#include "BS05S_HardwareDef.h"
#include "BS05S_HwCtrl.h"

#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define BS05S_CMD_BUFF_LEN     (64)

#define BS05S_CMD_SET_AUTO_MODE_EN          "AUTO_MODE:EN"
#define BS05S_CMD_SET_AUTO_MODE_DIS         "AUTO_MODE:DIS"

#define BS05S_CMD_SET_LIGHT_EN               "LIGHT:EN"
#define BS05S_CMD_SET_LIGHT_DIS              "LIGHT:DIS"

static uBit8 uCmdBuff[BS05S_CMD_BUFF_LEN] = {0};   //定义指令缓冲区
     

/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理
  * @param  None
  * @retval None
  */
void BS05S_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //如果成功接收到数据
    if (UART_RecvBuff(BS05S_COM_UART_NODE, &uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            //自动模式使能
            if ((memcmp(uCmdBuff, BS05S_CMD_SET_AUTO_MODE_EN, ulRxIndex) == 0) && (ulRxIndex == strlen(BS05S_CMD_SET_AUTO_MODE_EN)))
            {
                //使能自动模式
                BS05S_EnableAutoOutputMode(true);
                
                //返回应答
                UART_SendStr(BS05S_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                
            }
            //自动模式关闭
            else if ((memcmp(uCmdBuff, BS05S_CMD_SET_AUTO_MODE_DIS, ulRxIndex) == 0) && (ulRxIndex == strlen(BS05S_CMD_SET_AUTO_MODE_DIS)))
            {
                //使能自动模式
                BS05S_EnableAutoOutputMode(false);
                
                //返回应答
                UART_SendStr(BS05S_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                
            }
            //灯光输出使能
            if ((memcmp(uCmdBuff, BS05S_CMD_SET_LIGHT_EN, ulRxIndex) == 0) && (ulRxIndex == strlen(BS05S_CMD_SET_LIGHT_EN)))
            {
                //使能自动模式
                BS05S_EnableLightOutput(true);
                
                //返回应答
                UART_SendStr(BS05S_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                
            }
            //灯光输出关闭
            else if ((memcmp(uCmdBuff, BS05S_CMD_SET_LIGHT_DIS, ulRxIndex) == 0) && (ulRxIndex == strlen(BS05S_CMD_SET_LIGHT_DIS)))
            {
                //使能自动模式
                BS05S_EnableLightOutput(false);
                
                //返回应答
                UART_SendStr(BS05S_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                
            }
            
            ulRxIndex = 0;
            return;
        }
        
        ulRxIndex++;
    }
    
    
}