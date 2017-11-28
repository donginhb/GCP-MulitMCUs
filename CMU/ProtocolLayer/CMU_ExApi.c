
#include "string.h"
#include "CMU_ExApi.h"
#include "CMU_CmdProcess.h"

#include "..\\CMU_ErrorCodeDef.h"

#include "ComDataStructDef.h"


CMU_EXTERNAL_FUN_TEBLE m_sExternalFunTable={NULL};

void voidFun_void(void){};

uBit32 u32Fun_p8_u32(Bit8* pUpdateData, uBit32 ulDataLen){return CMU_ERR_UNBAND;}

const DEVICE_STATE* SysGetDevStateFun(uBit32 ulDeviceNo){return NULL;}
const SYS_CTRL_PARM* SysGetCtrlParmFun(void){return NULL;}
const SYS_STATE_DATA* SysGetStateFun(void){return NULL;}        //pState是出参
uBit32 CrdSetAxisMapFun(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount){return CMU_ERR_UNBAND;}
uBit32 CrdSetCtrlParmFun(Bit32 iCrdSysIndex, CRDSYS_PARM* pCrdParm){return CMU_ERR_UNBAND;}


Bit32 IPR_GetVar(Bit32 iCrdSysIndex, Bit32 iVarNO, struct universe *pVarData){return CMU_ERR_UNBAND;}
uBit32 CrdLoadIIPDataFun(Bit32 iCrdSysIndex, IIP_BUF* pIIPData){return CMU_ERR_UNBAND;}     
const CRDSYS_AXIS_MAP_TABLE* CrdGetAxisMapFun(void){return NULL;}
const CRDSYS_PARM* CrdGetCtrlParmFun(void){return NULL;}
const CRDSYS_STATE_DATA* CrdGetStateFun(Bit32 iCrdSysIndex){ return NULL;}        //pState是出参
const DATA_BUF_MAN* CrdGetProgBufStateFun(Bit32 iCrdSysIndex){ return NULL;}        //pState是出参

//电机和轴默认外部接口-----
uBit32 MotorCtrlParmFun(Bit32 iAxisNO, AXIS_PARM* pAxisParm){return CMU_ERR_UNBAND;} 
uBit32 MotorPitchCmpParmFun(Bit32 iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm){return CMU_ERR_UNBAND;} 
uBit32 MotorPosCtrlMotionFun(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotionl){return CMU_ERR_UNBAND;} 
uBit32 MotorSpeedCtrlMotionFun    (Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion){return CMU_ERR_UNBAND;} 
const AXIS_PARM* MotorGetCtrlParmFun(void){return NULL;} 
const AXIS_PITCH_CMP_PARM* MotorGetPitchCmpParmFun(void){return NULL;} 
uBit32 AxisPosCtrlMotionFun(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion){return CMU_ERR_UNBAND;} 
uBit32 AxisSpeedCtrlMotionFun(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion){return CMU_ERR_UNBAND;} 


uBit32 ProgIOCode(uBit32 ulCrdNO, uBit32 ulDeviceNo, uBit32 ulIOMask, uBit32 ulIOState, uBit32 ulDelayMs){return CMU_ERR_UNBAND;} 
uBit32 ProgRapid(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, BooLean bAbsFlag){return CMU_ERR_UNBAND;} 
uBit32 ProgLine(uBit32 ulCrdNO, uBit32 ulAxisMask, Bit32* pPos, float32 fSpeed, BooLean bAbsFlag){return CMU_ERR_UNBAND;} 
uBit32 ProgArcC(uBit32 ulCrdNO, uBit32* pAxisNO,  Bit32 *pEndPos, Bit32* pCenter, BooLean bClockWise, float32 fSpeed, BooLean bAbsFlag){return CMU_ERR_UNBAND;} 
uBit32 ProgArcR(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 iRadius, BooLean bClockWise,float32 fSpeed, BooLean bAbsFlag){return CMU_ERR_UNBAND;} 
uBit32 ProgArcPT(uBit32 ulCrdNO, uBit32* pAxisNO, Bit32 *pEndPos,  Bit32 *pMidPos, float32 fSpeed, BooLean bAbsFlag){return CMU_ERR_UNBAND;}


uBit32 u32Fun_u32(uBit32 ulParm0){return CMU_ERR_UNBAND;};
uBit32 u32Fun_p32(Bit32 *p){return CMU_ERR_UNBAND;};
uBit32 u32Fun_pdevconf_u32(DEVICE_CONFIG* pDevConf, uBit32 ulParm){return CMU_ERR_UNBAND;}
uBit32 u32Fun_psysctrlparm(SYS_CTRL_PARM* pSysCtrlParm){return CMU_ERR_UNBAND;}
uBit32 u32Fun_u32_p8_u32(uBit32 ulDeviceNo,uBit8* pUpdateData, uBit32 ulDataLen){return CMU_ERR_UNBAND;}
uBit32 u32Fun_pversion(SOFTWARE_VERSION* pVersion){return CMU_ERR_UNBAND;}    
uBit32 u32Fun_u32_pversion(uBit32 ulDeviceNo, SOFTWARE_VERSION* pVersion){return CMU_ERR_UNBAND;}    
uBit32 u32Fun_u32_pbusrsmsdata(uBit32 ulDeviceNo, BUS_RS_MS_DATA* pBusRsMsData){return CMU_ERR_UNBAND;}    

