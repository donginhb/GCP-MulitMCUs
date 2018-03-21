/**
  ******************************************************************************
  * @file    TLC5615.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.03.15
  * @brief   Digital-to-Analog Converter IC Driver
  ******************************************************************************
  * @attention
  * 
  * TLC5615是一款数模转换器,其输出为电压型.最大输出电压是基准电压值的两倍.
  * 
  * 控制接口: DIN(数据引脚), SCLK(时钟引脚), CS(片选引脚)  DOUT(级联时的串行数据输出)
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
#include "TLC5615.h"
#include "DataType/DataType.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define TLC5615_DATA_BIT        (12)

typedef struct 
{
    void (*pf_SetDIO)(bool bState);     //数据引脚状态控制
    void (*pf_SetCLK)(bool bState);     //时钟引脚状态控制
    void (*pf_SetCS)(bool bState);      //片选引脚状态控制
    
    bool bOpsValidFlag;
    
}TLC5615_BIT_OPS;

static TLC5615_BIT_OPS m_TlcBitOps = {0};


/**
  * @brief  简单的延时函数
  * @param  None
  * @retval None
  * @note   简单的延时,未确定具体延时时间,用于IO状态设置后状态稳定
  */
void TLC5615_SimpleDelay(void)
{
    for(volatile int i = 0; i < 100; i++);
    
}


/**
  * @brief  DIO引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_DIO(bool bState)
{
    m_TlcBitOps.pf_SetDIO(bState);
    TLC5615_SimpleDelay();
    
}


/**
  * @brief  CLK引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_CLK(bool bState)
{
    m_TlcBitOps.pf_SetCLK(bState);
    TLC5615_SimpleDelay();
    
}


/**
  * @brief  STB引脚控制
  * @param  bState IO状态
  * @retval None
  */
static void TLC5615_CS(bool bState)
{
    m_TlcBitOps.pf_SetCS(bState);
    TLC5615_SimpleDelay();
    
}


/**
  * @brief  TLC5615 数据设置
  * @param  uHexData 要设置的数据
  * @retval None
  */
static void TLC5615_SetData(uBit32 ulWriteData)
{
    unsigned int ulTrueWriteData = ulWriteData << 2;  //最低位补两位0
        
    //片选使能
    TLC5615_CS(false);  
    
    //写入数据
    for (int i = 0; i < TLC5615_DATA_BIT; i++)
    {
        TLC5615_DIO((ulTrueWriteData & (0x1<<((TLC5615_DATA_BIT-1)-i))) ? true : false);  
        TLC5615_CLK(true) ; //上升沿送数据
        TLC5615_CLK(false); 
    }
    
    //片选禁止
    TLC5615_CS(true);
    
}


/*****************************************************************************
 * TLC5615数模转换IC控制接口
 ****************************************************************************/

/**
  * @brief  TLC5615接口初始化
  * @param  pf_SetDIO 数据引脚控制接口
  * @param  pf_SetCLK 时钟引脚控制接口
  * @param  pf_SetCS 片选引脚控制接口
  * @retval 0-成功  非0-失败
  */
uBit32 TLC5615_InitInterface(void (*pf_SetDIO)(bool bState), 
                             void (*pf_SetCLK)(bool bState), 
                             void (*pf_SetCS)(bool bState))
{
    do 
    {
        if (pf_SetDIO == NULL) break;
        if (pf_SetCLK == NULL) break;
        if (pf_SetCS == NULL) break;
        
        m_TlcBitOps.pf_SetDIO = pf_SetDIO;
        m_TlcBitOps.pf_SetCLK = pf_SetCLK;
        m_TlcBitOps.pf_SetCS = pf_SetCS;
        
        m_TlcBitOps.bOpsValidFlag = true;
        
        //设置默认电平
        TLC5615_CLK(false); 
        TLC5615_DIO(false); 
        TLC5615_CS(true);
        
        return 0;
    }while (0);
    
    return 1;
}


/**
  * @brief  TLC5615模拟量输出设置
  * @param  ulWriteData 数字量数据,范围为[0,0x3FF],对应[0,2Vef]
  * @retval 0-成功 非0-失败
  */
uBit32 TLC5615_SetAnalogOutput(uBit32 ulWriteData)
{
    if (!m_TlcBitOps.bOpsValidFlag)
    {
        return 1;
    }
    
    TLC5615_SetData(ulWriteData);
    
    return 0;
}
