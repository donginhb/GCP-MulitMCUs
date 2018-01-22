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

GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};



typedef struct
{
    unsigned char OpSpeed    : 2; //[1:0]
    unsigned char PullUp     : 2; //[3:2]
    unsigned char OpType     : 2; //[5:4]
    unsigned char Moder      : 2; //[7:6]
}GPIO_REG_CODE;



/**
  * @brief  通用IO初始化函数
  * @param  nPort PA~PG,要初始化的中断端口
  * @param  nPin  0~15, 引脚
  * @param  Mode IO输入/输出模式
  * @retval None
  */
void HW_GPIO_ModeConfig(uint16_t nPort, uint16_t nPin, GPIO_MODE Mode)
{
    GPIO_TypeDef * const GPIOx[7] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};
    GPIO_REG_CODE  *GpioRegCode = (GPIO_REG_CODE *)&Mode;

    // 开时钟
    RCC->AHBENR |= (0x1<<(17+nPort));

    // 配置模式
    GPIOx[nPort]->MODER   &= ~(0x3<<nPin*2);
    GPIOx[nPort]->MODER   |=  (GpioRegCode->Moder<<nPin*2);

    // 配置输出类型
    GPIOx[nPort]->OTYPER  &= ~(0x1<<nPin*1);
    GPIOx[nPort]->OTYPER  |=  (GpioRegCode->OpType<<nPin*1);

    // 配置上下拉
    GPIOx[nPort]->PUPDR   &= ~(0x3<<nPin*2);
    GPIOx[nPort]->PUPDR   |=  (GpioRegCode->PullUp<<nPin*2);

    // 配置输出速度
    GPIOx[nPort]->OSPEEDR &= ~(0x3<<nPin*2);
    GPIOx[nPort]->OSPEEDR |=  (GpioRegCode->OpSpeed<<nPin*2);
  
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
    HW_GPIO_ModeConfig(nPort, nPin, GPIO_GP_OD_H);
    
}


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin)
{
    HW_GPIO_ModeConfig(nPort, nPin, GPIO_FLOAT);
    
}


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin)
{
    HW_GPIO_ModeConfig(nPort, nPin, GPIO_GP_PP_H);
    
}