uBit32 u32Fun_void(void){return CMU_ERR_UNBAND;}                
uBit32 u32Fun_u32_pdevinfo(uBit32 ulDeviceNo, DEVICE_INFO* pDeviceInfo){return CMU_ERR_UNBAND;}    //

//void voidFun_u32(uBit32 iCrdSysIndex){return;}
//void voidFun_u32_32(uBit32 a, Bit32 b){return;};
//void voidFun_i32_pi32(Bit32 a, Bit32 *p){return;}
//void voidFun_u32_32_p32(uBit32 iCrdSysIndex, Bit32 iAxisMask, Bit32 *pPos){return;}

Bit32 i32Fun(void){return 0;}
Bit32 i32Fun_u32(uBit32 iCrdSysIndex){return CMU_ERR_UNBAND;}
Bit32 i32Fun_32(Bit32 iCrdSysIndex){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_32(uBit32 a, Bit32 b){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_p32(uBit32 iCrdSysIndex, Bit32* pLine){return CMU_ERR_SUCCESS;}

//Bit32 i32Fun_32_pu8_32(Bit32 b, uBit8* p, Bit32 c){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_pu8_u32_u8(uBit32 b, uBit8* p, uBit32 c,uBit8 d){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_u32_pu8_u32(uBit32 a, uBit32 b, uBit8* p, uBit32 c){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_u32_pu8_u32_u8(uBit32 a, uBit32 b, uBit8* p, uBit32 c,uBit8 d){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_32_pcut(uBit32 a, Bit32 b, CUTTER_T *pt){return CMU_ERR_UNBAND;}
Bit32 i32Fun_u32_32_p32(uBit32 a, Bit32 b, Bit32 *p){return CMU_ERR_SUCCESS;}


uBit32 u32Fun_u16(uBit16 nCrdSysMask){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_32(Bit32 iCrdSysIndex){return CMU_ERR_UNBAND;}    
uBit32 u32Fun_32_32(Bit32 iCrdSysIndex, Bit32 iMode){return CMU_ERR_UNBAND;}    
uBit32 u32Fun_32_32_u32(Bit32 iCrdSysIndex, Bit32 iDir, uBit32 ulSpeed){return CMU_ERR_UNBAND;}
uBit32 u32Fun_32_32_32_32(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iHomeFlag, Bit32 iWaitExeResult){return CMU_ERR_UNBAND;} 

uBit32 u32Fun_32_u32(Bit32 iCrdSysIndex, uBit32 ulMode){return CMU_ERR_UNBAND;}
uBit32 u32Fun_32_u32_pu8_u32(Bit32 iCrdSysIndex, uBit32 ulStartPos, uBit8* pData, uBit32 ulLen){return CMU_ERR_UNBAND;}
uBit32 u32Fun_32_u32_u32_pu32(Bit32 iCrdIndex, uBit32 ulToolIndex, uBit32 ulCount, uBit32* pData){return CMU_ERR_UNBAND;}     
uBit32 u32Fun_32_p8_32(Bit32 iCrdSysIndex, Bit8* pUserData, Bit32 iDataLen){return CMU_ERR_UNBAND;}     //下载用户数据
uBit32 u32Fun_32_f32(Bit32 iCrdSysIndex, float32 dOverride){return CMU_ERR_UNBAND;}
uBit32 u32Fun_u32_u8(uBit32 ulCrdNO, uBit8 cTCode){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_pu8(uBit32 ulAxisNo, uBit8 *pData){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_u16_pu8(uBit32 ulAxisNo, uBit16 nParmNo, uBit8 *pData){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_u16(uBit32 ulCrdNO, uBit16 nMCode){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_u32(uBit32 ulCrdNO, uBit32 ulDelayMs){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_u32_u32(uBit32 ulIONO, uBit32 ulMask, uBit32 ulStatus){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_32_32(uBit32 ulAxisNO, Bit32 iHomeFlag, Bit32 iWaitExeResult){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_pu8_u32(uBit32 iCrdSysIndex, uBit8 *pBuf, uBit32 ulLen){return CMU_ERR_UNBAND;}
uBit32 u32Fun_u32_pu32(uBit32 ul, uBit32* p){return CMU_ERR_UNBAND;}        
uBit32 u32Fun_u32_pu32_u32(uBit32 ulStartRegIndex, uBit32* pData, uBit32 ulCount){return CMU_ERR_UNBAND;}        
uBit32 u32Fun_u32_pu32_pu32(uBit32 ulDevNo, uBit32* pCmdType, uBit32 *pExeRes){return CMU_ERR_UNBAND;}        
uBit32 u32Fun_32_32_pu32_pu32(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32 *pCmdType, uBit32 *pExeRes){return CMU_ERR_UNBAND;} 
uBit32 u32Fun_u32_pu8_u32_u8(uBit32 b, uBit8* p, uBit32 c,uBit8 d){return CMU_ERR_UNBAND;}

//错误管理模块接口
uBit32 GetErrorCodeFun(uBit32 *pulErrorCode)
{
    return 1;
}


//系统时钟计数
double64 GetTickCountFun(void)
{
    return 0;
}

void CMU_InitExApi(void)
{
    if (m_sExternalFunTable.pf_SYS_Reset==0)
        m_sExternalFunTable.pf_SYS_Reset        =&voidFun_void        ;
    
    if (m_sExternalFunTable.pf_SYS_UpdateSLC==0)
        m_sExternalFunTable.pf_SYS_UpdateSLC        = &u32Fun_p8_u32    ;

    if (m_sExternalFunTable.pf_SYS_UpdateIPO==0)
        m_sExternalFunTable.pf_SYS_UpdateIPO        =&u32Fun_p8_u32        ;

    if (m_sExternalFunTable.pf_DEV_BlinkLED==0)
        m_sExternalFunTable.pf_DEV_BlinkLED            =&u32Fun_u32            ;

    if (m_sExternalFunTable.pf_DEV_SetConfig==0)
        m_sExternalFunTable.pf_DEV_SetConfig        =&u32Fun_pdevconf_u32    ;

    if (m_sExternalFunTable.pf_SPM_SetSysCtrlParm==0)
        m_sExternalFunTable.pf_SPM_SetSysCtrlParm    =&u32Fun_psysctrlparm    ;

    if (m_sExternalFunTable.pf_DEV_SendUpdateAppData==0)
        m_sExternalFunTable.pf_DEV_SendUpdateAppData        =&u32Fun_u32_p8_u32        ;

    if (m_sExternalFunTable.pf_SYS_UpdateBootloader==0)
        m_sExternalFunTable.pf_SYS_UpdateBootloader        =&u32Fun_p8_u32        ;

    if (m_sExternalFunTable.pf_DEV_SendUpdateBootData==0)
        m_sExternalFunTable.pf_DEV_SendUpdateBootData        =&u32Fun_u32_p8_u32        ;

    if (m_sExternalFunTable.pf_DEV_CheckLastCmdExeState==0)
        m_sExternalFunTable.pf_DEV_CheckLastCmdExeState        =&u32Fun_u32_pu32_pu32    ;


    if (m_sExternalFunTable.pf_SYS_GetSLCVersion==0)
        m_sExternalFunTable.pf_SYS_GetSLCVersion        =&u32Fun_pversion        ;

    if (m_sExternalFunTable.pf_SYS_GetIPOVersion==0)
        m_sExternalFunTable.pf_SYS_GetIPOVersion        =&u32Fun_pversion        ;

    if (m_sExternalFunTable.pf_SYS_SetComType==0)
        m_sExternalFunTable.pf_SYS_SetComType        =&u32Fun_u32        ;

    if (m_sExternalFunTable.pf_DEV_GetVersion==0)
        m_sExternalFunTable.pf_DEV_GetVersion        =&u32Fun_u32_pversion        ;

    if (m_sExternalFunTable.pf_DEV_GetBusDriverVersion==0)
        m_sExternalFunTable.pf_DEV_GetBusDriverVersion        =&u32Fun_u32_pversion    ;

    if (m_sExternalFunTable.pf_DEV_GetBusRsMsData==0)
        m_sExternalFunTable.pf_DEV_GetBusRsMsData        =&u32Fun_u32_pbusrsmsdata    ;

    if (m_sExternalFunTable.pf_DEV_GetCount==0)
        m_sExternalFunTable.pf_DEV_GetCount        =&u32Fun_void        ;

    if (m_sExternalFunTable.pf_DEV_GetInfo==0)
        m_sExternalFunTable.pf_DEV_GetInfo        =&u32Fun_u32_pdevinfo        ;

    if(m_sExternalFunTable.pf_DEV_Scan==0)
        m_sExternalFunTable.pf_DEV_Scan =   &u32Fun_void;

    if (m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr==0)
        m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr        =&SysGetCtrlParmFun        ;

    if (m_sExternalFunTable.pf_SYS_GetStateReadAddr==0)
        m_sExternalFunTable.pf_SYS_GetStateReadAddr        =&SysGetStateFun        ;

    //高速接口
    if (m_sExternalFunTable.pf_GPIO_GetHSpdOutputStatus==0)
        m_sExternalFunTable.pf_GPIO_GetHSpdOutputStatus        =&u32Fun_void        ;

    if (m_sExternalFunTable.pf_GPIO_GetHSpdInputStatus==0)
        m_sExternalFunTable.pf_GPIO_GetHSpdInputStatus        =&u32Fun_void        ;

    if (m_sExternalFunTable.pf_GPIO_SetHSpdOutputState==0)
        m_sExternalFunTable.pf_GPIO_SetHSpdOutputState        =&u32Fun_u32_u32        ;

    if (m_sExternalFunTable.pf_MDAC_SetHSpdDaData==0)
        m_sExternalFunTable.pf_MDAC_SetHSpdDaData        =&u32Fun_u32_u32        ;

    if (m_sExternalFunTable.pf_MDAC_GetHSpdDaData==0)
        m_sExternalFunTable.pf_MDAC_GetHSpdDaData        =&u32Fun_u32_pu32        ;

    if (m_sExternalFunTable.pf_QEI_GetHSpdQeiPos==0)
        m_sExternalFunTable.pf_QEI_GetHSpdQeiPos        =&i32Fun        ;

    if (m_sExternalFunTable.pf_QEI_GetHSpdQeiSpeed==0)
        m_sExternalFunTable.pf_QEI_GetHSpdQeiSpeed        =&i32Fun        ;


    if (m_sExternalFunTable.pf_DEV_GetStateReadAddr==0)
        m_sExternalFunTable.pf_DEV_GetStateReadAddr             =&SysGetDevStateFun;


    if (m_sExternalFunTable.pf_SPM_SetCrdSysAxisMapTable==0)
        m_sExternalFunTable.pf_SPM_SetCrdSysAxisMapTable            =&CrdSetAxisMapFun                ;

    if (m_sExternalFunTable.pf_SPM_SetCrdParm==0)
        m_sExternalFunTable.pf_SPM_SetCrdParm            =&CrdSetCtrlParmFun            ;

    if (m_sExternalFunTable.pf_SPM_SaveParm==0)
        m_sExternalFunTable.pf_SPM_SaveParm        =&u32Fun_void    ;

    if (m_sExternalFunTable.pf_SPM_SaveConfig==0)
        m_sExternalFunTable.pf_SPM_SaveConfig        =&u32Fun_void    ; 

    if (m_sExternalFunTable.pf_CSN_SetRapidOverride==0)
        m_sExternalFunTable.pf_CSN_SetRapidOverride        =&u32Fun_32_f32        ;

    if (m_sExternalFunTable.pf_CSM_SetFeedOverride==0)
        m_sExternalFunTable.pf_CSM_SetFeedOverride        =&u32Fun_32_f32        ;

    if (m_sExternalFunTable.pf_CSM_SetSpindleOverride==0)
        m_sExternalFunTable.pf_CSM_SetSpindleOverride    =u32Fun_32_f32        ;

    if (m_sExternalFunTable.pf_CSM_SetCtrlMode==0)
        m_sExternalFunTable.pf_CSM_SetCtrlMode            =u32Fun_32_u32            ;

    if (m_sExternalFunTable.pf_CSM_SpindleRun==0)
        m_sExternalFunTable.pf_CSM_SpindleRun            =u32Fun_32_32_u32                ;

    if (m_sExternalFunTable.pf_CSM_SpindleStop==0)
        m_sExternalFunTable.pf_CSM_SpindleStop            =u32Fun_32            ;

    if (m_sExternalFunTable.pf_CSM_EnableVerify==0)
        m_sExternalFunTable.pf_CSM_EnableVerify                =u32Fun_32_32                ;

    if (m_sExternalFunTable.pf_CSM_ProgEStop==0)
        m_sExternalFunTable.pf_CSM_ProgEStop                =u32Fun_u16                    ;

    if (m_sExternalFunTable.pf_CSM_GetVersion==0)
        m_sExternalFunTable.pf_CSM_GetVersion        =&u32Fun_pversion        ;

    if (m_sExternalFunTable.pf_CSM_Reset==0)
        m_sExternalFunTable.pf_CSM_Reset                =u32Fun_32                    ;

    if (m_sExternalFunTable.pf_CSM_SetProgBufMode==0)
        m_sExternalFunTable.pf_CSM_SetProgBufMode        =u32Fun_32_32            ;


    if (m_sExternalFunTable.pf_CSM_ClearProgData==0)
        m_sExternalFunTable.pf_CSM_ClearProgData            =u32Fun_32_32            ;

    if (m_sExternalFunTable.pf_CSM_EnableSingleSegmentMode==0)
        m_sExternalFunTable.pf_CSM_EnableSingleSegmentMode        =u32Fun_32_32        ;

    if (m_sExternalFunTable.pf_SPM_SetLookaheadEnable==0)
        m_sExternalFunTable.pf_SPM_SetLookaheadEnable            =u32Fun_32_32            ;

    if (m_sExternalFunTable.pf_CSM_ProgRun==0)
        m_sExternalFunTable.pf_CSM_ProgRun                =u32Fun_u16                ;

    if (m_sExternalFunTable.pf_CSM_ProgHold==0)
        m_sExternalFunTable.pf_CSM_ProgHold                =u32Fun_u16                ;

    if (m_sExternalFunTable.pf_CSM_ProgStop==0)
        m_sExternalFunTable.pf_CSM_ProgStop                =u32Fun_u16                ;

    if (m_sExternalFunTable.pf_CSM_ProgCancel==0)
        m_sExternalFunTable.pf_CSM_ProgCancel            =u32Fun_u16                ;

    if (m_sExternalFunTable.pf_CSM_ProgSegmentEndStop==0)
        m_sExternalFunTable.pf_CSM_ProgSegmentEndStop   =u32Fun_u16            ;

    if (m_sExternalFunTable.pf_CSM_SetUserBufMode==0)
        m_sExternalFunTable.pf_CSM_SetUserBufMode        =u32Fun_32_32        ;

    if (m_sExternalFunTable.pf_CSM_ClearUserData==0)
        m_sExternalFunTable.pf_CSM_ClearUserData        =u32Fun_32        ;

    if (m_sExternalFunTable.pf_CSM_AddUserDataToBuf==0)
        m_sExternalFunTable.pf_CSM_AddUserDataToBuf            =u32Fun_32_p8_32            ;


    //------------
    if (m_sExternalFunTable.pf_IPR_SetPpiEnable==0)
        m_sExternalFunTable.pf_IPR_SetPpiEnable            = i32Fun_32            ;

    if (m_sExternalFunTable.pf_IPR_GetPpiEnable==0)
        m_sExternalFunTable.pf_IPR_GetPpiEnable            = i32Fun            ;

    if (m_sExternalFunTable.pf_IPR_Cancel==0)
        m_sExternalFunTable.pf_IPR_Cancel            = i32Fun_u32            ;

    if (m_sExternalFunTable.pf_IPR_Pause==0)
        m_sExternalFunTable.pf_IPR_Pause            = i32Fun_u32            ;

    if (m_sExternalFunTable.pf_IPR_Cycle==0)
        m_sExternalFunTable.pf_IPR_Cycle            = i32Fun_u32            ;

    if (m_sExternalFunTable.pf_IPR_ProgReset==0)
        m_sExternalFunTable.pf_IPR_ProgReset            = i32Fun_u32            ;

    if (m_sExternalFunTable.pf_IPR_PutGCodeToIprBuf==0)
        m_sExternalFunTable.pf_IPR_PutGCodeToIprBuf        = i32Fun_u32_u32_pu8_u32_u8            ;

    if (m_sExternalFunTable.pf_SYS_UpdateCCFileData==0)
        m_sExternalFunTable.pf_SYS_UpdateCCFileData        = u32Fun_u32_pu8_u32_u8            ;

    if (m_sExternalFunTable.pf_IPR_GetState==0)
        m_sExternalFunTable.pf_IPR_GetState            = i32Fun_u32            ;

    if (m_sExternalFunTable.pf_IPR_RndStart==0)
        m_sExternalFunTable.pf_IPR_RndStart            = i32Fun_u32_32        ;

    if (m_sExternalFunTable.pf_IPR_SetCrdZero==0)
        m_sExternalFunTable.pf_IPR_SetCrdZero            = i32Fun_u32_32_p32        ;

    if (m_sExternalFunTable.pf_IPR_GetCrdZero==0)
        m_sExternalFunTable.pf_IPR_GetCrdZero            = i32Fun_u32_32_p32        ;    

    if (m_sExternalFunTable.pf_IPR_SetToolData==0)
        m_sExternalFunTable.pf_IPR_SetToolData        = i32Fun_u32_32_pcut    ;

    if (m_sExternalFunTable.pf_IPR_GetToolData==0)
        m_sExternalFunTable.pf_IPR_GetToolData        = i32Fun_u32_32_pcut    ;

    if (m_sExternalFunTable.pf_IPR_GetBpDataLen==0)
        m_sExternalFunTable.pf_IPR_GetBpDataLen            = i32Fun_u32            ;                

    if (m_sExternalFunTable.pf_IPR_GetBpData==0)
        m_sExternalFunTable.pf_IPR_GetBpData            = i32Fun_u32_u32_pu8_u32            ;                

    if (m_sExternalFunTable.pf_IPR_SetBpData==0)
        m_sExternalFunTable.pf_IPR_SetBpData            = i32Fun_u32_u32_pu8_u32_u8            ;                

    if (m_sExternalFunTable.pf_IPR_SetTool==0)
        m_sExternalFunTable.pf_IPR_SetTool            = i32Fun_u32_32            ;    

    if (m_sExternalFunTable.pf_IPR_SetBaseTool==0)
        m_sExternalFunTable.pf_IPR_SetBaseTool            = i32Fun_u32_32            ;    

    if (m_sExternalFunTable.pf_IPR_UpdateCrdData==0)
        m_sExternalFunTable.pf_IPR_UpdateCrdData            = i32Fun            ;    

    if (m_sExternalFunTable.pf_IPR_GetCurLineNo==0)
        m_sExternalFunTable.pf_IPR_GetCurLineNo            = i32Fun_u32_p32            ;    

    if (m_sExternalFunTable.pf_IPR_GetBaseToolNo==0)
        m_sExternalFunTable.pf_IPR_GetBaseToolNo            = i32Fun_u32_p32            ;

    //后期完善----
    if (m_sExternalFunTable.pf_IPR_SetProgPos ==0)
        m_sExternalFunTable.pf_IPR_SetProgPos            = i32Fun_u32_32_p32                ;

    if (m_sExternalFunTable.pf_IPR_GetProgPos ==0)
        m_sExternalFunTable.pf_IPR_GetProgPos            = i32Fun_u32_p32                ;

    //-------------

    //if (m_sExternalFunTable.pCrdSetToolBaseDataFun==0)
    //    m_sExternalFunTable.pCrdSetToolBaseDataFun        =CrdSetToolBaseDataFun        ;

    if (m_sExternalFunTable.pf_CSM_AddProgDataToBuf==0)
        m_sExternalFunTable.pf_CSM_AddProgDataToBuf            =CrdLoadIIPDataFun            ;

    if (m_sExternalFunTable.pf_SPM_GetCrdSysAxisMapTableAddr==0)
        m_sExternalFunTable.pf_SPM_GetCrdSysAxisMapTableAddr            =CrdGetAxisMapFun                ;

    if (m_sExternalFunTable.pf_SPM_GetCrdSysParmAddr==0)
        m_sExternalFunTable.pf_SPM_GetCrdSysParmAddr            =CrdGetCtrlParmFun            ;

    if (m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr==0)
        m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr            =CrdGetStateFun                ;

    if (m_sExternalFunTable.pf_CSM_GetProgBufStateReadAddr==0)
        m_sExternalFunTable.pf_CSM_GetProgBufStateReadAddr        =CrdGetProgBufStateFun                ;

    if (m_sExternalFunTable.pf_CSM_GetUserBufStateReadAddr==0)
        m_sExternalFunTable.pf_CSM_GetUserBufStateReadAddr        = CrdGetProgBufStateFun                ;

    //if (m_sExternalFunTable.pCrdGetToolBaseDataFun==0)
    //    m_sExternalFunTable.pCrdGetToolBaseDataFun        =CrdGetToolBaseDataFun        ;

    if (m_sExternalFunTable.pf_CSM_UploadProgData==0)
        m_sExternalFunTable.pf_CSM_UploadProgData        =u32Fun_32_u32_pu8_u32            ;

    if (m_sExternalFunTable.pf_CSM_UploadUserData==0)
        m_sExternalFunTable.pf_CSM_UploadUserData        =u32Fun_32_u32_pu8_u32            ;

    if (m_sExternalFunTable.pf_CSM_ProgDwell==0)
        m_sExternalFunTable.pf_CSM_ProgDwell        =u32Fun_u32_u32            ;
    if (m_sExternalFunTable.pf_CSM_ProgMCode==0)
        m_sExternalFunTable.pf_CSM_ProgMCode        =u32Fun_u32_u16            ;
    if (m_sExternalFunTable.pf_CSM_ProgSCode==0)
        m_sExternalFunTable.pf_CSM_ProgSCode        =u32Fun_u32_u32            ;
    if (m_sExternalFunTable.pf_CSM_ProgTCode==0)
        m_sExternalFunTable.pf_CSM_ProgTCode        =u32Fun_u32_u8            ;
    if (m_sExternalFunTable.pf_CSM_ProgBCode==0)
        m_sExternalFunTable.pf_CSM_ProgBCode        =u32Fun_u32_u8            ;
    if (m_sExternalFunTable.pf_CSM_ProgIOCode==0)
        m_sExternalFunTable.pf_CSM_ProgIOCode        =ProgIOCode            ;
    if (m_sExternalFunTable.pf_CSM_ProgRapid==0)
        m_sExternalFunTable.pf_CSM_ProgRapid        =ProgRapid            ;
    if (m_sExternalFunTable.pf_CSM_ProgLine==0)
        m_sExternalFunTable.pf_CSM_ProgLine        =ProgLine            ;
    if (m_sExternalFunTable.pf_CSM_ProgArcC==0)
        m_sExternalFunTable.pf_CSM_ProgArcC        =ProgArcC            ;
    if (m_sExternalFunTable.pf_CSM_ProgArcR==0)
        m_sExternalFunTable.pf_CSM_ProgArcR        =ProgArcR            ;
    if (m_sExternalFunTable.pf_CSM_ProgLine==0)
        m_sExternalFunTable.pf_CSM_ProgLine        =ProgLine            ;
    if (m_sExternalFunTable.pf_CSM_ProgArcPT==0)
        m_sExternalFunTable.pf_CSM_ProgArcPT        =ProgArcPT            ;

    if (m_sExternalFunTable.pf_SPM_SetAxisParm==0)
        m_sExternalFunTable.pf_SPM_SetAxisParm        =MotorCtrlParmFun            ;

    if (m_sExternalFunTable.pf_SPM_SetAxisPitchCmpParm==0)
        m_sExternalFunTable.pf_SPM_SetAxisPitchCmpParm        =MotorPitchCmpParmFun        ;

    if (m_sExternalFunTable.pf_CSM_SetMotorPosCtrlMotion==0)
        m_sExternalFunTable.pf_CSM_SetMotorPosCtrlMotion        =MotorPosCtrlMotionFun    ;

    if (m_sExternalFunTable.pf_CSM_SetMotorSpeedCtrlMotion==0)
        m_sExternalFunTable.pf_CSM_SetMotorSpeedCtrlMotion            =MotorSpeedCtrlMotionFun    ;

    if (m_sExternalFunTable.pf_PAX_Home==0)
        m_sExternalFunTable.pf_PAX_Home            =&u32Fun_u32_32_32            ;

    if (m_sExternalFunTable.pf_CSM_SetMotorJogStop==0)
        m_sExternalFunTable.pf_CSM_SetMotorJogStop            =u32Fun_32                ;

    if (m_sExternalFunTable.pf_CSM_MotorJogEStop==0)
        m_sExternalFunTable.pf_CSM_MotorJogEStop            =u32Fun_32                ;

    if (m_sExternalFunTable.pf_PAX_SetCmdPos==0)
        m_sExternalFunTable.pf_PAX_SetCmdPos            =u32Fun_u32_32_32        ;

    if (m_sExternalFunTable.pf_PAX_SetQeiPos==0)
        m_sExternalFunTable.pf_PAX_SetQeiPos            =u32Fun_u32_32_32        ;

    if (m_sExternalFunTable.pf_PAX_Enable==0)
        m_sExternalFunTable.pf_PAX_Enable            =u32Fun_u32_32_32        ;

    if (m_sExternalFunTable.pf_PAX_Reset==0)
        m_sExternalFunTable.pf_PAX_Reset            =u32Fun_u32        ;

    if (m_sExternalFunTable.pf_PAX_SendSvParm==0)
        m_sExternalFunTable.pf_PAX_SendSvParm            =u32Fun_u32_pu8;        ;

    if (m_sExternalFunTable.pf_PAX_GetSvParm==0)
        m_sExternalFunTable.pf_PAX_GetSvParm            =u32Fun_u32_u16_pu8;        ;

    if (m_sExternalFunTable.pf_SPM_GetAxisParmAddr==0)
        m_sExternalFunTable.pf_SPM_GetAxisParmAddr        =MotorGetCtrlParmFun        ;

    if (m_sExternalFunTable.pf_SPM_GetAxisPitchCmpParmAddr==0)
        m_sExternalFunTable.pf_SPM_GetAxisPitchCmpParmAddr           =MotorGetPitchCmpParmFun;

    if (m_sExternalFunTable.pf_CSM_SetAxisPosCtrlMotion==0)
        m_sExternalFunTable.pf_CSM_SetAxisPosCtrlMotion            =AxisPosCtrlMotionFun                ;

    if (m_sExternalFunTable.pf_CSM_SetAxisSpeedCtrlMotion==0)
        m_sExternalFunTable.pf_CSM_SetAxisSpeedCtrlMotion            =AxisSpeedCtrlMotionFun                ;

    if (m_sExternalFunTable.pf_LAX_Home==0)
        m_sExternalFunTable.pf_LAX_Home            =u32Fun_32_32_32_32                ;

    if (m_sExternalFunTable.pf_CSM_SetAxisJogStop==0)
        m_sExternalFunTable.pf_CSM_SetAxisJogStop            =u32Fun_32_u32                ;

    if (m_sExternalFunTable.pf_CSM_AxisJogEStop==0)
        m_sExternalFunTable.pf_CSM_AxisJogEStop            =u32Fun_32_u32                ;

    if (m_sExternalFunTable.pf_LAX_SetCmdPos==0)
        m_sExternalFunTable.pf_LAX_SetCmdPos            =u32Fun_32_32_32_32            ;

    if (m_sExternalFunTable.pf_LAX_SetQeiPos==0)
        m_sExternalFunTable.pf_LAX_SetQeiPos            =u32Fun_32_32_32_32            ;

    if (m_sExternalFunTable.pf_LAX_Enable==0)
        m_sExternalFunTable.pf_LAX_Enable            =u32Fun_32_32_32_32            ;

    if (m_sExternalFunTable.pf_LAX_CheckLastCmdExeState==0)
        m_sExternalFunTable.pf_LAX_CheckLastCmdExeState            =u32Fun_32_32_pu32_pu32            ;

    if (m_sExternalFunTable.pf_IO_SetOutputStatus==0)
        m_sExternalFunTable.pf_IO_SetOutputStatus                =u32Fun_u32_u32_u32        ;

    if (m_sExternalFunTable.pf_IO_SetOutPutPWMMode==0)
        m_sExternalFunTable.pf_IO_SetOutPutPWMMode                    =u32Fun_u32_32_32            ;

    if (m_sExternalFunTable.pf_IO_SetOutPutPWMDuty==0)
        m_sExternalFunTable.pf_IO_SetOutPutPWMDuty        =u32Fun_u32_32_32    ;

    if (m_sExternalFunTable.pf_IO_SetOutPutPWMFreq==0)
        m_sExternalFunTable.pf_IO_SetOutPutPWMFreq                =u32Fun_u32_32_32        ;

    if (m_sExternalFunTable.pf_ADDA_SetDAStatus==0)
        m_sExternalFunTable.pf_ADDA_SetDAStatus            =u32Fun_u32_u32_u32    ;

    if (m_sExternalFunTable.pf_UREG_SetRegData==0)
        m_sExternalFunTable.pf_UREG_SetRegData                =u32Fun_u32_pu32_u32;

    if (m_sExternalFunTable.pf_UREG_GetRegData==0)
        m_sExternalFunTable.pf_UREG_GetRegData                =u32Fun_u32_pu32_u32;    

    if (m_sExternalFunTable.pf_ECM_GetErrorCode==0)
        m_sExternalFunTable.pf_ECM_GetErrorCode             =GetErrorCodeFun;

    if (m_sExternalFunTable.pf_SYS_GetSysTickCount==0)
        m_sExternalFunTable.pf_SYS_GetSysTickCount        = GetTickCountFun;

#ifdef RS_MONITOR_ENALBE        
    memset(&g_sCmuRsMonitorData,0, sizeof(CMU_RS_MS_DATA));
#endif
}                                     


void CMU_SetExternFun(CMU_EXTERNAL_FUN_TEBLE sCmuExternFunTable)
{
    m_sExternalFunTable = sCmuExternFunTable;
}


void CMU_SetIprOutApi(ipr_out_api *pIprOutApi)
{
    if (pIprOutApi->pf_IPR_SetPpiEnable)
        m_sExternalFunTable.pf_IPR_SetPpiEnable = pIprOutApi->pf_IPR_SetPpiEnable;

    if (pIprOutApi->pf_IPR_GetPpiEnable)
        m_sExternalFunTable.pf_IPR_GetPpiEnable = pIprOutApi->pf_IPR_GetPpiEnable;

    if (pIprOutApi->pf_IPR_Cancel)
        m_sExternalFunTable.pf_IPR_Cancel = pIprOutApi->pf_IPR_Cancel;

    if (pIprOutApi->pf_IPR_Pause)
        m_sExternalFunTable.pf_IPR_Pause = pIprOutApi->pf_IPR_Pause;

    if (pIprOutApi->pf_IPR_Cycle)
        m_sExternalFunTable.pf_IPR_Cycle = pIprOutApi->pf_IPR_Cycle;

    if (pIprOutApi->pf_IPR_ProgReset)
        m_sExternalFunTable.pf_IPR_ProgReset = pIprOutApi->pf_IPR_ProgReset;

    if (pIprOutApi->pf_IPR_PutGCodeToIprBuf)
        m_sExternalFunTable.pf_IPR_PutGCodeToIprBuf = pIprOutApi->pf_IPR_PutGCodeToIprBuf;

    if (pIprOutApi->pf_IPR_GetState)
        m_sExternalFunTable.pf_IPR_GetState = pIprOutApi->pf_IPR_GetState;

    if (pIprOutApi->pf_IPR_RndStart)
        m_sExternalFunTable.pf_IPR_RndStart = pIprOutApi->pf_IPR_RndStart;

    if (pIprOutApi->pf_IPR_SetCrdZero)
        m_sExternalFunTable.pf_IPR_SetCrdZero = pIprOutApi->pf_IPR_SetCrdZero;

    if (pIprOutApi->pf_IPR_GetCrdZero)
        m_sExternalFunTable.pf_IPR_GetCrdZero = pIprOutApi->pf_IPR_GetCrdZero;    

    if (pIprOutApi->pf_IPR_SetToolData)
        m_sExternalFunTable.pf_IPR_SetToolData = pIprOutApi->pf_IPR_SetToolData;

    if (pIprOutApi->pf_IPR_GetToolData)
        m_sExternalFunTable.pf_IPR_GetToolData = pIprOutApi->pf_IPR_GetToolData;


    if (pIprOutApi->pf_IPR_GetBpDataLen)
        m_sExternalFunTable.pf_IPR_GetBpDataLen = pIprOutApi->pf_IPR_GetBpDataLen;

    if (pIprOutApi->pf_IPR_GetBpData)
        m_sExternalFunTable.pf_IPR_GetBpData = pIprOutApi->pf_IPR_GetBpData;

    if (pIprOutApi->pf_IPR_SetBpData)
        m_sExternalFunTable.pf_IPR_SetBpData = pIprOutApi->pf_IPR_SetBpData;

    if (pIprOutApi->pf_IPR_SetTool)
        m_sExternalFunTable.pf_IPR_SetTool = pIprOutApi->pf_IPR_SetTool;

    if (pIprOutApi->pf_IPR_SetBaseTool)
        m_sExternalFunTable.pf_IPR_SetBaseTool = pIprOutApi->pf_IPR_SetBaseTool;

    if (pIprOutApi->pf_IPR_UpdateCrdData)
        m_sExternalFunTable.pf_IPR_UpdateCrdData = pIprOutApi->pf_IPR_UpdateCrdData;    

    if (pIprOutApi->pf_IPR_GetCurLineNo)
        m_sExternalFunTable.pf_IPR_GetCurLineNo            = pIprOutApi->pf_IPR_GetCurLineNo            ;    

    if (pIprOutApi->pf_IPR_GetBaseToolNo)
        m_sExternalFunTable.pf_IPR_GetBaseToolNo        = pIprOutApi->pf_IPR_GetBaseToolNo            ;    
}
