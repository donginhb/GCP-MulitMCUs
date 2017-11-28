/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_Interface.h

摘    要：通道管理模块接口

运行环境：LPC43xx 

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年9月25日
*/

#include "CSM_Interface.h"
#include "..\\DataStructDef\\DataType.h"



CSM_INTERFACE m_CSMInterface;
unsigned short m_nCSMInterfaceInit = 0;


//成功返回0，否则返回非0
uBit32 CSM_InitInterface(CSM_INTERFACE* pCSMInterface)
{
    uBit32 ulReturn = 0;
    
      if (pCSMInterface == NULL)
    {
        ulReturn = 1;
    }
    
    //获取轴（物理轴）状态
    if (pCSMInterface->pf_PAX_GetStateData == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_GetRunningStatus == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_GetRegCount == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_Enable == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_Home == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_SetCmdPos == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_PAX_SetQeiPos == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_DEV_CheckLastCmdExeState == NULL)
    {
        ulReturn = 1;
    }

    //--------------------------------------------------------------------------------------------
    //插补器状态
    if (pCSMInterface->pf_IPO_Reset == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_SetCtrlMode == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_GetStateData == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_GetBufState == NULL)
    {
        ulReturn = 1;
    }

    //插补器自动模式相关指令
    if (pCSMInterface->pf_IPO_SetCrdSysFeedOverride == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_SetCrdSysRapidOverride == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_SetVerifyEnable == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_EnableSingleSegmentMode == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgRun == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgStop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgSegmentEndStop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgEStop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgSegmentEndStop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_ProgHold == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_AddMotionDataToIPOBuf == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_IPO_CloseAxisOutput == NULL)
    {
        ulReturn = 1;
    }

    //插补器手动模式相关指令
    if (pCSMInterface->pf_JOG_SetAxisPosCtrlMotion == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_JOG_SetAxisSpeedCtrlMotion == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_JOG_SetMotorPosCtrlMotion == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_JOG_SetMotorSpeedCtrlMotion == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_JOG_SetMotionStop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_JOG_MotorEStop == NULL)
    {
        ulReturn = 1;
    }

    //
    if (pCSMInterface->pf_MST_Reset == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_Run == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_Stop == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_Cancel == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_ProcessBeforeActionMST == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_ProcessAfterActionMST == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_CheckBeforeActionMSTExist == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_CheckAfterActionMSTExist == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_MST_GetStateData == NULL)
    {
        ulReturn = 1;
    }


    //系统状态管理模块相关接口
    if (pCSMInterface->pf_SYS_GetAlarmStatus == NULL)
    {
        ulReturn = 1;
    }


    //------------------------------------------------------------------------------------------------------------------
    //错误管理相关接口
    if (pCSMInterface->pf_ECM_GenerateErrorCode == NULL)
    {
        ulReturn = 1;
    }

    if (pCSMInterface->pf_ECM_PutErrorToBuf == NULL)
    {
        ulReturn = 1;
    }

    //
    m_CSMInterface = *pCSMInterface;
    m_nCSMInterfaceInit = 1;

    return ulReturn;
}
