/**
  ******************************************************************************
  * @file    HW_Gpio.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.08.22
  * @brief   Flash drivers
  ******************************************************************************
  * @attention
  * 
  * 设置重试机制,如果FLASH执行失败,重试一定次数.
  * 具体数值由宏FLASH_OPERA_RETRY_NUM 所定义
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Gpio.h"
#include "chip.h"

static GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};


/**
  * @brief  通用IO初始化函数
  * @param  nPort PA~PG,要初始化的中断端口
  * @param  nPin  0~15, 引脚
  * @param  Mode IO输入/输出模式
  * @retval None
  */
void HW_GPIO_ModeConfig(uint16_t nPort, uint16_t nPin, HW_GPIO_MODE Mode)
{
    /* 开时钟 */
    RCC->APB2ENR |= (0x1<<(2+nPort));
    
    /* 配置工作模式 */
    if (nPin < 8)
    {
        GPIOx[nPort]->CRL &= ~(0XFU<<nPin*4);
        
        if (Mode == HW_GPIO_PUSH_UP)
        {
            GPIOx[nPort]->CRL |=  (HW_GPIO_PUSH_UP<<nPin*4);
            GPIOx[nPort]->ODR |=  (0x1U<<nPin);
        }
        else if (Mode == HW_GPIO_PUSH_DOWM)
        {
            GPIOx[nPort]->CRL |=  (HW_GPIO_PUSH_UP<<nPin*4);
            GPIOx[nPort]->ODR &= ~(0x1U<<nPin);
        }
        else 
        {
            GPIOx[nPort]->CRL |=  (Mode<<nPin*4);
        }
    }
    else
    {
        GPIOx[nPort]->CRH &= ~(0XFU<<(nPin-8)*4);
        
        if (Mode == HW_GPIO_PUSH_UP)
        {
            GPIOx[nPort]->CRH |=  (HW_GPIO_PUSH_UP<<(nPin-8)*4);
            GPIOx[nPort]->ODR |=  (0x1U<<nPin);
        }
        else if (Mode == HW_GPIO_PUSH_DOWM)
        {
            GPIOx[nPort]->CRH |=  (HW_GPIO_PUSH_UP<<(nPin-8)*4);
            GPIOx[nPort]->ODR &= ~(0x1U<<nPin);
        }
        else 
        {
            GPIOx[nPort]->CRH |=  (Mode<<(nPin-8)*4);
        }
    }
    
}



/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HW_GPIO_SetOutputState(uint16_t nPort, uint16_t nPin, bool bState)
{
    if (bState)
    {
        GPIOx[nPort]->BSRR |= (0x1<<nPin);
    }
    else 
    {
        GPIOx[nPort]->BRR |= (0x1<<nPin);
    }
    
}


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HW_GPIO_GetIOState(uint16_t nPort, uint16_t nPin)
{
    
    return (GPIOx[nPort]->IDR & (0x1<<nPin)) != 0;
}


/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOD(uint16_t nPort, uint16_t nPin)
{
    HW_GPIO_ModeConfig(nPort, nPin, HW_GPIO_GP_OD_H);
    
}


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin)
{
    HW_GPIO_ModeConfig(nPort, nPin, HW_GPIO_FLOAT);
    
}


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin)
{
    HW_GPIO_ModeConfig(nPort, nPin, HW_GPIO_GP_PP_H);
    
}

