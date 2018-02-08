/**
  ******************************************************************************
  * @file    HW_Dac.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.02.07
  * @brief   Hardware dac driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Dac.h"
#include "chip.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
//超时计数值
#define HW_DAC_TIMEOUT          (0xFFFF)    

//DAC参考电压
#define HW_DAC_VREF_VALUE       (3300)      //单位:毫伏

//DAC分辨率
#define HW_DAC_RESOLUTION       (1024) 


/*****************************************************************************
 * DAC相关控制接口
 ****************************************************************************/

/**
  * @brief  DAC初始化
  * @param  None
  * @retval None
  */
void HW_DAC_Init(void)
{
    //DAC引脚配置
    Chip_SCU_DAC_Analog_Config();

    //初始化DAC
    Chip_DAC_Init(LPC_DAC);
    
    //设置重装载值
    Chip_DAC_SetDMATimeOut(LPC_DAC, HW_DAC_TIMEOUT);
    
    //使能DCAR超时计数器和DMA
    Chip_DAC_ConfigDAConverterControl(LPC_DAC, (DAC_CNT_ENA | DAC_DMA_ENA));
    
}


/**
  * @brief  更新DAC输出(根据指定的数字量值)
  * @param  ulDigitalValue DAC数字量值
  * @retval None
  * @note   实际DAC输出模拟量电压 = (ulDigitalValue/硬件分辨率)*参考电压
  */
void HW_DAC_UpdateByDigitalValue(uint32_t ulDigitalValue)
{
   Chip_DAC_UpdateValue(LPC_DAC, ulDigitalValue); 
   while ( !(Chip_DAC_GetIntStatus(LPC_DAC)) ) ; //等待稳定
   
}


/**
  * @brief  更新DAC输出(根据指定的模拟量值)
  * @param  uDacValue DAC模拟电压值,单位: 毫伏
  * @retval None 
  * @note   注意电压范围最高为参考电压,超过该值均视为要输出参考电压
  */
void HW_DAC_UpdateByAnalogValue(uint32_t ulAnalogValue)
{
   uint16_t ulDigitalValue=0;
   ulDigitalValue = (ulAnalogValue*HW_DAC_RESOLUTION)/HW_DAC_VREF_VALUE;
   Chip_DAC_UpdateValue(LPC_DAC, ulDigitalValue); 
   while ( !(Chip_DAC_GetIntStatus(LPC_DAC)) ) ; //等待稳定
   
}
