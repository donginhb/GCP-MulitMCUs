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
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_ADC.h"
#include "HW_Gpio.h"
#include "chip.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define HW_ADC_COUNT            (3)             //ADC数量
#define HW_ADC_CH_COUNT         (16)             //ADC通道数
#define HW_ADC_CH_MASK(n)       ((n)&0xFFFF)      //通道掩码

ADC_TypeDef * const ADC[HW_ADC_COUNT] = { ADC1, ADC2, ADC3 };

/**
  * @brief  简单的微秒级延时函数
  * @param  ulDelay 延时参数,单位为微秒
  * @retval None
  */
static void HW_SimpleDelayUs(uint32_t ulDelay)
{
    for(volatile int i = 0; i < ulDelay*15; i++);
    
}


/**
  * @brief  ADC引脚配置
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @retval None
  */
static void HW_ADC_PortConfig(uint8_t uAdcNode, uint32_t ulChMask)
{
    HW_GPIO_PORT ADC_CH_PORT[HW_ADC_CH_COUNT] = 
    { 
        HW_GPIO_PORT_A, HW_GPIO_PORT_A, HW_GPIO_PORT_A, HW_GPIO_PORT_A, 
        HW_GPIO_PORT_A, HW_GPIO_PORT_A, HW_GPIO_PORT_A, HW_GPIO_PORT_A, 
        HW_GPIO_PORT_B, HW_GPIO_PORT_B, HW_GPIO_PORT_C, HW_GPIO_PORT_C, 
        HW_GPIO_PORT_C, HW_GPIO_PORT_C, HW_GPIO_PORT_C, HW_GPIO_PORT_C,
    };
    
    HW_GPIO_PIN ADC_CH_PIN[HW_ADC_CH_COUNT] = 
    { 
        HW_GPIO_PIN_0,  HW_GPIO_PIN_1,  HW_GPIO_PIN_2,  HW_GPIO_PIN_3,  
        HW_GPIO_PIN_4,  HW_GPIO_PIN_5,  HW_GPIO_PIN_6,  HW_GPIO_PIN_7,
        HW_GPIO_PIN_0,  HW_GPIO_PIN_1,  HW_GPIO_PIN_0,  HW_GPIO_PIN_1,  
        HW_GPIO_PIN_2,  HW_GPIO_PIN_3,  HW_GPIO_PIN_4,  HW_GPIO_PIN_5 
    };
    
    //初始化IO
    for (int i = 0; i < HW_ADC_CH_COUNT; i++)
    {
        if (HW_ADC_CH_MASK(ulChMask) & (0x1<<i))
        {
            HW_GPIO_ModeConfig(ADC_CH_PORT[i], ADC_CH_PIN[i], HW_GPIO_ANALOG);
        }
    }
    
}


/**
  * @brief  ADC转换顺序设置
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @retval 0-成功 非0-失败
  */
static void HW_ADC_SetConvertSequence(uint8_t uChNumber, uint8_t uSequence)
{
    switch (uSequence)
    {
    case  1: ADC1->SQR3 |= (uChNumber << (0*5)); break;
    case  2: ADC1->SQR3 |= (uChNumber << (1*5)); break;
    case  3: ADC1->SQR3 |= (uChNumber << (2*5)); break;
    case  4: ADC1->SQR3 |= (uChNumber << (3*5)); break;
    case  5: ADC1->SQR3 |= (uChNumber << (4*5)); break;
    case  6: ADC1->SQR3 |= (uChNumber << (5*5)); break;
    case  7: ADC1->SQR2 |= (uChNumber << (0*5)); break;
    case  8: ADC1->SQR2 |= (uChNumber << (1*5)); break;
    case  9: ADC1->SQR2 |= (uChNumber << (2*5)); break;
    case 10: ADC1->SQR2 |= (uChNumber << (3*5)); break;
    case 11: ADC1->SQR2 |= (uChNumber << (4*5)); break;
    case 12: ADC1->SQR2 |= (uChNumber << (5*5)); break;
    case 13: ADC1->SQR1 |= (uChNumber << (1*5)); break;
    case 14: ADC1->SQR1 |= (uChNumber << (2*5)); break;
    case 15: ADC1->SQR1 |= (uChNumber << (3*5)); break;
    case 16: ADC1->SQR1 |= (uChNumber << (4*5)); break;
    default: break;
    }
}


