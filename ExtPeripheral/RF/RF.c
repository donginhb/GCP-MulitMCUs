/**
  ******************************************************************************
  * @file    RF.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.25
  * @brief   Radio frequency communication application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "RF.h"
#include "NRF24L01.h"
#include "DataType/DataType.h"
#include "SysPeripheral/SPI/SPI.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit8  m_uSpiNode = 0;
static uBit32 m_ulIRQNo  = 0;
static uBit32 m_ulCSNNo = 0;
static uBit32 m_ulCENo   = 0;


/**
  * @brief  数码管DIO引脚设置
  * @param  None
  * @retval IRQ电平状态
  */
static bool GetIRQ(void)
{
    
    return GPIO_GetInputState(m_ulIRQNo);
}


/**
  * @brief  数码管CLK引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetCSN(bool bState)
{
    GPIO_SetOutputState(m_ulCSNNo, bState);
    
}


/**
  * @brief  数码管STB引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetCE(bool bState)
{
    GPIO_SetOutputState(m_ulCENo, bState);
    
}


/**
  * @brief  SPI数据读写
  * @param  nWriteData 要写入的数据
  * @retval 读取到的数据
  */
static uBit8 RF_ReadWriteByte(uBit8 cWriteData)
{
    
    return SPI_ReadWriteByte(m_uSpiNode, cWriteData);
}



/*****************************************************************************
 * RF相关控制接口
 ****************************************************************************/

/**
  * @brief  RF模块初始化
  * @param  uSpiNode SPI节点号
  * @param  ulIRQNo  IRQ引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCSNNo  CSN引脚编号(IO资源表中输出IO组的位置)
  * @param  ulCENo   CE引脚编号(IO资源表中输出IO组的位置)
  * @retval 0-成功 非0-失败
  */
uBit32 RF_Init(uBit8 uSpiNode, uBit32 ulIRQNo, uBit32 ulCSNNo, uBit32 ulCENo)
{
    m_uSpiNode = uSpiNode;
    m_ulIRQNo = ulIRQNo;
    m_ulCSNNo = ulCSNNo;
    m_ulCENo = ulCENo;
    
    //初始化SPI
    SPI_Init(uSpiNode, SPI_MASTER_MODE);
    SPI_SetSpeed(uSpiNode, SPI_SPEED_M);
    
    //设置控制接口
    uBit32 ulRet = nRF24L01_InitInterFaces(RF_ReadWriteByte,
                                             SetCSN,
                                             SetCE,
                                             GetIRQ);
    
    if (ulRet != 0)
    {
        return 1;
    }
    
    ulRet = nRF24L01_CheckConnect();
    
    return ulRet;
}


/**
  * @brief  NRF地址设置
  * @param  pRFAddr 地址指针,长度为5个字节
  * @retval None
  * @note   发送和接收的节点需要相同地址才能通信
  */
void RF_SetAddress(uBit8 *pRFAddr)
{
    nRF24L01_SetAddress(pRFAddr);
    
}


/**
  * @brief  数组数据发送
  * @param  pTBuff 发送缓冲区,最大31个字节
  * @param  ulSize 要发送的数据字节数
  * @retval 发送结果
  * @note   值得注意的是,这个函数不能在本模块的中断里面执行
  */
uBit8 RF_SendBuff(uBit8 *pTBuff, uBit8 uSize)
{
    
    return nRF24L01_SendBuff(pTBuff, uSize);
}


/**
  * @brief  数据接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uBit32 RF_RecvBuff(void *pRBuff, uBit32 ulSize)
{
    
    return nRF24L01_RecvBuff(pRBuff, ulSize);
}


/**
  * @brief  接收处理
  * @param  None
  * @retval None
  */
void RF_RecvHandler(void)
{
    nRF24L01_RecvHandler();
    
}







