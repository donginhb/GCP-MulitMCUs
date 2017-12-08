/**
  ******************************************************************************
  * @file    ModBus.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.09.12
  * @brief   Modbus 驱动层
  ******************************************************************************
  * @attention
  * 
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "ModBus.h"
#include "UART.h"

#include "DataType/DataType.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>


extern unsigned short int CRC16(unsigned char *Pushdata,unsigned long int length);

/*****************************************************************************
 * RS485相关私有函数
 ****************************************************************************/
#define RS485_DIR_TX                        (true)          //使能发送功能的DIR电平
#define RS485_DIR_RX                        (false)         //使能接收功能的DIR电平

static void (*pf_ModBus_RS485_SetDir)(bool bState) = NULL;  //485方向控制脚


/**
  * @brief  RS485数据传输方向设置
  * @param  bState 传输方向,true为发送,false为接收
  * @retval None
  */
static void RS485_SetDir(bool bState)
{
    if (pf_ModBus_RS485_SetDir != NULL)
    {
        pf_ModBus_RS485_SetDir(bState);
    }
    
}


/*****************************************************************************
 * ModBus相关私有函数
 ****************************************************************************/

static uBit8 m_uModBusRecvBuff[64] = {0}; //ModBus接收数据缓冲区
static uBit8 m_uUartNode = 0;

#define MODBUS_FRAME_TIME_OUT        (30)   //30M帧超时时间
#define MODBUS_DATA_TIME_OUT         (5)    //5MS数据超时时间

/**
  * @brief  ModBus接收数据校验函数
  * @param  pModBusData 结构体指针
  * @retval None
  */
static Bit8 ModBus_CheckRecvData(MODBUS_DATA *pModBusData)
{
    Bit8 cRetCode = MODBUS_ERR_CMU;   //执行结果
    uBit16 nCrcValue = 0;             //CRC校验值
    uBit32 ulDataCount = 0;           //接收到的数据中,数据区字节数
    
    //校验数据
    do 
    {
        //判断是否接收到数据
        if (pModBusData->ulRecvCount == 0) 
        {
            cRetCode = MODBUS_ERR_CMU;  //无效数据
            break;
        }
        //判断接收到的数据是否有效
        if (m_uModBusRecvBuff[0] != pModBusData->uDevAddr) 
        {
            cRetCode = MODBUS_ERR_CMU;  //无效数据
            break;
        }
        //判断是否错误帧
        if (m_uModBusRecvBuff[1] == MODBUS_CMD_ERR_CODE) 
        {
            cRetCode = m_uModBusRecvBuff[2];    //错误码
            break;
        }
        //判断数据区长度是否匹配
        ulDataCount = m_uModBusRecvBuff[2];  //获取数据区长度
        if (ulDataCount != pModBusData->uRegCount*2)
        {
            cRetCode = MODBUS_ERR_CMU;  //无效数据
            break;
        }
        //判断数据帧是否完整
        if (pModBusData->ulRecvCount != (5+ulDataCount))
        {
            cRetCode = MODBUS_ERR_CMU;  //无效数据
            break;
        }
        //判断CRC是否正确
        nCrcValue = (m_uModBusRecvBuff[ulDataCount+3] << 8) | m_uModBusRecvBuff[ulDataCount+4];
        if (nCrcValue != CRC16(m_uModBusRecvBuff, ulDataCount+3))
        {
            cRetCode = MODBUS_ERR_CMU;  //无效数据
            break;
        }
        
        //如果程序能运行到这里,可以确定当前数据为正确的数据,提取
        uBit8 *pDataPoint = (uBit8 *)pModBusData->nRegData;
        for (int i = 0; i < ulDataCount; i++)
        {
            pDataPoint[ulDataCount - 1 - i] = m_uModBusRecvBuff[3+i];
        }
        cRetCode = MODBUS_ERR_SUCCESS;  //执行成功
        
    }while (0);
    
    return cRetCode;
}


/*****************************************************************************
 * ModBus非阻塞性控制接口
 ****************************************************************************/

/**
  * @brief  ModBus初始化函数
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @param  pf_RS485_SetDir  485方向控制接口,true为发送,false为接收
  * @retval 0-成功  非0-失败
  */
uBit32 ModBus_Init(uBit8 uUartNode, uBit32 ulBaudRate, void (*pf_RS485_SetDir)(bool bState))
{
    m_uUartNode = uUartNode;
    pf_ModBus_RS485_SetDir = pf_RS485_SetDir;
    
    uBit32 ulRet = UART_Init(uUartNode, ulBaudRate);
    
    return ulRet;
}


