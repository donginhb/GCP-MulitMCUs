/**
  ******************************************************************************
  * @file    HW_ADC.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.20
  * @brief   Hardware adc driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 引脚,
  *    AD[0] --- P0[23]  FUNC1
  *    AD[1] --- P0[24]  FUNC1
  *    AD[2] --- P0[25]  FUNC1
  *    AD[3] --- P0[26]  FUNC1
  *    AD[4] --- P1[30]  FUNC2
  *    AD[5] --- P1[31]  FUNC2
  *    AD[6] --- P0[ 3]  FUNC2
  *    AD[7] --- P0[ 2]  FUNC2
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_ADC.h"
#include "chip.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define HW_ADC_COUNT            (3)             //ADC数量
#define HW_ADC_CH_COUNT         (8)             //ADC通道数
#define HW_ADC_CH_MASK(n)       ((n)&0xFF)      //通道掩码

static bool m_bBurstModeFlag = false;



/**
  * @brief  ADC引脚配置
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @retval None
  */
static void HW_ADC_PortConfig(uint8_t uAdcNode, uint32_t ulChMask)
{
    for (int i = 0; i < HW_ADC_CH_COUNT; i++)
    {
        if (HW_ADC_CH_MASK(ulChMask) & (0x1<<i))
        {
            
        }
    }
    
}

/*****************************************************************************
 * ADC相关控制接口
 ****************************************************************************/

/**
  * @brief  ADC模式使能
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @param  bBurstModeFlag Burst模式
  * @retval 0-成功 非0-失败
  * @note   非Burst模式下,只能使能一个通道,若入参多个通道,则默认只能最小的通道
  */
uint32_t HW_ADC_Init(uint8_t uAdcNode, uint32_t ulChMask, bool bBurstModeFlag)
{


    return 0;
}


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueNoBlock(uint8_t uAdcNode, uint32_t ulChannel, uint16_t *pAdcValue)
{
    //假如转换完成,则将数据出参,并返回0,否则返回1
    
    
    return 1;
}


/**
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  ulChannel 通道编号
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueBlock(uint8_t uAdcNode, uint32_t ulChannel, uint16_t *pAdcValue)
{
    //等待转换完成
    
    
    //读取采样值
    
    
    //返回执行结果
    return 0;
}


/**
  * @brief  ADC采集开始
  * @param  uAdcNode ADC节点
  * @retval None
  */
void HW_ADC_Start(uint8_t uAdcNode)
{
    
}

