/**
  ******************************************************************************
  * @file    BS08M_ComMan.c
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
#include "BS08M_ComMan.h"
#include "BS08M_HardwareDef.h"
#include "BS08M_HwCtrl.h"

#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define BS08M_CMD_BUFF_LEN     (64)

#define BS08M_CMD_SET_TEMP_VALUE          "TEMP:"

static uBit8 uCmdBuff[BS08M_CMD_BUFF_LEN] = {0};   //定义指令缓冲区
     

/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理
  * @param  None
  * @retval None
  */
void BS08M_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //如果成功接收到数据
    if (UART_RecvBuff(BS08M_COM_UART_NODE, &uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            uBit32 ulCmdLen = strlen(BS08M_CMD_SET_TEMP_VALUE);
            
            //获取温度值
            if (memcmp(uCmdBuff, BS08M_CMD_SET_TEMP_VALUE, ulCmdLen) == 0)
            {
                uBit8 uDataBuff[16] = {0};
                uBit32 ulTempValue = 0;
                
                //获取温度值
                for (int i = 0; i < (ulRxIndex - ulCmdLen); i++)
                {
                    uDataBuff[i] = uCmdBuff[ulCmdLen + i];
                }
                
                ulTempValue = atoi((const char *)uDataBuff);
                
                //设置温度值
                BS08M_SetTempValue(ulTempValue);
                
                //返回应答
                UART_SendStr(BS08M_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                
            }
            
            ulRxIndex = 0;
            return;
        }
        
        ulRxIndex++;
    }
    
    
}