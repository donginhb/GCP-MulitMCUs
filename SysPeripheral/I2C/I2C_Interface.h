#ifndef  __I2C_INTERFACE_H
#define  __I2C_INTERFACE_H

#include "DataType/DataType.h"
#include "I2C_DataStructDef.h"


#ifdef __cplusplus
extern "C"
{
#endif



/**
  * @brief  I2C总线设备读取
  * @param  pDevOps 设备操作接口
  * @param  Offset  数据偏移
  * @param  pBuff   数据缓冲区
  * @param  ulCount 数据字节数
  * @retval 读取的字节数
  */
uBit32 I2C_BusDeviceRead(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Offset, void *pBuff, uBit32 ulCount);


/**
  * @brief  I2C总线设备写入
  * @param  pDevOps 设备操作接口
  * @param  Offset  数据偏移
  * @param  pBuff   数据缓冲区
  * @param  ulCount 数据字节数
  * @retval 写入的字节数
  */
uBit32 I2C_BusDeviceWrite(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Offset, const void *pBuff, uBit32 ulCount);


/**
  * @brief  I2C总线设备配置
  * @param  pDevOps 设备操作接口
  * @param  Cmd     控制指令
  * @param  args    指令内容
  * @retval 错误码
  */
uBit32 I2C_BusDeviceControl(I2C_BUS_DEVICE_OPS_TYPE *pDevOps, uBit32 Cmd, void *args);


/**
  * @brief  I2C总线设备初始化
  * @param  pDevOps 设备操作接口
  * @retval 错误码
  */
uBit32 I2C_BusDevieInit(I2C_BUS_DEVICE_OPS_TYPE *pDevOps);




#ifdef __cplusplus
}
#endif

#endif /* __I2C_INTERFACE_H */
