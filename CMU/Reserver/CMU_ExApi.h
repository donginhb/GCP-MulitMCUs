#ifndef __CMU_EX_API_H
#define __CMU_EX_API_H

#include "DataType/DataType.h"
#include "Version/Version.h"

/*****************************************************************************
 * 外部函数列表定义
 ****************************************************************************/

typedef struct
{
    uBit32 (*pf_SYS_UpdateSLC)(uBit8* pUpdateData, uBit32 ulDataLen);
    uBit32 (*pf_SYS_UpdateIPO)(uBit8* pUpdateData, uBit32 ulDataLen);
    uBit32 (*pf_SYS_GetSLCVersion)(SOFTWARE_VERSION* pVersion);
    void   (*pf_SYS_Reset)(void);
    void   (*pf_SYS_WriteSLCProgID)(void);
    
}CMU_EXTERNAL_FUN_TEBLE;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 外部函数接口相关控制接口
 ****************************************************************************/

/**
  * @brief  外部函数地址获取
  * @param  None
  * @retval 外部函数表地址
  */
CMU_EXTERNAL_FUN_TEBLE *CMU_GetExternFunAddr(void);


/**
  * @brief  外部函数设置
  * @param  pCmuExternFunTable 外部函数指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE *pCmuExternFunTable);


/**
  * @brief  外部函数初始化
  * @param  None
  * @retval None
  * @note   应当在调用CMU_SetExternFun之后才调用此函数
  */
void CMU_InitExApi(void);


#ifdef __cplusplus
}
#endif


#endif

