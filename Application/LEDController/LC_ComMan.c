/**
  ******************************************************************************
  * @file    LC_ComMan.c
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
#include "LC_ComMan.h"
#include "LC_HardwareDef.h"
#include "LC_MotorCtrl.h"

#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define LC_CLC_BUFF_LEN     (64)

#define LC_CLC_SET_MOTOR_START          "MOTOR_START."
#define LC_CLC_SET_MOTOR_STOP           "MOTOR_STOP."
#define LC_CLC_SET_SPEED_ADD            "SPEED_ADD."
#define LC_CLC_SET_SPEED_REDUCE         "SPEED_REDUCE."
#define LC_CLC_SET_MOTOR_CW             "MOTOR_CW."
#define LC_CLC_SET_MOTOR_ACW            "MOTOR_ACW."

static uBit8 uCmdBuff[LC_CLC_BUFF_LEN] = {0};   //定义指令缓冲区
     

/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理
  * @param  None
  * @retval None
  */
void LC_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //如果成功接收到数据
    if (UART_RecvBuff(LC_COM_UART_NODE, &uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            //电机启动
            if (memcmp(uCmdBuff, LC_CLC_SET_MOTOR_START, strlen(LC_CLC_SET_MOTOR_START)) == 0)
            {
                //执行指令
                LC_EnableMotor(true);
                
                //返回应答
                UART_SendStr(LC_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机停止
            else if (memcmp(uCmdBuff, LC_CLC_SET_MOTOR_STOP, strlen(LC_CLC_SET_MOTOR_STOP)) == 0)
            {
                //执行指令
                LC_EnableMotor(false);
                
                //返回应答
                UART_SendStr(LC_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机加速
            else if (memcmp(uCmdBuff, LC_CLC_SET_SPEED_ADD, strlen(LC_CLC_SET_SPEED_ADD)) == 0)
            {
                //参数判断
                if ((LC_GetMotorSpeedIndex()) > 0)
                {
                    //执行指令
                    LC_SetMotorSpeed(LC_GetMotorSpeedIndex() - 1);
                    
                    //返回应答
                    UART_SendStr(LC_COM_UART_NODE, "OK.");
                }
                else 
                {
                    //返回应答
                    UART_SendStr(LC_COM_UART_NODE, "MAX SPEED.");
                }
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机减速
            else if (memcmp(uCmdBuff, LC_CLC_SET_SPEED_REDUCE, strlen(LC_CLC_SET_SPEED_REDUCE)) == 0)
            {
                //参数判断
                if ((LC_GetMotorSpeedIndex() + 1) <= LC_GetMaxMotorSpeedIndex())
                {
                    //执行指令
                    LC_SetMotorSpeed(LC_GetMotorSpeedIndex() + 1);
                    
                    //返回应答
                    UART_SendStr(LC_COM_UART_NODE, "OK.");
                }
                else 
                {
                    //返回应答
                    UART_SendStr(LC_COM_UART_NODE, "MIN SPEED.");
                }
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //顺时针转
            else if (memcmp(uCmdBuff, LC_CLC_SET_MOTOR_CW, strlen(LC_CLC_SET_MOTOR_CW)) == 0)
            {
                //执行指令
                LC_SetMotorDirect(LC_MOTOR_DIRECT_CW);
                
                //返回应答
                UART_SendStr(LC_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //逆时针转
            else if (memcmp(uCmdBuff, LC_CLC_SET_MOTOR_ACW, strlen(LC_CLC_SET_MOTOR_ACW)) == 0)
            {
                //执行指令
                LC_SetMotorDirect(LC_MOTOR_DIRECT_ACW);
                
                //返回应答
                UART_SendStr(LC_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
        }
        
        ulRxIndex++;
    }
    
    
}