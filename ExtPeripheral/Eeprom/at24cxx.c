/**
  ******************************************************************************
  * @file    at24cxx.c
  * @author  Duhanfeng
  * @version V2.0 
  * @date    2017.03.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 内存: 1/2/4/8/16K (AT24C01/02/04/08/16)
  * 数据: 高位先发 
  *
  * A2A1A0: 本文件默认芯片A2A1A0位接地,若实际与此不同,需修改宏 AT_BASE_ADDR
  *
  * 由于AT24Cxx芯片特性限制,AT24Cxx不能进行跨页(每8/16个字节为1页)写操作,所以设
  * 计了跨页写的算法.
  * 
  * 注: 1K/2K内存的芯片, 每8个字节为1页. 而4/8/16K的, 16个字节为1页.
  * 
  * V2.0------------
  * 修改描述: 1.将AT24Cxx的底层时序实现交由I2C框架去完成,本驱动只需要关心其功能
  *             的实现.而相关的接口由外界的函数指针提供
  *           2.修复AT24Cxx无法连续读大于256个字节的限制(无跨页限制).如果一次性
  *             读取大于256个字节,则分批执行
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2017.03.20
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "at24cxx.h"
#include "DataType/DataType.h"
#include "SysPeripheral/I2C/I2C_Interface.h"
#include "SysPeripheral/SysTimer/SysTimer.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define AT_BASE_ADDR            (0XA0)              //设备地址

static I2C_BIT_OPS_TYPE         m_I2C_BitOps = {0}; //I2C 位操作接口
static I2C_BUS_DEVICE_OPS_TYPE  m_AT_I2C_DEV = {0}; //I2C 设备接口

static AT24CXX_PAGE_SIZE        AT_PAGE_SIZE;       //AT24CXX页大小



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
                           AT24CXX_PAGE_SIZE    PAGE_SIZE)
{
    m_I2C_BitOps.Data = Data;
    m_I2C_BitOps.pf_I2C_SetSDA = SetSDA;
    m_I2C_BitOps.pf_I2C_SetSCL = SetSCL;
    m_I2C_BitOps.pf_I2C_GetSDA = GetSDA;
    m_I2C_BitOps.pf_I2C_GetSCL = GetSCL;
    m_I2C_BitOps.pf_I2C_BitDelay = BitDelay;
    
    m_I2C_BitOps.nTimeOut = 50;
    
    //初始化I2C设备控制接口
    I2C_BusDevieInit(&m_AT_I2C_DEV);
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_CLEAR_ALL_FLAGS, NULL);
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_BIT_OPS, &m_I2C_BitOps);
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_TIMEOUT, &m_I2C_BitOps.nTimeOut);
    
    AT_PAGE_SIZE = PAGE_SIZE;
    
    //如果是32KBit以上的EEPROM,则设置为16位寄存器地址
    if (AT_PAGE_SIZE >= AT24C32_PAGE_SIZE)
    {
        I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_REG_ADDR_16BIT, NULL);
    }
    
}


/**
  * @brief  AT24Cxx字节写函数
  * @param  cAddr 要写的地址
  * @param  cWriteData 要写的数据
  * @retval 无
  */
void AT24Cxx_WriteByte(uBit16 nAddr, uBit8 cWriteData)
{
    uBit16 cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
    
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
    I2C_BusDeviceWrite(&m_AT_I2C_DEV, nAddr, &cWriteData, 1);
    
    SysTime_DelayMs(5);   //写操作时,必须要等待5ms,等待数据从缓存器往AT24Cxx写入完成
}


/**
  * @brief  AT24Cxx随机读函数
  * @param  cAddr 要读的地址
  * @retval cReadData 读取到的数据
  */
uBit8 AT24Cxx_RandomRead(uBit16 nAddr)
{
    uBit8 cReadData = 0;
    uBit16 cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
    
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
    I2C_BusDeviceRead(&m_AT_I2C_DEV, nAddr, &cReadData, 1);
    
    return cReadData;
}


/**
  * @brief  AT24Cxx页写函数,只能写一页
  * @param  cAddr 要写的地址
  * @param  cNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  * @note   对于1K/2K的EEPROM, 8字节为1页, 对于4/8/16K的,16字节为之1页.
  *         对于跨页的写, 会在页尾进行环回到页首
  */
