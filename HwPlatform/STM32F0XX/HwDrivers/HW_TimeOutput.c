/**
  ******************************************************************************
  * @file    tim_output.c
  * @author  Duhanfeng
  * @version V4.1 寄存器版本
  * @date    2017.03.06
  * @brief   TIMx PWM模式配置
  ******************************************************************************
  * @attention
  *
  * TIM1:   CH1 --- PA8  复用推挽,50M
  *         CH2 --- PA9  复用推挽,50M
  *         CH3 --- PA10 复用推挽,50M
  *         CH4 --- PA11 复用推挽,50M
  *
  * TIM2:   CH1 --- PA0  复用推挽,50M
  *         CH2 --- PA1  复用推挽,50M
  *         CH3 --- PA2  复用推挽,50M
  *         CH4 --- PA3  复用推挽,50M
  *
  * TIM3:   CH1 --- PA6  复用推挽,50M
  *         CH2 --- PA7  复用推挽,50M
  *         CH3 --- PB0  复用推挽,50M
  *         CH4 --- PB1  复用推挽,50M
  *
  * TIM4:   CH1 --- PB6  复用推挽,50M
  *         CH2 --- PB7  复用推挽,50M
  *         CH3 --- PB8  复用推挽,50M
  *         CH4 --- PB9  复用推挽,50M
  *
  * TIM5:   CH1 --- PA0  复用推挽,50M
  *         CH2 --- PA1  复用推挽,50M
  *         CH3 --- PA2  复用推挽,50M
  *         CH4 --- PA3  复用推挽,50M
  *
  * TIM8:   CH1 --- PC6  复用推挽,50M
  *         CH2 --- PC7  复用推挽,50M
  *         CH3 --- PC8  复用推挽,50M
  *         CH4 --- PC9  复用推挽,50M
  *
  * 配置,
  *      开启影子寄存器,向上计数,非单脉冲模式,允许更新,PWM模式
  * 参数,
  *      时钟:   72MHz
  * 
  * 有效电平: 默认高电平为有效电平  TIM[uTimeNode]->CCER 中配置
  *
  * 输出模式,
  *   输出比较: 每当CNT=CCR时,电平翻转.高低电平时间固定为一个周期
  *   PWM模式1: 在向上计数时，一旦CNT<CCRx时,通道为有效电平,否则为无效电平
  *   PWM模式2: 在向上计数时，一旦CNT<CCRx时,通道为无效电平,否则为有效电平
  *   当占空比一致时,PWM1和PWM2波形为互补波形
  *   
  * 注1: 定时器4个通道共用自动重装载和分频,即频率跟分辨率保持一致.但是占空比跟有效
  *      电平可以各自配置.
  *
  * 注2: PSC(预分频系数)=时钟/(频率*分辨率),右侧结果应为一个整数,否则实际的频率值
  *      会有误差(丢失小数部分).
  * 
  * 注3: 占空比=比较值/分辨率*100%; 如果要配置PWM波的占空比,可以通过调用函数
  *      TIMx_SetPwmDutyRatio()来实现.如果要针对于高分辨率高精度要求来进行配置,
  *      则可以根据分辨率通过接口函数修改其比较值来实现.
  * 
  * V2.0------------    
  * 修改描述: 1. 修改文件头描述,开放对自动重装载以及捕获/比较寄存器的接口函数.
  *           2. 将PWM模式与比较模式合并,开放输出模式来配置;
  *           3. 增加比较中断的接口,可以通过修改_TIMx_COMPARE_INT_的值来开关中断
  * 输出比较: 用于控制一个输出波形,或者是指示一段给定的时间已经到时了.
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.07.15
  *
  * V3.0------------    
  * 修改描述: 1. 修改函数机制,本文件可以用于TIM2~5的输出(比较/PWM),不同于tim_delay
  *              等文件需指定单个定时器并通过此定时器实现功能,本文件可以被多定时器
  *              同时调用,类似于usart.c与spi.c的实现机制.
  * 修改作者: 杜公子寒枫
  * 当前版本: V3.0
  * 修改日期: 2016.08.25
  * 
  * V4.0------------    
  * 修改描述: 1. 修改函数机制,兼容TIM1~8
  *           2. 定时器的调用统一规范,部分功能的实现在hw_platform文件中
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.0
  * 修改日期: 2017.03.06
  *
  * V4.1------------
  * 修改描述: 1.修改其配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: 杜公子寒枫
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  * 
  * V4.2------------
  * 修改描述: 规范接口,作为GCP(General control platform)的硬件抽象层使用
  * 修改作者: Duhanfeng
  * 当前版本: V4.2
  * 修改日期: 2017.11.20
  * 
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "HW_TimeOutput.h"
#include "HW_TimeComm.h"
#include "HW_SysCtrl.h"
#include "chip.h"



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//定时器输入捕获中断向量定义
static const IRQn_Type TIM_IRQn[HW_TIM_NODE_NUM] = {TIM1_CC_IRQn, TIM2_IRQn, TIM3_IRQn};


/**
  * @brief  定时器输出模式配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uOutputCfgCode 工作模式
  * @retval None
  */
