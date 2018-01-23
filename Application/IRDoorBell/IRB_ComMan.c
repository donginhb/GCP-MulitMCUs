/**
  ******************************************************************************
  * @file    IRB_ComMan.c
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
#include "IRB_ComMan.h"
#include "IRB_HardwareDef.h"
#include "IRB_MotorCtrl.h"

#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define IRB_CIRB_BUFF_LEN     (64)

#define IRB_CIRB_SET_MOTOR_START          "MOTOR_START."
#define IRB_CIRB_SET_MOTOR_STOP           "MOTOR_STOP."
#define IRB_CIRB_SET_SPEED_ADD            "SPEED_ADD."
#define IRB_CIRB_SET_SPEED_REDUCE         "SPEED_REDUCE."
#define IRB_CIRB_SET_MOTOR_CW             "MOTOR_CW."
#define IRB_CIRB_SET_MOTOR_ACW            "MOTOR_ACW."

static uBit8 uCmdBuff[IRB_CIRB_BUFF_LEN] = {0};   //定义指令缓冲区
     

/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理
  * @param  None
  * @retval None
  */
void IRB_ComHandler(void)
{
    static uBit32 ulRxIndex = 0;
    
    //如果成功接收到数据
    if (UART_RecvBuff(IRB_COM_UART_NODE, &uCmdBuff[ulRxIndex], 1))
    {
        //如果已经接收到结束符,则进行指令处理
        if (uCmdBuff[ulRxIndex] == '.')
        {
            //电机启动
            if (memcmp(uCmdBuff, IRB_CIRB_SET_MOTOR_START, strlen(IRB_CIRB_SET_MOTOR_START)) == 0)
            {
                //执行指令
                IRB_EnableMotor(true);
                
                //返回应答
                UART_SendStr(IRB_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机停止
            else if (memcmp(uCmdBuff, IRB_CIRB_SET_MOTOR_STOP, strlen(IRB_CIRB_SET_MOTOR_STOP)) == 0)
            {
                //执行指令
                IRB_EnableMotor(false);
                
                //返回应答
                UART_SendStr(IRB_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机加速
            else if (memcmp(uCmdBuff, IRB_CIRB_SET_SPEED_ADD, strlen(IRB_CIRB_SET_SPEED_ADD)) == 0)
            {
                //参数判断
                if ((IRB_GetMotorSpeedIndex()) > 0)
                {
                    //执行指令
                    IRB_SetMotorSpeed(IRB_GetMotorSpeedIndex() - 1);
                    
                    //返回应答
                    UART_SendStr(IRB_COM_UART_NODE, "OK.");
                }
                else 
                {
                    //返回应答
                    UART_SendStr(IRB_COM_UART_NODE, "MAX SPEED.");
                }
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //电机减速
            else if (memcmp(uCmdBuff, IRB_CIRB_SET_SPEED_REDUCE, strlen(IRB_CIRB_SET_SPEED_REDUCE)) == 0)
            {
                //参数判断
                if ((IRB_GetMotorSpeedIndex() + 1) <= IRB_GetMaxMotorSpeedIndex())
                {
                    //执行指令
                    IRB_SetMotorSpeed(IRB_GetMotorSpeedIndex() + 1);
                    
                    //返回应答
                    UART_SendStr(IRB_COM_UART_NODE, "OK.");
                }
                else 
                {
                    //返回应答
                    UART_SendStr(IRB_COM_UART_NODE, "MIN SPEED.");
                }
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //顺时针转
            else if (memcmp(uCmdBuff, IRB_CIRB_SET_MOTOR_CW, strlen(IRB_CIRB_SET_MOTOR_CW)) == 0)
            {
                //执行指令
                IRB_SetMotorDirect(IRB_MOTOR_DIRECT_CW);
                
                //返回应答
                UART_SendStr(IRB_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
            //逆时针转
            else if (memcmp(uCmdBuff, IRB_CIRB_SET_MOTOR_ACW, strlen(IRB_CIRB_SET_MOTOR_ACW)) == 0)
            {
                //执行指令
                IRB_SetMotorDirect(IRB_MOTOR_DIRECT_ACW);
                
                //返回应答
                UART_SendStr(IRB_COM_UART_NODE, "OK.");
                
                //清空数据
                memset(uCmdBuff, 0, sizeof(uCmdBuff));
                ulRxIndex = 0;
                return;
            }
        }
        
        ulRxIndex++;
    }
    
    
}