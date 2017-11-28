#ifndef  __EEPROM_H
#define  __EEPROM_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/*****************************************************************************
 * EEPROM相关控制接口
 ****************************************************************************/

/**
  * @brief  EEPROM 读写有效校验
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 EEPROM_CheckRW(void);


/**
  * @brief  EEPROM初始化
  * @param  ulDataPinNO 数据控制脚编号
  * @param  ulClockPinNO 时钟控制脚编号
  * @retval 0-成功 非0-失败
  */
uBit32 EEPROM_Init(uBit32 ulDataPinNO, uBit32 ulClockPinNO);


/**
  * @brief  EEPROM数据读取
  * @param  nAddr     存储地址
  * @param  pReadBuff 接收缓冲区
  * @param  ulSize    要读取的长度
  * @retval None
  */
void EEPROM_ReadBuff(uBit16 nAddr, uBit8 *pReadBuff, uBit32 ulSize);


/**
  * @brief  EEPROM数据写入
  * @param  nAddr     存储地址
  * @param  pReadBuff 写入缓冲区
  * @param  ulSize    要写入的长度
  * @retval None
  */
void EEPROM_WriteBuff(uBit16 nAddr, const uBit8 *pWriteBuff, uBit32 ulSize);


#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H */