/**
  * @brief  ModBus数据发送函数
  * @param  pModBusData 结构体指针
  * @retval None
  */
void ModBus_Send(MODBUS_DATA *pModBusData)
{
    uBit8 uSendBuff[64] = {0};        //发送缓冲区
    uBit16 nCrcValue = 0;             //CRC校验值
    
    //初始化相关参数
    pModBusData->ulWordStatus = MODBUS_STATUS_IDLE;   //设置当前工作状态:空闲
    pModBusData->ulRecvCount = 0;
    pModBusData->ulTimeTick = 0;
    
    memset(pModBusData->nRegData, 0, sizeof(pModBusData->nRegData));
    memset(m_uModBusRecvBuff, 0, sizeof(m_uModBusRecvBuff));
    
    //组装数据
    uSendBuff[0] = pModBusData->uDevAddr;           //设备地址
    uSendBuff[1] = pModBusData->uCmdType;           //读取指令
    uSendBuff[2] = pModBusData->nRegAddr >> 8;      //寄存器地址高8位
    uSendBuff[3] = pModBusData->nRegAddr & 0xFF;    //寄存器地址低8位
    uSendBuff[4] = pModBusData->uRegCount >> 8;     //读取寄存器数高8位
    uSendBuff[5] = pModBusData->uRegCount & 0xFF;   //读取寄存器数低8位

    nCrcValue = CRC16(uSendBuff, 6);                //获取CRC
    uSendBuff[6] = nCrcValue >> 8;                  //CRC高8位
    uSendBuff[7] = nCrcValue & 0xFF;                //CRC低8位
    
    //发送数据
    RS485_SetDir(RS485_DIR_TX);                     //配置RS485方向:发送
    UART_SendBuff(m_uUartNode, uSendBuff, 8);       //非阻塞性发送
    
    pModBusData->ulWordStatus = MODBUS_STATUS_SEND; //设置当前工作状态:正在发送
    

}


/**
  * @brief  ModBus数据接收处理函数
  * @param  pModBusData 结构体指针
  * @retval 当前ModBus的工作状态
  */
uBit32 ModBus_RecvHandler(MODBUS_DATA *pModBusData)
{
    //假如当前处于发送状态
    if (pModBusData->ulWordStatus == MODBUS_STATUS_SEND)
    {
        if (UART_GetTransStatus(m_uUartNode))   //假如数据发送完成(非阻塞)
        {
            RS485_SetDir(RS485_DIR_RX);                         //设置数据接收方向:接收
            pModBusData->ulTimeTick = SysTime_GetTickCount();   //获取当前系统滴答值
            pModBusData->ulWordStatus = MODBUS_STATUS_RECV;     //设置当前工作状态:正在接收
        }
    }
    
    //假如当前处于接收状态
    if (pModBusData->ulWordStatus == MODBUS_STATUS_RECV)
    {
        //接收处理
        if (UART_RecvBuff(m_uUartNode, &m_uModBusRecvBuff[pModBusData->ulRecvCount], 1)) //数据接收(非阻塞)
        {
            pModBusData->ulRecvCount++;
            pModBusData->ulTimeTick = SysTime_GetTickCount();      //刷新滴答值
        }
        
        //超时处理
        if ((SysTime_GetTickCount() - pModBusData->ulTimeTick) > 
            (pModBusData->ulRecvCount ? MODBUS_DATA_TIME_OUT : MODBUS_FRAME_TIME_OUT))
        {
            //数据校验
            Bit8 cRetCode = ModBus_CheckRecvData(pModBusData);
            
            //数据解析
            if (cRetCode == MODBUS_ERR_SUCCESS)
            {
                pModBusData->ulWordStatus = MODBUS_STATUS_OPERA_COMPLETE; //设置当前工作状态: 操作完成
                
                //提取数据
                for (int i = 0; i < pModBusData->uRegCount; i++)
                {
                    pModBusData->nRegData[i] = (m_uModBusRecvBuff[i*2 + 3] << 8) | m_uModBusRecvBuff[i*2 + 4];
                }
            }
            else
            {
                pModBusData->ulWordStatus = MODBUS_STATUS_OPERA_FAIL;     //设置当前工作状态: 操作失败
            }
        }
    }
    
    return pModBusData->ulWordStatus;
}


