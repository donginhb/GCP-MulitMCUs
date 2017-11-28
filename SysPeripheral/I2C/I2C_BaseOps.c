/**
  ******************************************************************************
  * @file    I2C_BaseOps.c
  * @author  Duhanfeng
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:位操作实现
  ******************************************************************************
  * @attention
  * 
  * 对象: I2C_BIT_OPS_TYPE
  * 
  * 本文件主要是通过BIT_OPS来实现基本IIC时序: 
  * pf_I2C_Start   
  * pf_I2C_Stop    
  * pf_I2C_SendAckOrNack 
  * pf_I2C_WaitAck  
  * pf_I2C_SendByte 
  * pf_I2C_RecvByte 
  * 
  * 上面的时序接口函数可封装成结构体I2C_BUS_BASE_OPS_TYPE,并交由I2C总线设备结构体
  * 管理及调用.
  * 
  * 底层操作函数,以实现最基本的I2C时序逻辑,直接对IO进行控制
  * 其操作的对象是 I2C_BIT_OPS_TYPE 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "I2C_Interface.h"
#include "I2C_DataStructDef.h"
#include "SysPeripheral/SysTimer/SysTimer.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define SET_SDA(Ops, Value)     Ops->pf_I2C_SetSDA(Ops->Data, Value)
#define SET_SCL(Ops, Value)     Ops->pf_I2C_SetSCL(Ops->Data, Value)
#define GET_SDA(Ops)            Ops->pf_I2C_GetSDA(Ops->Data)
#define GET_SCL(Ops)            Ops->pf_I2C_GetSCL(Ops->Data)
#define I2C_BitDelay(Ops)       Ops->pf_I2C_BitDelay(Ops->Data);

#define SDA_L(Ops)              SET_SDA(Ops, 0)
#define SDA_H(Ops)              SET_SDA(Ops, 1)
#define SCL_L(Ops)              SET_SCL(Ops, 0)
#define SCL_H(Ops)              SET_SCL(Ops, 1)


/**
  * @brief  I2C开始标志(在SCL高电平时拉低SDA)
  * @param  None
  * @retval None
  */
static void I2C_Base_Start(I2C_BIT_OPS_TYPE *Ops)
{
    SDA_H(Ops);
    SCL_H(Ops);
    I2C_BitDelay(Ops);
    SDA_L(Ops);
    I2C_BitDelay(Ops);
    SCL_L(Ops);
    
}


/**
  * @brief  I2C结束标志(在SCL高电平时拉高SDA)
  * @param  None
  * @retval None
  */
static void I2C_Base_Stop(I2C_BIT_OPS_TYPE *Ops)
{
    SCL_L(Ops);
    SDA_L(Ops);
    
    SCL_H(Ops);
    I2C_BitDelay(Ops);
    SDA_H(Ops);
    I2C_BitDelay(Ops);
    
}


/**
  * @brief  I2C等待应答信号(在SCL为高时等待应答,SDA变低为应答)
  * @param  None
  * @retval isAck 是否有应答信号,有则返回1,无则返回0
  */
static uBit8 I2C_Base_WaitAck(I2C_BIT_OPS_TYPE *Ops)
{
    uBit8 isAck = 1;
    SYS_TIME_DATA I2C_Timer = {0};
    
    SDA_H(Ops); //释放总线
    SCL_L(Ops);
    I2C_BitDelay(Ops);
    SCL_H(Ops);
    
    SysTime_Start(&I2C_Timer, Ops->nTimeOut);  //设置超时时间
    while (GET_SDA(Ops)) //等待SDA变低
    {
        if (SysTime_CheckExpiredState(&I2C_Timer))
        {
            isAck = 0;
            break;
        }
        I2C_BitDelay(Ops);
    }
    
    I2C_BitDelay(Ops);
    SCL_L(Ops);
    
    return isAck;
}


/**
  * @brief  I2C给应答/不应答信号(在SCL为低时SDA给个低电平即为应答信号)
  * @param  None
  * @retval None
  */
static void I2C_Base_SendAckOrNack(I2C_BIT_OPS_TYPE *Ops, uBit8 ack)
{
    SCL_L(Ops);
    
    if (ack) 
    {
        SDA_L(Ops);  //应答信号
    }
    else 
    {
        SDA_H(Ops);  //应答信号
    }
    
    I2C_BitDelay(Ops);
    
    SCL_H(Ops);
    I2C_BitDelay(Ops);
    
    SCL_L(Ops);
    
}


/**
  * @brief  I2C字节发送函数(在SCL低电平时拉高/拉低SDA)
  * @param  cSendData 要发送的字节
  * @retval None
  */
static uBit8 I2C_Base_SendByte(I2C_BIT_OPS_TYPE *Ops, uBit8 cByte)
{
    uBit8 i = 0;
    
    for (i = 0; i < 8; i++)
    {
        SCL_L(Ops);
        
        if (cByte & (0X1<<(7-i)))  //发送数据
        {
            SDA_H(Ops);
        }
        else
        {
            SDA_L(Ops);
        }
        
        I2C_BitDelay(Ops);
        
        SCL_H(Ops);
        I2C_BitDelay(Ops);
        
    }
    SCL_L(Ops);
    
    return I2C_Base_WaitAck(Ops);
}


/**
  * @brief  I2C字节接收函数(在SCL高电平时读SDA电平状态)
  * @param  None
  * @retval cRecvData 接收到的字节
  */
static uBit8 I2C_Base_RecvByte(I2C_BIT_OPS_TYPE *Ops)
{
    uBit8 i = 0;
    uBit8 cRecvData = 0;
    
    SDA_H(Ops);   //凡是读取操作前,都要释放总线,否则从机无法操作总线
    
    for (i = 0; i < 8; i++)
    {
        SCL_L(Ops);
        I2C_BitDelay(Ops);
        
        SCL_H(Ops);
        
        if (GET_SDA(Ops))
        {
            cRecvData |=  (0X1<<(7-i));
        }
        
        I2C_BitDelay(Ops);
    }
    SCL_L(Ops);
    
    return cRecvData;
}


/*****************************************************************************
 * I2C 基本操作输出
 ****************************************************************************/

/**
  * @brief  I2C 基本操作接口输出
  * @param  pBaseOpsInterface 操作接口指针
  * @retval None
  */
void I2C_BaseOpsInterfaceExport(I2C_BUS_BASE_OPS_TYPE *pBaseOpsInterface)
{
    pBaseOpsInterface->pf_I2C_Start         = I2C_Base_Start;
    pBaseOpsInterface->pf_I2C_Stop          = I2C_Base_Stop;
    pBaseOpsInterface->pf_I2C_WaitAck       = I2C_Base_WaitAck;
    pBaseOpsInterface->pf_I2C_SendAckOrNack = I2C_Base_SendAckOrNack;
    pBaseOpsInterface->pf_I2C_SendByte      = I2C_Base_SendByte;
    pBaseOpsInterface->pf_I2C_RecvByte      = I2C_Base_RecvByte;
    
}

