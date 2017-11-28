/**
  ******************************************************************************
  * @file    TM1617.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.09
  * @brief   Nixie Tube Device IC Driver
  ******************************************************************************
  * @attention
  * 
  * TM1617可以驱动8段*2或者是7段*3两种显示方式,本驱动程序用于8*2的显示方式.
  * 
  * 控制接口: DIO(数据引脚), CLK(时钟引脚), STB(片选引脚)
  * 
  * 时序: STB低电平有效,CLK空闲电平为高电平,IC在CLK上升沿时读取数据,
  *       MCU若要改变数据脚电平,在CLK高电平时进行
  * 
  * 注: 在直接的IO控制之后,最好加一小段延时(>=100ns),以保证数据传输的稳定
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "TM1617.h"
#include "DataType/DataType.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

typedef struct 
{
    void (*pf_SetDIO)(bool bState);     //数据引脚状态控制
    void (*pf_SetCLK)(bool bState);     //时钟引脚状态控制
    void (*pf_SetSTB)(bool bState);     //片选引脚状态控制
    
    uBit32 ulOpsValidFlag;
    
}TM_BIT_OPS;

static TM_BIT_OPS m_TmBitOps = {0};


//数码管共阴极码表
const static uBit8 m_CathodeTubeTable[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
                                            0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };


/**
  * @brief  简单的延时函数
  * @param  None
  * @retval None
  * @note   简单的延时,未确定具体延时时间,用于IO状态设置后状态稳定
  */
void TM1617_SimpleDelay(void)
{
    for(volatile int i = 0; i < 100; i++);
    
}


/**
  * @brief  DIO引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TM1617_DIO(bool bState)
{
    m_TmBitOps.pf_SetDIO(bState);
    TM1617_SimpleDelay();
    
}


/**
  * @brief  CLK引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TM1617_CLK(bool bState)
{
    m_TmBitOps.pf_SetCLK(bState);
    TM1617_SimpleDelay();
    
}


/**
  * @brief  STB引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TM1617_STB(bool bState)
{
    m_TmBitOps.pf_SetSTB(bState);
    TM1617_SimpleDelay();
    
}


/**
  * @brief  指令发送
  * @param  uCmd  命令
  * @retval None
  * @note   直接的IO控制中,时钟信号的频率大约为500~600KHz. 
  *         IC在CLK上升沿时读取数据,MCU若要改变数据脚电平,在CLK高电平时进行
  */
static void TM1617_SendCmd(uBit8 uCmd)
{
    TM1617_STB(false);
    
    for (int i = 0; i < 8; i++)
    {
        TM1617_CLK(true);
        TM1617_DIO(uCmd & (0x1<<(i)));
        TM1617_CLK(false);
    }
    
    TM1617_CLK(true);
    TM1617_STB(true);
    
}


/**
  * @brief  数据指令发送(命令+数据)
  * @param  uCmd  命令
  * @param  cData 数据
  * @retval None
  */
static void TM1617_SendData(uBit8 uCmd, uBit8 cData)
{
    TM1617_STB(false);
    
    for (int i = 0; i < 8; i++)
    {
        TM1617_CLK(true);
        TM1617_DIO(uCmd & (0x1<<(i)));
        TM1617_CLK(false);
    }
    
    TM1617_CLK(true);
    
    for (int i = 0; i < 8; i++)
    {
        TM1617_CLK(true);
        TM1617_DIO(cData & (0x1<<(i)));
        TM1617_CLK(false);
    }
    
    TM1617_CLK(true);
    TM1617_STB(true);
    
}


/*****************************************************************************
 * TM1617数码管驱动IC控制接口
 ****************************************************************************/

/**
  * @brief  TM1617控制引脚编号设置
  * @param  pf_SetDIO 数据引脚控制接口
  * @param  pf_SetCLK 时钟引脚控制接口
  * @param  pf_SetSTB 片选引脚控制接口
  * @retval None
  */
void TM1617_SetCtrlPin(void (*pf_SetDIO)(bool bState), 
                       void (*pf_SetCLK)(bool bState), 
                       void (*pf_SetSTB)(bool bState))
{
    if ((pf_SetDIO != NULL) && (pf_SetCLK != NULL) && (pf_SetSTB != NULL))
    
    m_TmBitOps.pf_SetDIO = pf_SetDIO;
    m_TmBitOps.pf_SetCLK = pf_SetCLK;
    m_TmBitOps.pf_SetSTB = pf_SetSTB;
    
    m_TmBitOps.ulOpsValidFlag = 1;
    
}


/**
  * @brief  数码管数据显示函数
  * @param  uHexData 要显示的数据
  * @retval 0-成功 非0-失败
  */
uBit32 TM1617_DisNumber(uBit8 uHexData)
{
    if (m_TmBitOps.ulOpsValidFlag == 0)
    {
        return 1;
    }
    
    uBit8 Data[4] = {  m_CathodeTubeTable[uHexData >> 4]  << 4,
                       m_CathodeTubeTable[uHexData >> 4]  >> 4,
                       m_CathodeTubeTable[uHexData & 0x0F] << 4,
                       m_CathodeTubeTable[uHexData & 0x0F] >> 4};
    
    TM1617_SendCmd(0x03);    //显示模式: 2位8段
    TM1617_SendCmd(0x44);    //数据命令: 非地址自增模式
    
    for (int i = 0; i < 4; i++)
    {
        TM1617_SendData(0xC0 + i, Data[i]);    //设置数据
    }
    
    TM1617_SendCmd(0x8C);
    
    return 0;
}