static void HW_TIM_OutputConfig(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uOutputMode)
{
    uint8_t uOutputCfgCode = 0;     //输出配置码
    
    //开时钟
    HW_TIM_ClockEnable(uTimeNode);
    
    //获取输出配置码
    switch (uOutputMode)
    {
    case TIM_OUTPUT_MODE_COMPARE: uOutputCfgCode = 3; break;
    case TIM_OUTPUT_MODE_PWM1   : uOutputCfgCode = 6; break;
    case TIM_OUTPUT_MODE_PWM2   : uOutputCfgCode = 7; break;
    default: break; //不应该出现在这里
    }
    
    //配置工作模式
    TIM[uTimeNode]->CR1 |=  TIM_CR1_ARPE; //开启影子寄存器
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CMS;  //边沿对齐模式
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_DIR;  //向上计数
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_OPM;  //非单脉冲模式
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_URS;  //配置更新源
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_UDIS; //允许更新
    
    //选择工作时钟
    TIM[uTimeNode]->SMCR &= ~TIM_SMCR_SMS;   //关闭从模式--内部时钟72M
    
    //配置中断             
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UDE;  //禁止触发DMA请求
    TIM[uTimeNode]->DIER &= ~TIM_DIER_UIE;  //关闭更新中断
    TIM[uTimeNode]->EGR  |=  TIM_EGR_UG;    //开启更新事件
    
#if 0
    if (Def_IntFlag[Timer])  //根据标志位配置中断
    {
        TIM[uTimeNode]->DIER |= (uChannelMask<<1);  //开外设中断
        NVIC_Enable(TIMx_IRQn[Timer],2,2); //开内核中断
    }
#endif
    
    //配置CH1 PWM模式
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_OC1CE;  //OC1REF不受ETRF影响
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_OC1M;  
    TIM[uTimeNode]->CCMR1 |=  (uOutputCfgCode<<(4+0));    //输出模式
    TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_OC1PE;  //输出比较1预装载使能
    TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_OC1FE;  //输出比较1快速使能
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC1S;   //CH1配置为输出模式
    
    //配置CH2 PWM模式
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_OC2CE;  //OC2REF不受ETRF影响
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_OC2M;  
    TIM[uTimeNode]->CCMR1 |=  (uOutputCfgCode<<(4+8));    //输出模式
    TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_OC2PE;  //输出比较2预装载使能
    TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_OC2FE;  //输出比较2快速使能
    TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC2S;   //CH2配置为输出模式
    
    
    //配置CH3 PWM模式
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_OC3CE;  //OC3REF不受ETRF影响
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_OC3M;  
    TIM[uTimeNode]->CCMR2 |=  (uOutputCfgCode<<(4+0));    //输出模式
    TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_OC3PE;  //输出比较3预装载使能
    TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_OC3FE;  //输出比较3快速使能
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_CC3S;   //CH3配置为输出模式
    
    //配置CH4 PWM模式
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_OC4CE;  //OC4REF不受ETRF影响
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_OC4M;   
    TIM[uTimeNode]->CCMR2 |=  (uOutputCfgCode<<(4+8));    //输出模式
    TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_OC4PE;  //输出比较4预装载使能
    TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_OC4FE;  //输出比较4快速使能
    TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_CC4S;   //CH4配置为输出模式
    
    //配置有效电平
    TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC1P;    //CH1默认高电平为有效电平
    TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC2P;    //CH2默认高电平为有效电平
    TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC3P;    //CH3默认高电平为有效电平
    TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC4P;    //CH4默认高电平为有效电平
    
    //根据关键字配置输出
    TIM[uTimeNode]->CCER |= (uChannelMask & (0X1<<0)) ? TIM_CCER_CC1E : 0;
    TIM[uTimeNode]->CCER |= (uChannelMask & (0X1<<1)) ? TIM_CCER_CC2E : 0;
    TIM[uTimeNode]->CCER |= (uChannelMask & (0X1<<2)) ? TIM_CCER_CC3E : 0;
    TIM[uTimeNode]->CCER |= (uChannelMask & (0X1<<3)) ? TIM_CCER_CC4E : 0;
    
    //配置PWM参数
    TIM[uTimeNode]->PSC  = TIM_GET_PSC_BY_OP_FRE(1000, 1000); //频率配置
    TIM[uTimeNode]->ARR  = 1000;   //分辨率配置(默认)
    
    //使能输出
    if(uTimeNode == HW_TIM_1)//假如是高级定时器,还需配置BDTR寄存器
    {
        TIM[uTimeNode]->BDTR |= TIM_BDTR_MOE; //主输出使能
        TIM[uTimeNode]->BDTR |= TIM_BDTR_AOE; //自动输出使能
    }
    
    //开启定时器
//  TIM[uTimeNode]->CR1  |= TIM_CR1_CEN;
    
}


