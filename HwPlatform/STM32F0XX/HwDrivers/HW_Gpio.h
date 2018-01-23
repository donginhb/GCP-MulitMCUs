#ifndef  __HW_GPIO_H
#define  __HW_GPIO_H

#include "chip.h"


/* ---定义IO模式--- */
typedef enum
{
  // 输入
  HW_GPIO_FLOAT     = 0x00,   //输入浮空    
  HW_GPIO_PUSH_UP   = 0x04,   //输入上拉    
  HW_GPIO_PUSH_DOWM = 0x08,   //输入下拉    

  // 通用输出
  HW_GPIO_GP_PP_L   = 0x40,   //通用推挽低速
  HW_GPIO_GP_PP_M   = 0x41,   //通用推挽中速
  HW_GPIO_GP_PP_H   = 0x43,   //通用推挽高速

  #if 0
  HW_GPIO_GP_OD_L   = 0x50,   //通用开漏低速
  HW_GPIO_GP_OD_M   = 0x51,   //通用开漏中速
  HW_GPIO_GP_OD_H   = 0x53,   //通用开漏高速
  #else 
  HW_GPIO_GP_OD_L   = 0x54,   //通用开漏低速(内部上拉)
  HW_GPIO_GP_OD_M   = 0x55,   //通用开漏中速(内部上拉)
  HW_GPIO_GP_OD_H   = 0x57,   //通用开漏高速(内部上拉)
  #endif

  // 复用输出
  #if 1
  HW_GPIO_AF_PP_L   = 0x80,   //复用推挽低速
  HW_GPIO_AF_PP_M   = 0x81,   //复用推挽中速
  HW_GPIO_AF_PP_H   = 0x83,   //复用推挽高速
  #else 
  HW_GPIO_AF_PP_L   = 0x88,   //复用推挽低速(内部下拉)
  HW_GPIO_AF_PP_M   = 0x89,   //复用推挽中速(内部下拉)
  HW_GPIO_AF_PP_H   = 0x8B,   //复用推挽高速(内部下拉)
  #endif
     
  HW_GPIO_AF_OD_L   = 0x90,   //复用开漏低速
  HW_GPIO_AF_OD_M   = 0x91,   //复用开漏中速
  HW_GPIO_AF_OD_H   = 0x93,   //复用开漏高速
             
  // 模拟模式
  GPIO_ANALOG    = 0xC0,   //模拟模式    
  
}GPIO_MODE;


#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  通用IO初始化函数
  * @param  nPort PA~PG,要初始化的中断端口
  * @param  nPin  0~15, 引脚
  * @param  Mode IO输入/输出模式
  * @retval None
  */
void HW_GPIO_ModeConfig(uint16_t nPort, uint16_t nPin, GPIO_MODE Mode);


/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HW_GPIO_SetOutputState(uint16_t nPort, uint16_t nPin, bool bState);


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HW_GPIO_GetIOState(uint16_t nPort, uint16_t nPin);


/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOD(uint16_t nPort, uint16_t nPin);


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin);


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin);
      

/**
  * @brief  复用模式配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  uMode 复用模式
  * @retval None
  */
void HW_GPIO_SetAFMode(uint16_t nPort, uint16_t nPin, uint8_t uMode);
    
    
#ifdef __cplusplus
}
#endif

#endif /* __HW_GPIO_H */