/**
  * @brief  ADC模式配置
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @param  bScanModeFlag 扫描模式
  * @retval 0-成功 非0-失败
  * @note   非扫描模式下,只能使能一个通道,若入参多个通道,则默认只能最小的通道
  */
static void HW_ADC_ConfigMode(uint8_t uAdcNode, uint32_t ulChMask, bool bScanModeFlag)
{
    //开ADC1时钟 
    RCC->APB2ENR |= (0x1<<9);   //ADC1
    
    //时钟分频 
    RCC->CFGR &= ~(0x3<<14);
    RCC->CFGR |=  (0x2<<14);    //6分频(ADC的时钟频率不得超过14M,此分频后为12M)
    
    //配置工作模式 
    ADC1->CR1 &= ~(0x1<<23);    //规则组:关闭模拟看门狗
    ADC1->CR1 &= ~(0x1<<22);    //注入组:关闭模拟看门狗
    ADC1->CR1 &= ~(0xF<<16);   
    ADC1->CR1 |=  (0x0<<16);    //独立模式(常用)
    
    ADC1->CR1 &= ~(0x1<<12);    //注入组:禁用间断模式
    ADC1->CR1 &= ~(0x1<<11);    //规则组:禁用间断模式
    ADC1->CR1 &= ~(0x1<<10);    //关闭注入组自动转换
    
#if 0
    if (bScanModeFlag)
    {
        ADC1->CR1 |=  (0x1<<8); //开启扫描模式 (如果有多个通道或者外部事件触发, 必须要开启)
    }
    else 
    {
        ADC1->CR1 &= ~(0x1<<8); //关闭扫描模式
    }
#else 
    ADC1->CR1 &= ~(0x1<<8); //关闭扫描模式
#endif
    
#if 0
    ADC1->CR1 &= ~(0x1<<7);
    ADC1->CR1 &= ~(0x1<<6);
    ADC1->CR1 |=  (0x1<<5);     //开中断
#else   
    ADC1->CR1 &= ~(0x1<<7);
    ADC1->CR1 &= ~(0x1<<6);
    ADC1->CR1 &= ~(0x1<<5);     //关中断
#endif  
        
    ADC1->CR2 &= ~(0x1<<23);    //禁止温度传感器和V_REFINT
        
#if 0   
    ADC1->CR2 |=  (0x1<<20);    //规则组:使用外部触发转换模式
    ADC1->CR2 &= ~(0x7<<17);    
    ADC1->CR2 |=  (0x5<<17);    //规则组:TIM4的CC4事件触发
#else   
    ADC1->CR2 &= ~(0x1<<20);    //规则组:关闭外部条件出发
    ADC1->CR2 &= ~(0x7<<17);    
    ADC1->CR2 |=  (0x7<<17);    //规则组:软件触发
#endif  
        
    ADC1->CR2 &= ~(0x1<<11);    //数据右对齐
    ADC1->CR2 &= ~(0x1<<8);     //不使用DMA模式
    //ADC1->CR2 &= ~(0x1<<1);     //ADC1单次转换模式
    
    if (bScanModeFlag)
    {
        ADC1->CR2 |=  (0x1<<1); //开启扫描模式 (如果有多个通道或者外部事件触发, 必须要开启)
    }
    else 
    {
        ADC1->CR2 &= ~(0x1<<1); //关闭扫描模式
    }
    
    //配置采样时间 
    ADC1->SMPR1 = 0x00FFFFFF;   //通道0-17: 采样时间为239.5个周期
    ADC1->SMPR2 = 0x3FFFFFFF;   
    
    //配置通道
    uint8_t uCurSequence = 0;
    
    ADC1->SQR1 = 0;
    ADC1->SQR2 = 0;
    ADC1->SQR3 = 0;
    
    for (int i = 0; i < HW_ADC_CH_COUNT; i++)
    {
        if (ulChMask & (0x1<<i))
        {
            //设置当前的采样顺序
            uCurSequence++;
            HW_ADC_SetConvertSequence(i, uCurSequence);
            
            //假如非扫描模式,则退出
            if (!bScanModeFlag)
            {
                break;
            }
        }
    }
    
    //配置通道数量
    ADC1->SQR1 &= ~(0xF<<20);
    ADC1->SQR1 |=  ((uCurSequence-1)<<20);  //配置通道数量
    
#if 0
    //开核级中断 
    NVIC_SetPriorityGrouping(7-2); 
    NVIC_SetPriority(ADC1_2_IRQn, NVIC_EncodePriority(7-2, 2, 2));
    NVIC_EnableIRQ(ADC1_2_IRQn);
#endif
    
    //唤醒ADC 
    ADC1->CR2 |=  (0x1<<0);         //0-->1  唤醒ADC1
    HW_SimpleDelayUs(10);           //至少1us的稳定时间
    
    //初始化/校准 
    ADC1->CR2 |=  (0x1<<3);
    while (ADC1->CR2 & (1<<3));     //复位校准
    
    ADC1->CR2 |=  (0x1<<2);
    while (ADC1->CR2 & (1<<2));     //AD校准
    
    //启动ADC1
    ADC1->CR2 |=  (0x1<<0);         //1-->1  启动ADC1
    
}


