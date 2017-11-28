#ifndef  __AT24CXX_H
#define  __AT24CXX_H

#include "DataType/DataType.h"

typedef enum
{
    AT24C01_PAGE_SIZE =  8,
    AT24C02_PAGE_SIZE =  8,
    AT24C04_PAGE_SIZE = 16,
    AT24C08_PAGE_SIZE = 16,
    AT24C16_PAGE_SIZE = 16,
    AT24C32_PAGE_SIZE = 32,
    AT24C64_PAGE_SIZE = 32,
    
}AT24CXX_PAGE_SIZE;

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * AT24CXX相关控制接口
 ****************************************************************************/

/**
  * @brief  设置外部控制接口
  * @param  Data 端口初始化函数
  * @param  SetSDA 设置SDA的电平
  * @param  SetSCL 设置SCL的电平
  * @param  GetSDA 获取SDA的电平
  * @param  GetSCL 获取SCL的电平
  * @param  BitDelay 微秒级延时函数
  * @retval 无
  */
void AT24Cxx_InitInterface(void   *Data,
                           void   (*SetSDA)  (void *Data, uBit8 State),
                           void   (*SetSCL)  (void *Data, uBit8 State),
                           uBit8  (*GetSDA)  (void *Data),
                           uBit8  (*GetSCL)  (void *Data),
                           void   (*BitDelay)(void *Data),
                           AT24CXX_PAGE_SIZE    PAGE_SIZE);


/**
  * @brief  AT24Cxx字节写函数
  * @param  cAddr 要写的地址
  * @param  cWriteData 要写的数据
  * @retval 无
  */
void AT24Cxx_WriteByte(uBit16 nAddr, uBit8 cWriteData);


/**
  * @brief  AT24Cxx随机读函数
  * @param  cAddr 要读的地址
  * @retval cReadData 读取到的数据
  */
uBit8 AT24Cxx_RandomRead(uBit16 nAddr);


/**
  * @brief  AT24Cxx页写函数,只能写一页
  * @param  cAddr 要写的地址
  * @param  cNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  * @note   对于1K/2K的EEPROM, 8字节为1页, 对于4/8/16K的,16字节为之1页.
  *         对于跨页的写, 会在页尾进行环回到页首
  */
void AT24Cxx_PageWrite(uBit16 nAddr, const uBit8 *pWriteBuff, uBit8 cNum);


/**
  * @brief  AT24Cx连续读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   一次读取的数据不可以超过256个字节,如果需要读取更多的数据,需要分批来读取
  */
void AT24Cxx_SequentialRead(uBit16 nAddr, uBit8 *pReadBuff, uBit16 nNum);


/**
  * @brief  AT24Cxx多字节跨页读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   是对连续读函数的封装,如果一次要读的数据超过256字节,则分批读取
  */
void AT24Cxx_ReadMultiBytes(uBit16 nAddr, uBit8 *pReadBuff, uBit32 iNum);


/**
  * @brief  AT24Cxx多字节跨页写函数
  * @param  nAddr 要写的地址
  * @param  nNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  */
void AT24Cxx_WriteMultiBytes(uBit16 nAddr, const uBit8 *pWriteBuff, uBit32 iNum);


#ifdef __cplusplus
}
#endif
                                  

#endif  /* __AT24CXX_DEV_H */
