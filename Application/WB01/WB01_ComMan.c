/**
  ******************************************************************************
  * @file    WB01_ComMan.c
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
#include "WB01_ComMan.h"
#include "WB01_HardwareDef.h"
#include "WB01_HwCtrl.h"
#include "WB01_CmdUnpack.h"
#include "WB01_CD4094.h"
      
#include "DataType/DataType.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ExtPeripheral/LogicIC/HC595.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define WB01_CMD_BUFF_LEN                   (64)

static uBit8 uRecvCmdBuff[WB01_CMD_BUFF_LEN] = {0};   //定义指令缓冲区
static uBit8 uSendCmdBuff[WB01_CMD_BUFF_LEN] = {0};   //定义指令缓冲区
static uBit32 m_ulRecvLen = 0;
static uBit32 m_ulSendLen = 0;

typedef struct
{
    uBit8 uCmdType;
    uBit8 uCmd;
    uBit8 uData[];
    
}CMD_RECV_DATA;

typedef struct
{
    uBit8 uCmdType;
    uBit8 uCmd;
    uBit8 uErrCode;
    uBit8 uData[];
    
}CMD_SEND_DATA;

/*****************************************************************************
 * 指令执行
 ****************************************************************************/

//指令类型定义
#define WB01_CMD_TYPE_SET                       (0) //设置指令
#define WB01_CMD_TYPE_GET                       (1) //查询指令
    
//设置指令定义    
#define WB01_SET_CMD_INDOOR                     (1) //进货门控制
#define WB01_SET_CMD_OUTDOOR                    (2) //取货门控制
#define WB01_SET_CMD_OUTGOOS                    (3) //出货控制
#define WB01_SET_CMD_AISLE                      (4) //货道控制
#define WB01_SET_CMD_GRID_LEARN                 (5) //柜号学习(检测当前总柜号)
#define WB01_SET_CMD_OUTGOODS_RESET             (6) //出货复位

//查询指令定义
#define WB01_GET_CMD_INDOOR_STATUS              (1) //进货门状态查询
#define WB01_GET_CMD_OUTDOOR_STATUS             (2) //取货门状态查询
#define WB01_GET_CMD_MAIN_AXIS_MOTOR_STATUS     (3) //出货流程步骤获取
#define WB01_GET_CMD_OUTGOODS_GRID              (4) //当前出货柜号查询
#define WB01_GET_CMD_GRID_LEARN_STATUS          (5) //总柜数自学习状态查询
#define WB01_GET_CMD_TOTAL_GRID                 (6) //总柜数查询
#define WB01_GET_CMD_AISLE_MOTOR_STATUS         (7) //货道电机状态查询
#define WB01_GET_CMD_LIGHT_STATUS               (8) //光栅状态查询
#define WB01_GET_CMD_LIGHT_REACTION_STATUS      (9) //光感状态查询
#define WB01_GET_CMD_IN_IO_STATUS               (10)//输入IO状态查询

//错误码定义
#define WB01_CMD_ERR_SUCCESS                    (0) //成功
#define WB01_CMD_ERR_PARM_LENGTH                (1) //参数长度错误
#define WB01_CMD_ERR_PARM_RANGE                 (2) //参数范围错误(超过约定的范围)
#define WB01_CMD_ERR_DEV_BUSY                   (3) //设备忙
#define WB01_CMD_ERR_INVALID                    (4) //无效指令

/**
  * @brief  指令处理
  * @param  None
  * @retval None
  */
