/**
  ******************************************************************************
  * @file    ModBus_Spm91.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.09.13
  * @brief   SPM91 电表驱动(基于modbus总线)
  ******************************************************************************
  * @attention
  * 
  * 串口时序: 8位字长,无校验,1位停止位
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Ammeter_Spm91.h"

#include "DataType/DataType.h"
#include "SysPeripheral/UART/ModBus.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <string.h>
#include <stdio.h>


/*****************************************************************************
 * 电表相关寄存器定义
 ****************************************************************************/
#define SPM91_REG_TOTAL_ACTIVE_ENERGY_L         (40001 - 40001)  
#define SPM91_REG_TOTAL_ACTIVE_ENERGY_H         (40002 - 40001)  //有功电能总和
#define SPM91_REG_VOLTAGE                       (40003 - 40001)  //电压
#define SPM91_REG_CURRENT_L                     (40004 - 40001)  
#define SPM91_REG_CURRENT_H                     (40005 - 40001)  //电流
#define SPM91_REG_ACTIVE_POWER_L                (40006 - 40001)  
#define SPM91_REG_ACTIVE_POWER_H                (40007 - 40001)  //有功功率
#define SPM91_REG_APPARENT_POWER_L              (40008 - 40001)  
#define SPM91_REG_APPARENT_POWER_H              (40009 - 40001)  //视在功率
#define SPM91_REG_REACTIVE_POWER_L              (40010 - 40001)  
#define SPM91_REG_REACTIVE_POWER_H              (40011 - 40001)  //无功功率
#define SPM91_REG_FREQUENCY                     (40012 - 40001)  //频率
#define SPM91_REG_POWER_FACTOR                  (40013 - 40001)  //功率因子
#define SPM91_REG_INPUT_ACTIVE_ENERGY_L         (40014 - 40001)  
#define SPM91_REG_INPUT_ACTIVE_ENERGY_H         (40015 - 40001)  //输入有功电量
#define SPM91_REG_OUTPUT_ACTIVE_ENERGY_L        (40016 - 40001)  
#define SPM91_REG_OUTPUT_ACTIVE_ENERGY_H        (40017 - 40001)  //输出有功电量
#define SPM91_REG_INPUT_REACTIVE_ENERGY_L       (40020 - 40001)  
#define SPM91_REG_INPUT_REACTIVE_ENERGY_H       (40021 - 40001)  //输入无功电量
#define SPM91_REG_OUTPUT_REACTIVE_ENERGY_L      (40022 - 40001)  
#define SPM91_REG_OUTPUT_REACTIVE_ENERGY_H      (40023 - 40001)  //输出无功电量
#define SPM91_REG_TOTAL_REACTIVE_ENERGY_L       (40024 - 40001)  
#define SPM91_REG_TOTAL_REACTIVE_ENERGY_H       (40025 - 40001)  //总无功电量


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static MODBUS_DATA m_ModBusData = {0};  //用于实现ModBus的数据
static uBit32 m_ulDirPortNO = 0;


/**
  * @brief  RS485方向控制
  * @param  bState IO状态
  * @retval None
  */
static void SPM91_RS485_SetDirIOState(bool bState)
{
    GPIO_MAN_SetOutputPinState(m_ulDirPortNO, bState);
    
}


/*****************************************************************************
 * SPM91相关接口函数
 ****************************************************************************/

/**
  * @brief  SPM91电表初始化函数
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @param  ulDirPortNO RS485方向控制端口在输出IO表中的编号
  * @retval 0-成功  非0-失败
  */
uBit32 SPM91_Init(uBit8 uUartNode, uBit32 ulBaudRate, uBit32 ulDirPortNO)
{
    m_ulDirPortNO = ulDirPortNO;
    
    //初始化ModBus
    return ModBus_Init(uUartNode, ulBaudRate, SPM91_RS485_SetDirIOState);
}


/**
  * @brief  SPM91电表参数获取函数
  * @param  SPM91_DATA 接收到电表参数(出参)
  * @retval true - 接收到有效数据 false - 没接收到数据
  * @note   获取到的电表参数数据,都是乘以因子后放大100倍的整型值
  */
bool SPM91_RecvHandler(SPM91_DATA *pSpm91Data)
{
    if (ModBus_RecvHandler(&m_ModBusData) == MODBUS_STATUS_OPERA_COMPLETE)
    {
        pSpm91Data->uDevAddr = m_ModBusData.uDevAddr;
        pSpm91Data->AmmeterParam = (SPM91_AMMETER_PARAM)m_ModBusData.nRegAddr;
        
        switch (pSpm91Data->AmmeterParam)
        {
        case SPM91_PARAM_TOTAL_ACTIVE_ENERGY    : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //有功电能总和
        case SPM91_PARAM_VOLTAGE                : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.01 );  break; //电压
        case SPM91_PARAM_CURRENT                : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.001);  break; //电流
        case SPM91_PARAM_ACTIVE_POWER           : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //有功功率
        case SPM91_PARAM_APPARENT_POWER         : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //视在功率
        case SPM91_PARAM_REACTIVE_POWER         : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //无功功率
        case SPM91_PARAM_FREQUENCY              : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.01 );  break; //频率
        case SPM91_PARAM_POWER_FACTOR           : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.001);  break; //功率因子
        case SPM91_PARAM_INPUT_ACTIVE_ENERGY    : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //输入有功电量
        case SPM91_PARAM_OUTPUT_ACTIVE_ENERGY   : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //输出有功电量
        case SPM91_PARAM_INPUT_REACTIVE_ENERGY  : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //输入无功电量
        case SPM91_PARAM_OUTPUT_REACTIVE_ENERGY : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //输出无功电量
        case SPM91_PARAM_TOTAL_REACTIVE_ENERGY  : pSpm91Data->ulParamValue = (uBit32)((m_ModBusData.nRegData[0] | m_ModBusData.nRegData[1] << 16) * 100 * 0.1  );  break; //总无功电量
        default: break;
        }
        
        m_ModBusData.ulWordStatus = MODBUS_STATUS_IDLE;
        
        return true;
    }
    
  return false;
}


/**
  * @brief  SPM91电表参数请求函数
  * @param  uDevAddr 设备号
  * @param  SPM91_AMMETER_PARAM 要请求的电表参数
  * @retval None
  */
void SPM91_RequestAmmeterParam(uBit8 uDevAddr, SPM91_AMMETER_PARAM AmmeterParam)
{
    m_ModBusData.uDevAddr = uDevAddr;           //设备地址
    m_ModBusData.nRegAddr = AmmeterParam;       //寄存器地址(电表参数)
    m_ModBusData.uCmdType = MODBUS_CMD_READ_REG;//获取指令
    
    //计算要读取的寄存器数
    switch (AmmeterParam)
    {
    case SPM91_PARAM_VOLTAGE:
    case SPM91_PARAM_FREQUENCY:
    case SPM91_PARAM_POWER_FACTOR:
        {
            m_ModBusData.uRegCount = 1;
            break;
        }
    default:
        {
            m_ModBusData.uRegCount = 2;
            break;
        }
    }
    
    ModBus_Send(&m_ModBusData);
}


