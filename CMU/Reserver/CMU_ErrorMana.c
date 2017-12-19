/**
  ******************************************************************************
  * @file    CMU_ErrorMana.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.15
  * @brief   Cmu Error manage 
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_ErrorMana.h"
#include "DataType/DataType.h"
#include "CMU_DataStructDef.h"
      
#include <string.h>


/*****************************************************************************
 * CMU 错误管理相关控制接口
 ****************************************************************************/

/**
  * @brief  错误码生成
  * @param  ulCodeNo 错误编码
  * @param  ulID 通信ID
  * @retval 错误码
  */
uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID)
{
    ERROR_CODE sError = {0};
    COM_DATA_ID sID = {0};

    sID.ulFrameID = ulID;
    sError.ulCodeNO = ulCodeNo;
    sError.ulCrdNO = sID.ulComDataID.ulCrdNo;
    sError.ulDeviceNO = sID.ulComDataID.ulDevNo;
    sError.ulModuleNO = SYS_SOFT_MODULE_COM_WITH_UPCPU;

    return *((uBit32*)&sError);
}