void AT24Cxx_PageWrite(uBit16 nAddr, const uBit8 *pWriteBuff, uBit8 cNum)
{
    uBit16 cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
    
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
    I2C_BusDeviceWrite(&m_AT_I2C_DEV, nAddr, pWriteBuff, cNum);
    
    SysTime_DelayMs(5);   //写操作时,必须要等待5ms,等待数据从缓存器往AT24Cxx写入完成
    
}


/**
  * @brief  AT24Cx连续读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   一次读取的数据不可以超过256个字节,如果需要读取更多的数据,需要分批来读取
  */
void AT24Cxx_SequentialRead(uBit16 nAddr, uBit8 *pReadBuff, uBit16 nNum)
{
    uBit16 cDevAddr = 0;
    
    nNum = (nNum > 256) ? 256 : nNum; //AT24Cxx 最高只能支持256个字节的连续接收
    
    cDevAddr = AT_BASE_ADDR | ((AT_PAGE_SIZE >= AT24C32_PAGE_SIZE) ? 0 : ((nAddr&0x0700)>>7));
    
    I2C_BusDeviceControl(&m_AT_I2C_DEV, I2C_DEV_CTRL_SET_DEV_ADDR, &cDevAddr);
    I2C_BusDeviceRead(&m_AT_I2C_DEV, nAddr, pReadBuff, nNum);
    
}


/**
  * @brief  AT24Cxx多字节跨页读函数
  * @param  cAddr 要读的地址
  * @param  cNum  要读的数量
  * @param  pReadBuff 要读的内容(数组/指针的形式)
  * @retval 无
  * @note   是对连续读函数的封装,如果一次要读的数据超过256字节,则分批读取
  */
void AT24Cxx_ReadMultiBytes(uBit16 nAddr, uBit8 *pReadBuff, uBit32 iNum)
{
    uBit16 i = 0;
    uBit16 nBloakNum = iNum/256;
    
    for (i = 0 ; i < nBloakNum; i++)
    {
        AT24Cxx_SequentialRead(nAddr+i*256, &pReadBuff[i*256], 256);
    }
    
    AT24Cxx_SequentialRead(nAddr+i*256, &pReadBuff[i*256], iNum%256);
    
}


/**
  * @brief  AT24Cxx多字节跨页写函数
  * @param  nAddr 要写的地址
  * @param  nNum  要写的数量
  * @param  pWriteBuff 要写的内容(数组/字符串/指针的形式)
  * @retval 无
  */
void AT24Cxx_WriteMultiBytes(uBit16 nAddr, const uBit8 *pWriteBuff, uBit32 iNum)
{
    uBit8 i = 0;
    uBit16 nSurplusNum = iNum;  // 剩余未写入的数量
    uBit16 nCurrentPageNum = 0; // 当前阶段需要写入的页数
    
    uBit16 nCurrentAddr = nAddr;  // 当前操作的地址
    const uBit8 *pStoreBuff = pWriteBuff; // 当前存储的数据缓存区
    uBit16 nCurrentWriteNum = 0;  // 当前需要写入的数量
    
    // 第一阶段
    if (nCurrentAddr & (AT_PAGE_SIZE - 1)) // 假如当前地址并未与页地址对齐
    {
        nCurrentWriteNum = AT_PAGE_SIZE - (nCurrentAddr & (AT_PAGE_SIZE - 1));
        
        if (nSurplusNum < nCurrentWriteNum) // 假如剩余的数量少于当前可写的数量
        {
            nCurrentWriteNum = nSurplusNum; 
        }
        
        AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
        
        pStoreBuff   += nCurrentWriteNum; // 指针偏移
        nSurplusNum  -= nCurrentWriteNum; // 更新剩余的数据的数量
        nCurrentAddr += nCurrentWriteNum; // 更新当前要写的地址
    }
    
    // 第二阶段
    nCurrentPageNum = nSurplusNum/AT_PAGE_SIZE;
    
    nCurrentWriteNum = AT_PAGE_SIZE;
    for (i = 0; i < nCurrentPageNum; i++)
    {
        AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nCurrentWriteNum);
        
        pStoreBuff   += nCurrentWriteNum; // 指针偏移
        nSurplusNum  -= nCurrentWriteNum; // 更新剩余的数据的数量
        nCurrentAddr += nCurrentWriteNum; // 更新当前要写的地址
    }
    
    // 第三阶段
    if (nSurplusNum != 0)
    {
        AT24Cxx_PageWrite(nCurrentAddr, pStoreBuff, nSurplusNum);
    }
    
}

