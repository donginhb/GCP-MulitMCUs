#ifndef  __HW_TIME_COMM_H
#define  __HW_TIME_COMM_H

#include "chip.h"

//定义定时器节点
typedef enum
{
    HW_TIM_0 = 0,  //TIM1 高级定时器
    HW_TIM_1,      //TIM8 高级定时器
    HW_TIM_2,      //TIM2 通用定时器
    HW_TIM_3,      //TIM3 通用定时器
    HW_TIM_4,      //TIM4 通用定时器(F103小容量的资源到此)
    HW_TIM_5,      //TIM5 通用定时器
    HW_TIM_6,      //TIM6 基本定时器
    HW_TIM_7,      //TIM7 基本定时器(F103的资源到此)
    HW_TIM_8,      //TIM9 通用定时器
    HW_TIM_9,      //TIM10 通用定时器
    HW_TIM_10,     //TIM11 通用定时器
    HW_TIM_11,     //TIM12 通用定时器
    HW_TIM_12,     //TIM13 通用定时器
    HW_TIM_13,     //TIM14 通用定时器
    HW_TIM_14,     //TIM15 通用定时器
    HW_TIM_15,     //TIM16 通用定时器
    HW_TIM_16,     //TIM17 通用定时器
    HW_TIM_NODE_NUM,
    
}HW_TIME_NODE;


typedef enum
{
    HW_TIME_PORT_OUTPUT = 0,
    HW_TIME_PORT_INPUT,
    
}HW_TIME_PORT_DIR;




#ifndef TIMx_FCLK
#define TIMx_FCLK      SystemCoreClock      //低速外设时钟总线2倍频(最高72M)
#endif

//定时器分频系数计算宏
#define TIM_GET_PSC_BY_CNT_FRE(CntFre)            (TIMx_FCLK/(CntFre) - 1)   //通过计数频率计算预分频值
#define TIM_GET_PSC_BY_OP_FRE(OutFre, AutoLoad)   (TIM_GET_PSC_BY_CNT_FRE((OutFre) * (AutoLoad))) //通过输出频率计算预分频值(计数频率=输出频率*自动重装载值)


//定时器节点定义
extern TIM_TypeDef * const TIM[HW_TIM_NODE_NUM];


#ifdef __cplusplus
extern "C"
{
#endif


/**
  * @brief  定时器时钟开启
  * @param  uTimeNode 定时器节点
  * @retval None
  */
void HW_TIM_ClockEnable(uint8_t uTimeNode);


/**
  * @brief  定时器端口配置
  * @param  uTimeNode 定时器节点
  * @param  uChannelMask 通道掩码
  * @param  uPortDir 端口方向(0-输出,1-输入)
  * @retval None
  */
void HW_TIM_PortConfig(uint8_t uTimeNode, uint8_t uChannelMask, uint8_t uPortDir);
    

#ifdef __cplusplus
}
#endif

#endif /* __HW_TIME_COMM_H */
