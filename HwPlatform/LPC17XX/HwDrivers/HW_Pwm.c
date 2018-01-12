/**
  ******************************************************************************
  * @file    HW_Pwm.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   pwm drivers
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Pwm.h"
#include "chip.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define PWM_GET_PR_BY_CNT_FRE(CntFre)            (Chip_PWM_GetClock(PWM[uPwmNode])/(CntFre) - 1)    //通过计数频率计算预分频值
#define PWM_GET_PR_BY_OP_FRE(OutFre, AutoLoad)   (PWM_GET_PR_BY_CNT_FRE((OutFre) * (AutoLoad)))     //通过输出频率计算预分频值(计数频率=输出频率*自动重装载值)

//PWM数量定义
#define HW_PWM_COUNT                (1)             //PWM数量

#define PWM_CH_COUNT                (6)             //通道计数
#define PWM_CH_MASK(n)              ((n)&0x3F)      //通道掩码
#define PWM_DEFAULT_COUNT_RANG      (100)           //默认定时器计数范围

static LPC_PWM_T * const PWM[HW_PWM_COUNT] = {LPC_PWM1};


//PWM1 IO 引脚定义
static const PINMUX_GRP_T pinmuxing[] = 
{
    /* PWM1 */
    {0x2, 0,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH1
    {0x2, 1,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH2
    {0x2, 2,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH3
    {0x2, 3,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH4
    {0x2, 4,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH5
    {0x2, 5,  IOCON_MODE_INACT | IOCON_FUNC1},    //PWM_CH6

};


/**
  * @brief  PWM IO 配置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
static void HW_PWM_IOConfig(uint8_t uPwmNode, uint8_t uChannelMask)
{
    for (int i = 0; i < PWM_CH_COUNT; i++)
    {
        if (PWM_CH_MASK(uChannelMask) & (0x1<<i))
        {
            Chip_IOCON_PinMuxSet(LPC_IOCON, pinmuxing[i].pingrp, pinmuxing[i].pinnum, pinmuxing[i].modefunc);
        }
    }
    
}


/**
  * @brief  PWM模式配置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
static void HW_PWM_ModeConfig(uint8_t uPwmNode, uint8_t uChannelMask)
{
    //初始化模块
    Chip_PWM_Init(PWM[uPwmNode]);
    
    //复位计数器
    PWM[uPwmNode]->TCR = (0x1<1);
    
    //设置PWM模式(计时)
    PWM[uPwmNode]->CTCR &= ~(0x3<<0);
    
    //开影子
    PWM[uPwmNode]->LER = (0x7F<<0);

    //设置PWM触发操作(复位计数)
    PWM[uPwmNode]->MCR |= PWM_MCR_RESET_ON_MATCH(0);
    
    //设置PWM时序
    PWM[uPwmNode]->MR0 = PWM_DEFAULT_COUNT_RANG;        //计数范围
    PWM[uPwmNode]->MR1 = PWM_DEFAULT_COUNT_RANG / 2;    //触发值(触发值/计数范围 == 占空比)
    PWM[uPwmNode]->MR2 = PWM_DEFAULT_COUNT_RANG / 2;
    PWM[uPwmNode]->MR3 = PWM_DEFAULT_COUNT_RANG / 2;
    PWM[uPwmNode]->MR4 = PWM_DEFAULT_COUNT_RANG / 2;
    PWM[uPwmNode]->MR5 = PWM_DEFAULT_COUNT_RANG / 2;
    PWM[uPwmNode]->MR6 = PWM_DEFAULT_COUNT_RANG / 2;
    //PWM[uPwmNode]->PR = PWM_GET_PR_BY_OP_FRE(1000, PWM[uPwmNode]->MR0); //设置PWM分频系数
    HW_PWM_SetOutputPwmFrq(uPwmNode, 1000);
    
}



/**
  * @brief  定时器外设时钟获取
  * @param  uPwmNode 定时器节点
  * @retval 定时器时钟
  */
static uint32_t HW_PWM_GetPeripheralClock(uint8_t uPwmNode)
{
    uint32_t ulTimeClock = 0;
    
    switch (uPwmNode)
    {
    case HW_PWM_NODE_0: ulTimeClock = Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_PWM1); break;
    default: break; //不应该到这里
    }
    
    return ulTimeClock;
}


/*****************************************************************************
 * PWM相关控制接口
 ****************************************************************************/

/**
  * @brief  PWM 初始化
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @retval None
  */
void HW_PWM_Init(uint8_t uPwmNode, uint8_t uChannelMask)
{
    //模式配置
    HW_PWM_ModeConfig(uPwmNode, uChannelMask);
    
    //IO配置
    HW_PWM_IOConfig(uPwmNode, uChannelMask);
    
    //使能PWM通道
    HW_PWM_EnableChannel(uPwmNode, uChannelMask, true);
    
}


/**
  * @brief  PWM 通道使能设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码字
  * @param  bIsEnble true-使能 flase-关闭
  * @retval None
  * @note   选择的通道,必须是已初始化的通道
  */
void HW_PWM_EnableChannel(uint8_t uPwmNode, uint8_t uChannelMask, bool bIsEnble)
{
    if (bIsEnble)
    {
        //使能PWM通道
        PWM[uPwmNode]->PCR |=  (PWM_CH_MASK(uChannelMask)<<9);
    }
    else 
    {
        //使能PWM通道
        PWM[uPwmNode]->PCR &= ~(PWM_CH_MASK(uChannelMask)<<9);
    }
}


/**
  * @brief  PWM输出PWM占空比设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask PWM通道掩码,位值为1时开启对应的通道
  * @param  fDutyRatio PWM波的占空比,范围为[0,100]
  * @retval None
  */
void HW_PWM_SetOutputPwmDutyRatio(uint8_t uPwmNode, uint8_t uChannelMask, float fDutyRatio)
{
    uint16_t nRegValue = (uint16_t)(fDutyRatio * (PWM[uPwmNode]->MR0) / 100.0);
    
    PWM[uPwmNode]->MR1 = (uChannelMask & 0X01) ? nRegValue : PWM[uPwmNode]->MR1;
    PWM[uPwmNode]->MR2 = (uChannelMask & 0X02) ? nRegValue : PWM[uPwmNode]->MR2;
    PWM[uPwmNode]->MR3 = (uChannelMask & 0X04) ? nRegValue : PWM[uPwmNode]->MR3;
    PWM[uPwmNode]->MR4 = (uChannelMask & 0X08) ? nRegValue : PWM[uPwmNode]->MR4;
    PWM[uPwmNode]->MR5 = (uChannelMask & 0X10) ? nRegValue : PWM[uPwmNode]->MR5;
    PWM[uPwmNode]->MR6 = (uChannelMask & 0X20) ? nRegValue : PWM[uPwmNode]->MR6;
    
    //设置更新
    PWM[uPwmNode]->LER = uChannelMask<<1;
    
}


/**
  * @brief  PWM输出比较值设置
  * @param  uPwmNode PWM节点
  * @param  uChannelMask 通道掩码
  * @param  nCompareVal 比较值
  * @retval None
  */
void HW_PWM_SetOutputCmpVal(uint8_t uPwmNode, uint8_t uChannelMask, uint16_t nCompareVal)
{
    PWM[uPwmNode]->MR1 = (uChannelMask & 0X01) ? nCompareVal : PWM[uPwmNode]->MR1;
    PWM[uPwmNode]->MR2 = (uChannelMask & 0X02) ? nCompareVal : PWM[uPwmNode]->MR2;
    PWM[uPwmNode]->MR3 = (uChannelMask & 0X04) ? nCompareVal : PWM[uPwmNode]->MR3;
    PWM[uPwmNode]->MR4 = (uChannelMask & 0X08) ? nCompareVal : PWM[uPwmNode]->MR4;
    PWM[uPwmNode]->MR5 = (uChannelMask & 0X10) ? nCompareVal : PWM[uPwmNode]->MR5;
    PWM[uPwmNode]->MR6 = (uChannelMask & 0X20) ? nCompareVal : PWM[uPwmNode]->MR6;
    
    //设置更新
    PWM[uPwmNode]->LER = uChannelMask<<1;
    
}


/**
  * @brief  PWM输出PWM频率设置
  * @param  uPwmNode PWM节点
  * @param  ulFrequency  PWM波的频率
  * @retval None
  */
void HW_PWM_SetOutputPwmFrq(uint8_t uPwmNode, uint32_t ulFrequency)
{
#if 0
    PWM[uPwmNode]->PR  = PWM_GET_PR_BY_OP_FRE(ulFrequency, PWM[uPwmNode]->MR0);  //频率配置
#else 
    uint32_t ulPwmClock = HW_PWM_GetPeripheralClock(uPwmNode);
    uint32_t ulPrescale = ((ulPwmClock / (ulFrequency*100)))-1;
    PWM[uPwmNode]->PR = ulPrescale;
#endif
    
}


/**
  * @brief  PWM输出自动重加载值设置
  * @param  nReloadVal 重加载值
  * @retval None
  */
void HW_PWM_SetOutputAutoReloadValue(uint8_t uPwmNode, uint16_t nReloadVal)
{
    PWM[uPwmNode]->MR0 = nReloadVal;
    
}


/**
  * @brief  PWM输出自动重加载值获取
  * @param  uPwmNode PWM节点
  * @retval PWM中的重加载值
  */
uint16_t HW_PWM_GetOutputAutoReloadValue(uint8_t uPwmNode)
{
    
    return PWM[uPwmNode]->MR0;
}


/**
  * @brief  PWM输出使能
  * @param  uPwmNode PWM节点
  * @param  bIsEnablle PWM使能位
  * @retval None
  */
void HW_PWM_OutputEnable(uint8_t uPwmNode, bool bIsEnablle)
{
    if (bIsEnablle)
    {
        PWM[uPwmNode]->TCR = (0x1<<0) | (0x1<<3);  //开启PWM  
    }
    else 
    {
        PWM[uPwmNode]->TCR = (0x1<<1);  //关闭PWM
    }
    
}