/*****************************************************************************
 * ADC相关控制接口
 ****************************************************************************/

/**
  * @brief  ADC模式初始化
  * @param  uAdcNode ADC节点
  * @param  ulChMask 通道掩码
  * @param  bScanModeFlag 扫描模式
  * @retval 0-成功 非0-失败
  * @note   非扫描模式下,只能使能一个通道,若入参多个通道,则默认只能最小的通道
  */
uint32_t HW_ADC_Init(uint8_t uAdcNode, uint32_t ulChMask, bool bScanModeFlag)
{
    if (ulChMask == 0)
    {
        return 1;
    }
    
    //初始化IO
    HW_ADC_PortConfig(uAdcNode, ulChMask);
    
    //初始化ADC
    HW_ADC_ConfigMode(uAdcNode, ulChMask, bScanModeFlag);
    
    return 0;
}


/**
  * @brief  ADC采集数据获取(非阻塞)
  * @param  uAdcNode ADC节点
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueNoBlock(uint8_t uAdcNode, uint16_t *pAdcValue)
{
    //假如转换完成,则将数据出参,并返回0,否则返回1
    if (ADC1->SR & (0x1<<1))    //转换完成
    {
        ADC1->SR &= ~(0x1<<1);  //清标志位
        
        *pAdcValue = ADC1->DR & 0xFFF;
        
        return 0;
    }
    
    return 1;
}


/** 
  * @brief  ADC采集数据获取(阻塞)
  * @param  uAdcNode ADC节点
  * @param  pAdcValue 采样值(出参)
  * @retval 0-成功 非0-失败
  */
uint32_t HW_ADC_RecvValueBlock(uint8_t uAdcNode, uint16_t *pAdcValue)
{
    //等待转换完成
    while (!(ADC1->SR & (0x1<<1)));
    
    //清标志位
    ADC1->SR &= ~(0x1<<1); 
    
    //读取采样结果
    *pAdcValue = ADC1->DR & 0xFFF;
    
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
    ADC1->CR2 |= (0x1<<22);  //软件触发
    
}
