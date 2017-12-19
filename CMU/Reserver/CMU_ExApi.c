/**
  ******************************************************************************
  * @file    CMU_CmdProcess.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.14
  * @brief   CMU Cmd Process
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_ExApi.h"
#include "CMU_DataStructDef.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//外部函数空指针入口(避免意外访问空指针导致程序挂掉)
static void voidFun_void(void){};
static uBit32 u32Fun_pvoid_u32(void* a, uBit32 b){return CMU_ERR_UNBAND;}
static uBit32 u32Fun_pvoid(void* a){return CMU_ERR_UNBAND;}

//外部函数列表
static CMU_EXTERNAL_FUN_TEBLE m_sExternalFunTable = {NULL};    


/*****************************************************************************
 * 外部函数接口相关控制接口
 ****************************************************************************/

/**
  * @brief  外部函数地址获取
  * @param  None
  * @retval 外部函数表地址
  */
CMU_EXTERNAL_FUN_TEBLE *CMU_GetExternFunAddr(void)
{
    
    return &m_sExternalFunTable;
}


/**
  * @brief  外部函数设置
  * @param  pCmuExternFunTable 外部函数指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE *pCmuExternFunTable)
{
    if (pCmuExternFunTable)
    {
        m_sExternalFunTable = *pCmuExternFunTable;
        
        return 0;
    }
    
    return 1;
}


/**
  * @brief  外部函数初始化
  * @param  None
  * @retval None
  * @note   应当在调用CMU_SetExternFun之后才调用此函数
  */
void CMU_InitExApi(void)
{
    if (!m_sExternalFunTable.pf_SYS_UpdateSLC)
        m_sExternalFunTable.pf_SYS_UpdateSLC            = (uBit32 (*)(uBit8*, uBit32))u32Fun_pvoid_u32;;

    if (!m_sExternalFunTable.pf_SYS_UpdateIPO)
        m_sExternalFunTable.pf_SYS_UpdateIPO            = (uBit32 (*)(uBit8*, uBit32))u32Fun_pvoid_u32;
 
    if (!m_sExternalFunTable.pf_SYS_GetSLCVersion)
        m_sExternalFunTable.pf_SYS_GetSLCVersion        = (uBit32 (*)(SOFTWARE_VERSION*))u32Fun_pvoid;

    if (!m_sExternalFunTable.pf_SYS_Reset)
        m_sExternalFunTable.pf_SYS_Reset                = voidFun_void          ;

    if (!m_sExternalFunTable.pf_SYS_WriteSLCProgID)
        m_sExternalFunTable.pf_SYS_WriteSLCProgID       = voidFun_void          ;
    
}