/*****************************************************************************
 * 定时器输出相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输出模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uOutputCfgCode 工作模式
  * @retval None
  */
void HW_TIM_InitOutput(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uOutputMode)
{
    //引脚配置
    HW_TIM_PortConfig(uTimeNode, uChannelMask, HW_TIME_PORT_OUTPUT);
    
    //模式配置
    HW_TIM_OutputConfig(uTimeNode, uChannelMask, uOutputMode);
    
    //时序配置
    HW_TIM_SetOutputPwmDutyRatio(uTimeNode, uChannelMask, 50);
    
    //开启定时器
    HW_TIM_OutputEnable(uTimeNode, 1);
    
}


/**
  * @brief  定时器输出中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableOutputIRQ(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t bIsEnable)
{
    if (bIsEnable)
    {
        TIM[uTimeNode]->DIER |= (uChannelMask<<1); //开外设中断
        HW_NVIC_Enable(TIM_IRQn[uTimeNode], 2);//开内核中断
    }
    else 
    {
        HW_NVIC_DisableIRQ(TIM_IRQn[uTimeNode]);
    }
    
}


/**
  * @brief  定时器输出PWM占空比设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 定时器通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_TIM_SetOutputPwmDutyRatio(uint8_t uTimeNode, uint8_t uChannelMask, float fDutyRatio)
{
    uint16_t nRegValue = (uint16_t)(fDutyRatio * (TIM[uTimeNode]->ARR) / 100.0);
    
    TIM[uTimeNode]->CCR1 = (uChannelMask & 0X01) ? nRegValue : TIM[uTimeNode]->CCR1;
    TIM[uTimeNode]->CCR2 = (uChannelMask & 0X02) ? nRegValue : TIM[uTimeNode]->CCR2;
    TIM[uTimeNode]->CCR3 = (uChannelMask & 0X04) ? nRegValue : TIM[uTimeNode]->CCR3;
    TIM[uTimeNode]->CCR4 = (uChannelMask & 0X08) ? nRegValue : TIM[uTimeNode]->CCR4;
    
}


/**
  * @brief  定时器输出比较值设置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @param  nCompareVal 比较值
  * @retval None
  */
void HW_TIM_SetOutputCmpVal(uint8_t uTimeNode, uint8_t uChannelMask, uint16_t nCompareVal)
{
    TIM[uTimeNode]->CCR1 = (uChannelMask & 0X01) ? nCompareVal : TIM[uTimeNode]->CCR1;
    TIM[uTimeNode]->CCR2 = (uChannelMask & 0X02) ? nCompareVal : TIM[uTimeNode]->CCR2;
    TIM[uTimeNode]->CCR3 = (uChannelMask & 0X04) ? nCompareVal : TIM[uTimeNode]->CCR3;
    TIM[uTimeNode]->CCR4 = (uChannelMask & 0X08) ? nCompareVal : TIM[uTimeNode]->CCR4;
    
}


/**
  * @brief  定时器输出PWM频率设置
  * @param  uTimeNode 定时器节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HW_TIM_SetOutputPwmFrq(uint8_t uTimeNode, uint32_t ulFrequency)
{
    TIM[uTimeNode]->PSC  = TIM_GET_PSC_BY_OP_FRE(ulFrequency, TIM[uTimeNode]->ARR);  //频率配置
    
}


/**
  * @brief  定时器输出自动重加载值设置
  * @param  nReloadVal 重加载值
  * @retval None
  */
void HW_TIM_SetOutputAutoReloadValue(uint8_t uTimeNode, uint16_t nReloadVal)
{
    TIM[uTimeNode]->ARR = nReloadVal;
    
}


/**
  * @brief  定时器输出自动重加载值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器中的重加载值
  */
uint16_t HW_TIM_GetOutputAutoReloadValue(uint8_t uTimeNode)
{
    
    return TIM[uTimeNode]->ARR;
}


/**
  * @brief  定时器输出使能
  * @param  uTimeNode 定时器节点
  * @param  bIsEnablle 定时器使能位
  * @retval None
  */
void HW_TIM_OutputEnable(uint8_t uTimeNode, bool bIsEnablle)
{
    if (bIsEnablle)
    {
        TIM[uTimeNode]->CR1 |=  TIM_CR1_CEN;  //开启定时器  
    }
    else 
    {
        TIM[uTimeNode]->CR1 &= ~TIM_CR1_CEN;  //关闭定时器
    }
    
}
