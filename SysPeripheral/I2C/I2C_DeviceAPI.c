/**
  ******************************************************************************
  * @file    I2C_DeviceAPI.c
  * @author  Duhanfeng
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:应用层实现文件
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "I2C_Interface.h"
#include "I2C_DataStructDef.h"

#define I2C_ASSERT(x)    {if(!x){while(1);}}


/**
  * @brief  I2C 基本操作接口输出
  * @param  pBaseOpsInterface 操作接口指针
  * @retval None
  */
extern void I2C_BaseOpsInterfaceExport(I2C_BUS_BASE_OPS_TYPE *pBaseOpsInterface);


/**
  * @brief  I2C设备操作接口输出
  * @param  pDeviceOpsInterface 操作接口指针
  * @retval None
  */
extern void I2C_DeviceOpsInterfaceExport(I2C_BUS_DEVICE_OPS_TYPE *pDeviceOpsInterface);



/**
  * @brief  I2C总线设备读取
  * @param  pDevOps 设备操作接口
  * @param  Offset  数据偏移
  * @param  pBuff   数据缓冲区
  * @param  ulCount 数据字节数
  * @retval 读取的字节数
  */
uBit32 I2C_BusDeviceRead(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Offset, void *pBuff, uBit32 ulCount)
{
    I2C_BUS_BASE_OPS_TYPE *pBaseOps = &pDevOps->BaseOps;
    I2C_CTRL_PARM *pCtrlParm = &pDevOps->CtrlParm;
    
    uBit16 nRegAddr = 0;
    
    I2C_ASSERT(pDevOps != NULL);
    
    nRegAddr = Offset & 0xFFFF;
    
    if (pDevOps->pf_I2C_MST_RecvBuff(pBaseOps, pCtrlParm, nRegAddr, pBuff, ulCount) != I2C_ERR_OK)
    {
        return 0;
    }
    
    return ulCount;
}


/**
  * @brief  I2C总线设备写入
  * @param  pDevOps 设备操作接口
  * @param  Offset  数据偏移
  * @param  pBuff   数据缓冲区
  * @param  ulCount 数据字节数
  * @retval 写入的字节数
  */
uBit32 I2C_BusDeviceWrite(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Offset, const void *pBuff, uBit32 ulCount)
{
    I2C_BUS_BASE_OPS_TYPE *pBaseOps = &pDevOps->BaseOps;
    I2C_CTRL_PARM *pCtrlParm = &pDevOps->CtrlParm;
    
    uBit16 nRegAddr = 0;
    
    I2C_ASSERT(pDevOps != NULL);
    
    nRegAddr = Offset & 0xFFFF;
    
    if (pDevOps->pf_I2C_MST_SendBuff(pBaseOps, pCtrlParm, nRegAddr, pBuff, ulCount) != I2C_ERR_OK)
    {
        return 0;
    }
    
    return ulCount;
}


/**
  * @brief  I2C总线设备配置
  * @param  pDevOps 设备操作接口
  * @param  Cmd     控制指令
  * @param  args    指令内容
  * @retval 错误码
  */
uBit32 I2C_BusDeviceControl(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Cmd, void *args)
{
    I2C_CTRL_PARM *pCtrlParm = &pDevOps->CtrlParm;
    
    switch (Cmd)
    {
        //清除所有的标志位
    case I2C_DEV_CTRL_CLEAR_ALL_FLAGS:
        {
            pCtrlParm->nFlags = 0;
            break;
        }
        
        //8位设备地址字长(清除16位的标志)
    case I2C_DEV_CTRL_DEV_ADDR_8BIT:
        {
            pCtrlParm->nFlags &= ~I2C_FLAG_DEV_ADDR_16BIT;
            pCtrlParm->nFlags &= ~I2C_FLAG_NO_DEV_ADDR;
            break;
        }
        
        //16位设备地址字长
    case I2C_DEV_CTRL_DEV_ADDR_16BIT:
        {
            pCtrlParm->nFlags |=  I2C_FLAG_DEV_ADDR_16BIT;
            pCtrlParm->nFlags &= ~I2C_FLAG_NO_DEV_ADDR;
            break;
        }
        
        //无设备地址
    case I2C_DEV_CTRL_NO_DEV_ADDR:
        {
            pCtrlParm->nFlags |=  I2C_FLAG_NO_DEV_ADDR;
            break;
        }
        
        //8位寄存器地址字长(清除16位的标志)
    case I2C_DEV_CTRL_REG_ADDR_8BIT:
        {
            pCtrlParm->nFlags &= ~I2C_FLAG_REG_ADDR_16BIT;
            pCtrlParm->nFlags &= ~I2C_FLAG_NO_REG_ADDR;
            break;
        }
        
        //16位寄存器地址字长
    case I2C_DEV_CTRL_REG_ADDR_16BIT:
        {
            pCtrlParm->nFlags |=  I2C_FLAG_REG_ADDR_16BIT;
            pCtrlParm->nFlags &= ~I2C_FLAG_NO_REG_ADDR;
            break;
        }
        
        //无寄存器地址
    case I2C_DEV_CTRL_NO_REG_ADDR:
        {
            pCtrlParm->nFlags |=  I2C_FLAG_NO_REG_ADDR;
            break;
        }
        
        //设置超时时间
    case I2C_DEV_CTRL_SET_TIMEOUT:
        {
            pCtrlParm->nTimeOut = *(uBit32 *)args;
            pDevOps->BaseOps.BitOps.nTimeOut = pCtrlParm->nTimeOut;
            break;
        }
        
        //设置重发次数
    case I2C_DEV_CTRL_SET_RETRIES:
        {
            pCtrlParm->nRetries = *(uBit32 *)args;
            break;
        }
        
        //设置设备地址
    case I2C_DEV_CTRL_SET_DEV_ADDR:
        {
            pCtrlParm->nDevAddr = *(uBit16 *)args;
            break;
        }
        
        //设置BIT_OPS的指针
    case I2C_DEV_CTRL_SET_BIT_OPS:
        {
            pDevOps->BaseOps.BitOps = *(I2C_BIT_OPS_TYPE *)args;
            
            break;
        }
        
    default: break;
    }
    
    return I2C_ERR_OK;
}


/**
  * @brief  I2C总线设备初始化
  * @param  pDevOps 设备操作接口
  * @retval 错误码
  */
uBit32 I2C_BusDevieInit(I2C_BUS_DEVICE_OPS_TYPE *pDevOps)
{
    I2C_DeviceOpsInterfaceExport(pDevOps);
    I2C_BaseOpsInterfaceExport(&pDevOps->BaseOps);
    
    return I2C_ERR_OK;
}

