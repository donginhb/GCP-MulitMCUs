/**
  ******************************************************************************
  * @file    LogicIC.c
  * @author  Duhanfeng
  * @version V1.1
  * @date    2017.04.12
  * @brief   about some logic ic drivers
  ******************************************************************************
  * @attention
  * 
  * 74HC164:  串转并
  * 74HC4052: 双闸模拟开关  PORT_INH  PORT_SEL0  PORT_SEL1
  * 
  * V1.0------------
  * 修改描述: 增加74HC4052模拟开关的驱动文件
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.0
  * 修改日期: 2017.04.11
  * 
  * V1.1------------
  * 修改描述: 增加74HC595模拟开关的驱动文件
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2017.04.12
  * 
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "LogicIC.h"
#include "DataType/DataType.h"

      
/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define GET_BOOL(x)     ((x) ? (1) : (0))
      
      
/*****************************************************************************
 * 74HC405x 多路X通道模拟开关
 ****************************************************************************/
 
static void    *HC405x_PrivateData;
static void    (*HC405x_SetINH)  (void *pData, uBit8 State);
static void    (*HC405x_SetSEL0) (void *pData, uBit8 State);
static void    (*HC405x_SetSEL1) (void *pData, uBit8 State);
static void    (*HC405x_SetSEL2) (void *pData, uBit8 State); //在4052不需要

#define HC405x_INH(x)   HC405x_SetINH (HC405x_PrivateData, (x))
#define HC405x_SEL0(x)  HC405x_SetSEL0(HC405x_PrivateData, (x))
#define HC405x_SEL1(x)  HC405x_SetSEL1(HC405x_PrivateData, (x))
#define HC405x_SEL2(x)  HC405x_SetSEL2(HC405x_PrivateData, (x))


/**
  * @brief  74HC595硬件控制接口初始化
  * @param  Data 私有数据
  * @param  SetINH INH设置
  * @param  SetSEL0 SEL0设置
  * @param  SetSEL1 SEL1设置
  * @param  SetSEL2 SEL2设置
  * @retval None
  */
void HC405x_HwCtrlInterFaces(void    *pData,
                             void    (*SetINH) (void *pData, uBit8 State),
                             void    (*SetSEL0)(void *pData, uBit8 State),
                             void    (*SetSEL1)(void *pData, uBit8 State),
                             void    (*SetSEL2)(void *pData, uBit8 State))
{
    HC405x_PrivateData = pData;
    HC405x_SetINH  = SetINH;
    HC405x_SetSEL0 = SetSEL0;
    HC405x_SetSEL1 = SetSEL1;
    HC405x_SetSEL2 = SetSEL2;
    
}
  

/**
  * @brief  405X使能(INH脚低电平有效)
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC405x_Enable(uBit8 isEnable)
{
    HC405x_INH(!GET_BOOL(isEnable));
    
}


/**
  * @brief  405X通道选择
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC405x_SetChannel(uBit8 uChannel)
{
    HC405x_Enable(uChannel != 0xFF);  //如果通道为0xFF,则关闭
    
    if (HC405x_SetSEL2)
    {
        HC405x_SEL2(GET_BOOL(uChannel & 0x04));
    }
    HC405x_SEL1(GET_BOOL(uChannel & 0x02));
    HC405x_SEL0(GET_BOOL(uChannel & 0x01));
    
}


/*****************************************************************************
 * 74HC595 串转并芯片
 ****************************************************************************/

static void    *HC595_PrivateData;
static void    (*HC595_SetSHCP) (void *pData, uBit8 State);
static void    (*HC595_SetSTCP) (void *pData, uBit8 State);
static void    (*HC595_SetDATA) (void *pData, uBit8 State);
static void    (*HC595_DelayUs) (uBit32 Us);
static uBit8 HC595_ParBitNum = 0;

#define HC595_SHCP(x)   HC595_SetSHCP(HC595_PrivateData, (x))
#define HC595_STCP(x)   HC595_SetSTCP(HC595_PrivateData, (x))
#define HC595_DATA(x)   HC595_SetDATA(HC595_PrivateData, (x))


/**
  * @brief  74HC595硬件控制接口初始化
  * @param  Data 私有数据
  * @param  SetSHCP SHCP设置
  * @param  SetSTCP STCP设置
  * @param  SetDATA DATA设置
  * @param  HC_DelayUs 微秒级延时函数接口
  * @param  uParBitNum 并行位数
  * @retval None
  */
void HC595_HwCtrlInterFaces(void    *pData,
                            void    (*SetSHCP)(void *pData, uBit8 State),
                            void    (*SetSTCP)(void *pData, uBit8 State),
                            void    (*SetDATA)(void *pData, uBit8 State),
                            void    (*HC_DelayUs)(uBit32 Us),
                            uBit8 uParBitNum)
{
    HC595_PrivateData = pData; //私有数据
    HC595_SetSHCP = SetSHCP;
    HC595_SetSTCP = SetSTCP;
    HC595_SetDATA = SetDATA;
    HC595_DelayUs = HC_DelayUs;
    HC595_ParBitNum = uParBitNum;
    
}


/**
  * @brief  并型数据输出
  * @param  iSerialData 要输出的数据
  * @retval None
  */
void HC595_OutputParallelData(uBit32 iSerialData)
{
    uBit8 i = 0;
    
    //数据移位
    for (i = 0; i < HC595_ParBitNum; i++)
    {
        HC595_SHCP(1);
        HC595_DelayUs(5);
        
        HC595_SHCP(0);
        
        HC595_DATA((iSerialData & (0x1<<((HC595_ParBitNum - 1)-i))) ? 1 : 0);  //从高位到低位扫描
        
        HC595_DelayUs(5);
        
        HC595_SHCP(1);
    }
    
    //数据锁存
    HC595_STCP(0);
    HC595_DelayUs(5);
    HC595_STCP(1);
    
}
