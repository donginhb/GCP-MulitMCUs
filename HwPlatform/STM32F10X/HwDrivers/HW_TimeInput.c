/**
  ******************************************************************************
  * @file    HW_TimeInput.c
  * @author  Duhanfeng
  * @version V4.2 寄存器版本
  * @date    2017.11.20
  * @brief   TIM input drivers
  ******************************************************************************
  * @attention
  *
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
  *      PWM输入捕获,每捕获到一个脉冲,读取TIMx->CCR1可以得到脉冲的周期时间,
  *      TIM[uTimeNode]->CCR2可以得到脉冲的高电平时间
  * 参数,
  *      时钟:   72MHz
  * 
  * 有效电平: 默认高电平为有效电平  TIM[uTimeNode]->CCER 中配置
  *
  *   
  * V3.0------------    
  * 修改描述: 开放定时器配置接口,传定时器的编号进来即可对此定时器配置成输入
  * 修改作者: Duhanfeng
  * 当前版本: V3.0
  * 修改日期: 2016.07.15
  * 
  * V4.0------------    
  * 修改描述: 规范相关接口,兼容常规的输入捕获模式以及特殊的PWM输入模式
  * PWM输入:  该模式是输入捕获模式的一个特例,其特性是两个ICx被映射到同一个TIx输
  *           入....
  * 编码接口: ....
  * 修改作者: Duhanfeng
  * 当前版本: V4.0
  * 修改日期: 2017.02.14
  *  
  * V4.1------------
  * 修改描述: 1.修改其配置机制,将其从指针数组的索引改成直接的外设指针访问
  * 修改作者: Duhanfeng
  * 当前版本: V4.1
  * 修改日期: 2017.05.23
  * 
  * V4.2------------
  * 修改描述: 规范接口,作为GCP(General control platform)的硬件抽象层使用
  * 修改作者: Duhanfeng
  * 当前版本: V4.2
  * 修改日期: 2017.11.20
  * 
  ******************************************************************************
  */



/***********************************<INCLUDES>**********************************/  
#include "HW_TimeInput.h"
#include "HW_TimeComm.h"
#include "HW_SysCtrl.h"
#include "chip.h"



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//定时器输入捕获中断向量定义
static const IRQn_Type TIM_IRQn[HW_TIM_NODE_NUM] = {TIM1_CC_IRQn, TIM8_CC_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn};


/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式(普通/PWM输入/编码器模式)
  * @retval None
  */
