/**
  ******************************************************************************
  * @file    HW_Eeprom.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.09.21
  * @brief   eeprom Drivers
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "HW_Eeprom.h"
#include "chip.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define EEPROM_WRITE_CMD_DELAY          (20000)    //指令延时时间
#define EEPROM_PAGE_WORD_COUNT          (32)


/*****************************************************************************
 * DEMO相关控制接口
 ****************************************************************************/


/**
  * @brief  EEPROM初始化
  * @param  None
  * @retval None
  */
void HW_EEPROM_Init(void)
{
    Chip_EEPROM_Init(LPC_EEPROM);
    
}


/**
  * @brief  EEPROM字写入
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @param  ulValue  要写入的字
  * @retval None
  */
void HW_EEPROM_WriteWord(uint32_t ulPage, uint32_t ulOffice, uint32_t ulValue)
{
    //写入数据
    *(uint32_t *)EEPROM_ADDRESS(ulPage, ulOffice * 4) = ulValue;
    Chip_EEPROM_SetCmd(LPC_EEPROM, EEPROM_CMD_ERASE_PRG_PAGE);
    
    //短暂延时
    for (volatile int i = 0; i < EEPROM_WRITE_CMD_DELAY; i++);
    
}


/**
  * @brief  EEPROM多字写入
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @param  pWordBuff  要写入的字缓冲区
  * @param  ulSize   要写入的字数
  * @retval None
  */
void HW_EEPROM_WriteMultiWord(uint32_t ulPage, uint32_t ulOffice, uint32_t *pWordBuff, uint32_t ulSize)
{
    for (int i = 0 ; i < ulSize; i++)
    {
        HW_EEPROM_WriteWord(ulPage, ulOffice, pWordBuff[i]);
    }
    
}


/**
  * @brief  EEPROM字读取
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @retval 读取到的字
  */
uint32_t HW_EEPROM_ReadWord(uint32_t ulPage, uint32_t ulOffice)
{
    
    return *(uint32_t *)EEPROM_ADDRESS(ulPage, ulOffice * 4);
}


/**
  * @brief  EEPROM多字读取
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @param  pWordBuff  字缓冲区
  * @param  ulSize   要读取的字数
  * @retval None
  */
void HW_EEPROM_ReadMultiWord(uint32_t ulPage, uint32_t ulOffice, uint32_t *pWordBuff, uint32_t ulSize)
{
    for (int i = 0 ; i < ulSize; i++)
    {
        pWordBuff[i] = HW_EEPROM_ReadWord(ulPage, ulOffice + i);
        
    }
    
}
