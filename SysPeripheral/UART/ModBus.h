#ifndef  __MODBUS_H
#define  __MODBUS_H

#include "DataType/DataType.h"

#pragma pack(push)
#pragma pack(8)    


/*****************************************************************************
 * ModBus相关参数定义
 ****************************************************************************/

//ModBus指令类型
#define MODBUS_CMD_READ_REG                 (0x03)      //读寄存器指令
#define MODBUS_CMD_WRITE_MULTI_REG          (0x10)      //写寄存器指令
#define MODBUS_CMD_ERR_CODE                 (0x83)      //错误码
    
//ModBus数据校验错误码
#define MODBUS_ERR_CMU                      (-1)        //通信失败(接收不到数据/数据异常等)
#define MODBUS_ERR_SUCCESS                  (0)         //执行成功
#define MODBUS_ERR_CMD_CODE                 (1)         //非法功能码(错误的操作码)
#define MODBUS_ERR_REG_ADDR                 (2)         //非法寄存器地址(寄存器数据地址无效)
#define MODBUS_ERR_REG_COUNT                (3)         //非法寄存器数(请求的寄存器个数超长)

//ModBus状态定义
#define MODBUS_STATUS_IDLE                  (0)         //空闲
#define MODBUS_STATUS_SEND                  (1)         //正在发送
#define MODBUS_STATUS_RECV                  (2)         //正在接收
#define MODBUS_STATUS_OPERA_COMPLETE        (3)         //操作完成
#define MODBUS_STATUS_OPERA_FAIL            (4)         //操作失败


//ModBus数据结构定义
typedef struct 
{
    //发送帧基本信息
    uBit8  uDevAddr;      //设备地址
    uBit8  uCmdType;      //控制类型
    uBit8  uRegCount;     //寄存器数量
    uBit16 nRegAddr;      //寄存器地址
    
    //接收处理信息
    uBit16 nRegData[4];   //寄存器数据
    uBit32 ulRecvCount;   //接收到数据计数
    
    //其他
    uBit32 ulTimeTick;    //滴答值
    uBit32 ulWordStatus;  //工作状态
    
}MODBUS_DATA;


#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  ModBus初始化函数
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @param  pf_RS485_SetDir  485方向控制接口,true为发送,false为接收
  * @retval 0-成功  非0-失败
  */
uBit32 ModBus_Init(uBit8 uUartNode, uBit32 ulBaudRate, void (*pf_RS485_SetDir)(bool bState));


/**
  * @brief  ModBus数据发送函数
  * @param  pModBusData 结构体指针
  * @retval None
  */
void ModBus_Send(MODBUS_DATA *pModBusData);


/**
  * @brief  ModBus数据接收处理函数
  * @param  pModBusData 结构体指针
  * @retval 当前ModBus的工作状态
  */
uBit32 ModBus_RecvHandler(MODBUS_DATA *pModBusData);


#ifdef __cplusplus
}
#endif

#pragma pack(pop)


#endif /* __MODBUS_H */