uBit32 WB01_CmdProcesser(uBit8* pRcvBuf, uBit32 ulRcvLen)
{
    CMD_RECV_DATA *pRecvData = (CMD_RECV_DATA *)pRcvBuf;
    CMD_SEND_DATA *pSendData = (CMD_SEND_DATA *)uSendCmdBuff;
    
    uBit32 ulDataLen = ulRcvLen - 2;
    uBit32 ulRet = 0;
    
    //清空发送数据缓冲区
    memset(uSendCmdBuff, 0, sizeof(uSendCmdBuff));
    
    //设置指令参数
    pSendData->uCmdType = pRecvData->uCmdType;
    pSendData->uCmd = pRecvData->uCmd;
    
    switch (pRecvData->uCmdType)
    {
    case WB01_CMD_TYPE_SET:     //设置指令
        
        switch (pRecvData->uCmd)
        {
        case WB01_SET_CMD_INDOOR     : //进货门控制
            
            //参数长度校验
            if (ulDataLen != sizeof(uBit8))
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            WB01_SetIndoorStatus(pRecvData->uData[0]);
            
            //设置回发数据
            pSendData->uData[0] = 0;    //成功
            m_ulSendLen = sizeof(uBit8);
            
            break;
        case WB01_SET_CMD_OUTDOOR    : //取货门控制
            
            //参数长度校验
            if (ulDataLen != sizeof(uBit8))
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            WB01_SetOutdoorStatus(pRecvData->uData[0]);
            
            //设置回发数据
            pSendData->uData[0] = 0;    //成功
            m_ulSendLen = sizeof(uBit8);
            
            break;
        case WB01_SET_CMD_OUTGOOS    : //出货控制
            
            //参数长度校验
            if (ulDataLen != sizeof(uBit8))
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            ulRet = WB01_SetObjGridNumber(pRecvData->uData[0]);
            
            if (ulRet == 2)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_RANGE;
            }
            else if (ulRet == 1)
            {
                pSendData->uErrCode = WB01_CMD_ERR_DEV_BUSY;
            }
            
            //设置回发数据
            pSendData->uData[0] = (ulRet & 0xFF);
            m_ulSendLen = sizeof(uBit8);
            
            break;
        case WB01_SET_CMD_AISLE      : //货道控制
            
            //参数长度校验
            if (ulDataLen != sizeof(uBit8))
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            ulRet = WB01_EnableAisleMotor(pRecvData->uData[0]/10, pRecvData->uData[0]%10);
            
            if (ulRet == 2)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_RANGE;
            }
            else if (ulRet == 1)
            {
                pSendData->uErrCode = WB01_CMD_ERR_DEV_BUSY;
            }
            
            //设置回发数据
            pSendData->uData[0] = (ulRet & 0xFF);
            m_ulSendLen = sizeof(uBit8);
            
            break;
        case WB01_SET_CMD_GRID_LEARN : //柜号学习(检测当前总柜号)
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            ulRet = WB01_StartSelfLearn();
            
            if (ulRet == 1)
            {
                pSendData->uErrCode = WB01_CMD_ERR_DEV_BUSY;
            }
                
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = (ulRet & 0xFF);
            
            break;
        case WB01_SET_CMD_OUTGOODS_RESET:
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //执行指令
            ulRet = WB01_StartOutGoodsResetTask();
            
            if (ulRet == 1)
            {
                pSendData->uErrCode = WB01_CMD_ERR_DEV_BUSY;
            }
            
            //设置回发数据
            pSendData->uData[0] = (ulRet & 0xFF);
            m_ulSendLen = sizeof(uBit8);
            
            break;
            
        default:
            pSendData->uErrCode = WB01_CMD_ERR_INVALID;
            
            //设置回发数据
            m_ulSendLen = 0;
            
            break;
        }
        
        break;
        
        
    case WB01_CMD_TYPE_GET:     //查询指令
        
        switch (pRecvData->uCmd)
        {
        case WB01_GET_CMD_INDOOR_STATUS           : //进货门状态查询   
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetIndoorStatus();
            
            break;
        case WB01_GET_CMD_OUTDOOR_STATUS          : //取货门状态查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetOutdoorStatus();
            
            break;
        case WB01_GET_CMD_MAIN_AXIS_MOTOR_STATUS  : //出货流程步骤获取
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetOutGoodsStep();
            
            break;
        case WB01_GET_CMD_OUTGOODS_GRID           : //当前出货柜号查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetObjGridNumber();
            
            break;
        case WB01_GET_CMD_GRID_LEARN_STATUS       : //总柜数自学习状态查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetSelfLearnStep();
            
            break;
        case WB01_GET_CMD_TOTAL_GRID              : //总柜数查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetMaxGridCount();
            
            break;
        case WB01_GET_CMD_AISLE_MOTOR_STATUS      : //货道电机状态查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = WB01_GetAisleMotorRunningState();
            
            break;
        case WB01_GET_CMD_LIGHT_STATUS            : //光栅状态查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = GPIO_MAN_GetInputPinState(INPUT_IO_GOODS_DECTECT);
            
            break;
            
        case WB01_GET_CMD_LIGHT_REACTION_STATUS:    //光感检测查询
            
            //参数长度校验
            if (ulDataLen != 0)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = GPIO_MAN_GetInputPinState(INPUT_IO_LIGHT_REACTION);
            
            break;
            
        case WB01_GET_CMD_IN_IO_STATUS:             //输入IO状态查询
            
            //参数长度校验
            if (ulDataLen != 1)
            {
                pSendData->uErrCode = WB01_CMD_ERR_PARM_LENGTH;
            }
            
            //设置回发数据
            m_ulSendLen = sizeof(uBit8);
            pSendData->uData[0] = GPIO_GetInputState(pRecvData->uData[0]);
            
            break;
        default:
            pSendData->uErrCode = WB01_CMD_ERR_INVALID;
            
            //设置回发数据
            m_ulSendLen = 0;
            
            break;
        }
        
        break;
        
    default:
        pSendData->uErrCode = WB01_CMD_ERR_INVALID;
        
        //设置回发数据
        m_ulSendLen = 0;
        
        break;
    }
    
    
    //数据回发
    WB01_Pack((uBit8 *)pSendData, sizeof(pSendData->uCmdType) + sizeof(pSendData->uCmd) + sizeof(pSendData->uErrCode) + m_ulSendLen);
    
    return ulRet;
}


/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理
  * @param  None
  * @retval None
  */
void WB01_ComHandler(void)
{
    if (WB01_UnPack(uRecvCmdBuff, &m_ulRecvLen) == 0)
    {
        WB01_CmdProcesser(uRecvCmdBuff, m_ulRecvLen);
        
    }
    
}
