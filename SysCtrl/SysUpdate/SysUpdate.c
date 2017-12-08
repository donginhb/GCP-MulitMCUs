/**
  ******************************************************************************
  * @file    SysUpdate.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.07
  * @brief   System update mamage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SysUpdate.h"
#include "DataType/DataType.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

typedef struct
{
    //Bootloader 相关
    uBit32 ulMainAppFlag;           //主APP存在标志
    uBit32 ulSubAppFlag;            //副APP存在标志
    uBit32 ulIgnoreSubAppFlag;      //忽略副APP标志
    
    uBit32 ulMainAppFlashLen;       //主APP FLASH长度
    uBit32 ulMainAppFlashCheckSum;  //主APP FALSH校验和
    uBit32 ulSubAppFlashLen;        //副APP FLASH长度
    uBit32 ulSubAppFlashCheckSum;   //副APP FALSH校验和
    
}SYS_UAER_BOOTLOADER_DATE;  //用户Bootloader数据


/*****************************************************************************
 * DEMO相关控制接口
 ****************************************************************************/

/**
  * @brief  Demo
  * @param  None
  * @retval None
  */