static void HW_TIM_InputConfig(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uInputMode)
{
    //开时钟
    HW_TIM_ClockEnable(uTimeNode);
    
    //模式配置
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CKD;  //时钟分频因子
    TIM[uTimeNode]->CR1 |=  TIM_CR1_ARPE; //开启影子
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_CMS;  //边沿对齐模式
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_DIR;  //计数器向上计数
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_OPM;  //非单脉冲模式
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_URS;  //配置更新源--允许软件触发更新
    TIM[uTimeNode]->CR1 &= ~TIM_CR1_UDIS; //允许更新事件
    
    //配置时序参数
    TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE(1000000);  //预分频器:分频,计数频率为1MHz
    TIM[uTimeNode]->ARR = TIM_ARR_ARR;     //最大自动重装载值
    
    //选择工作时钟
    TIM[uTimeNode]->SMCR &= ~TIM_SMCR_SMS;//关闭从模式,选择内部时钟
    
    //配置捕获模式
    if (uInputMode == TIM_INPUT_MODE_NORMAL)
    {
        //uChannelMask 1
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC1F;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC1S;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_CC1S_0; //捕获TI1
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC1P;    //捕获上升沿
        TIM[uTimeNode]->CCER  |=  (uChannelMask & 0x01) ? TIM_CCER_CC1E : 0x00;
        
        //uChannelMask 2
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC2F;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC2S;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_CC2S_0; //捕获TI2
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC2P;    //捕获上升沿
        TIM[uTimeNode]->CCER  |=  (uChannelMask & 0x02) ? TIM_CCER_CC2E : 0x00;
        
        //uChannelMask 3
        TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_IC3F;
        TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_IC3F_0 | TIM_CCMR2_IC3F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_CC3S;
        TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_CC3S_0; //捕获TI3
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC3P;    //捕获上升沿
        TIM[uTimeNode]->CCER  |=  (uChannelMask & 0x04) ? TIM_CCER_CC3E : 0x00;
        
        //uChannelMask 4
        TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_IC4F;
        TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_IC4F_0 | TIM_CCMR2_IC4F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR2 &= ~TIM_CCMR2_CC4S;
        TIM[uTimeNode]->CCMR2 |=  TIM_CCMR2_CC4S_0; //捕获TI4
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC4P;    //捕获上升沿
        TIM[uTimeNode]->CCER  |=  (uChannelMask & 0x08) ? TIM_CCER_CC4E : 0x00;
        
    }
    //PWM输入模式
    else if ((uInputMode == TIM_INPUT_MODE_PWM1) || (uInputMode == TIM_INPUT_MODE_PWM2))
    {
        //uChannelMask 1
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC1F;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; //以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC1S;
        TIM[uTimeNode]->CCMR1 |=  ((uInputMode == TIM_INPUT_MODE_PWM1) ? TIM_CCMR1_CC1S_0 : TIM_CCMR1_CC1S_1); //捕获TI1/TI2
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC1P; //捕获上升沿
        
        //uChannelMask 2
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC2F;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1;//以Fck_int频率采样,8次相同的捕获结果来确认电平
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC2S;
        TIM[uTimeNode]->CCMR1 |=  ((uInputMode == TIM_INPUT_MODE_PWM1) ? TIM_CCMR1_CC2S_1 : TIM_CCMR1_CC2S_0); //捕获TI1/TI2
        TIM[uTimeNode]->CCER  |=  TIM_CCER_CC2P; //捕获下降沿
        
        //Save mode config
        TIM[uTimeNode]->SMCR  &= ~TIM_SMCR_TS;
        TIM[uTimeNode]->SMCR  |=  TIM_SMCR_TS_0 | TIM_SMCR_TS_2; //TI1触发
        TIM[uTimeNode]->SMCR  &= ~TIM_SMCR_SMS;
        TIM[uTimeNode]->SMCR  |=  TIM_SMCR_SMS_2; //从模式:复位模式
        
        TIM[uTimeNode]->CCER  |=  TIM_CCER_CC1E | TIM_CCER_CC2E;  //开启输入通道1,2
    }
    //编码器模式
    else if (uInputMode == TIM_INPUT_MODE_ENCODE)
    {
        //配置编码器模式
        TIM[uTimeNode]->CR2 &= ~TIM_CR2_TI1S;   //CH1脚连到TI1输入
        TIM[uTimeNode]->CR2 &= ~TIM_CR2_MMS;
        TIM[uTimeNode]->CR2 |=  TIM_CR2_MMS_0;  //主模式:使能
        
        TIM[uTimeNode]->SMCR &= ~TIM_SMCR_ETF;
        TIM[uTimeNode]->SMCR |=  TIM_SMCR_ETF_1;//触发滤波,每记录4个事件就输出一个跳变
        
        TIM[uTimeNode]->SMCR &= ~TIM_SMCR_SMS;
        TIM[uTimeNode]->SMCR |=  TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; //从模式: 编码器模式3 
        
        //输入端口捕获配置
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC1F;   //输入捕获1无滤波器
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC1PSC; //输入捕获1无预分频
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC1S;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_CC1S_0; //输入,IC1映射到TI1上
        
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC2F;   //输入捕获2无滤波器
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_IC2PSC;
        TIM[uTimeNode]->CCMR1 &= ~TIM_CCMR1_CC2S;
        TIM[uTimeNode]->CCMR1 |=  TIM_CCMR1_CC2S_0; //输入,IC2映射到TI2上
        
        //捕获端口使能
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC2P;    //输入不反向
        TIM[uTimeNode]->CCER  |=  TIM_CCER_CC2E;    //CH2使能
        TIM[uTimeNode]->CCER  &= ~TIM_CCER_CC1P;    //输入不反向
        TIM[uTimeNode]->CCER  |=  TIM_CCER_CC1E;    //CH1使能
        
        TIM[uTimeNode]->PSC = TIM_GET_PSC_BY_CNT_FRE(TIMx_FCLK);       //预分频器:不分频
        
    }
    
    //开启定时器
    TIM[uTimeNode]->CR1 |=  TIM_CR1_CEN;
    
}


/*****************************************************************************
 * 定时器输入捕获相关控制接口
 ****************************************************************************/

/**
  * @brief  定时器输入模式初始化
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  uInputMode 输入模式(普通/PWM输入/编码器模式)
  * @retval None
  */
void HW_TIM_InputInit(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uInputMode)
{
    //引脚配置
    HW_TIM_PortConfig(uTimeNode, uChannelMask, HW_TIME_PORT_INPUT);

    //模式配置
    HW_TIM_InputConfig(uTimeNode, uChannelMask, uInputMode);
    
}


/**
  * @brief  定时器输入中断使能
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnable 使能位
  * @retval None
  */
void HW_TIM_EnableInputIRQ(uint8_t uTimeNode, uint8_t uChannelMask, bool bIsEnable)
{
    if (bIsEnable)
    {
        TIM[uTimeNode]->DIER |= (uChannelMask<<1); //开外设中断
        HW_NVIC_Enable(TIM_IRQn[uTimeNode], 2, 2);//开内核中断
    }
    else 
    {
        HW_NVIC_DisableIRQ(TIM_IRQn[uTimeNode]);
    }
    
}


/**
  * @brief  定时器计数值获取
  * @param  uTimeNode 定时器节点
  * @retval 定时器计数寄存器值
  */
uint16_t HW_TIM_GetInputCount(uint8_t uTimeNode)
{
    
    return TIM[uTimeNode]->CNT;
}


/**
  * @brief  定时器输入捕获值获取
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码字
  * @retval 定时器通道捕获寄存器值
  */
uint16_t HW_TIM_GetInputCapValue(uint8_t uTimeNode, uint8_t uChannelMask)
{
    uint16_t nCapValue = 0;
    
    switch (uChannelMask)
    {
    case 1: nCapValue = TIM[uTimeNode]->CCR1; break;
    case 2: nCapValue = TIM[uTimeNode]->CCR2; break;
    case 4: nCapValue = TIM[uTimeNode]->CCR3; break;
    case 8: nCapValue = TIM[uTimeNode]->CCR4; break;
    default: break; //不应该到这里
    }
    
    return nCapValue;
}
