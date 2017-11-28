#include "SPM_ParmCheck.h"
#include "..\\DVM\\DEV_Interface.h"
#include "..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"

//检验系统控制参数合法性，参数合法返回1，否则返回0
int SPM_CheckSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
{
    if (pSysCtrlParm->nCrdSysCount==0||
        pSysCtrlParm->nCrdSysCount>CRDSYS_MAX_COUNT||
        pSysCtrlParm->nTimeCycle<1000||
        pSysCtrlParm->nTimeCycle>4000||
        (pSysCtrlParm->nTimeCycle%200 !=0))
    {
        return 0;
    }

    return 1;
}

//检验通道参数合法性，参数合法返回1，否则返回0
int SPM_CheckCrdSysParm(CRDSYS_PARM* pCrdSysParm)
{
    if (pCrdSysParm->dLineAxisCornerPreci<0 ||
        pCrdSysParm->dRotAxisCornerPreci<0  ||
        pCrdSysParm->dCircleErrorLim<0  ||
        pCrdSysParm->dProgMaxFeedSpeed<=0 ||
        pCrdSysParm->dProgMaxAcc<=0 ||
        pCrdSysParm->dProgMaxJerk<=0 ||
        pCrdSysParm->dMaxCornerAcc<=0 ||
        pCrdSysParm->dTimebaseSlewRate<=0 ||
        pCrdSysParm->dFeedHoldSlewRate<=0 ||
        pCrdSysParm->dStopSlewRate<=0 ||
        pCrdSysParm->lSpindleQeiSense<=0)
    {
        return 0;
    }
    return 1;
}


//检验轴参数合法性，参数合法返回1，否则返回0
int SPM_CheckAxisParm(AXIS_PARM* pAxisParm)
{
    //运动参数检查
    if (pAxisParm->AxisMoveParm.dMaxJogSpeed<ZERO ||
        pAxisParm->AxisMoveParm.dMaxJogAcc<ZERO ||
        pAxisParm->AxisMoveParm.dJogJerk<ZERO ||
        pAxisParm->AxisMoveParm.dMaxFeedSpeed<ZERO ||
        pAxisParm->AxisMoveParm.dMaxFeedAcc<ZERO ||
        pAxisParm->AxisMoveParm.dFeedJerk<ZERO ||
        pAxisParm->AxisMoveParm.dRapidSpeed<ZERO ||
        pAxisParm->AxisMoveParm.dRapidAcc<ZERO ||
        pAxisParm->AxisMoveParm.dRapidJerk<ZERO)
    {
        return 0;
    }

    //比例参数检查
    if (pAxisParm->AxisScaleParm.nType> AXIS_TYPE_ROTATE_360||
        pAxisParm->AxisScaleParm.lInternalPulseScale< 0 ||
        pAxisParm->AxisScaleParm.lQEIPulseScale< 0 ||
        pAxisParm->AxisScaleParm.lGearRatioPosScale< 0 ||
        pAxisParm->AxisScaleParm.lGearRatioPlusScale< 0 ||
        pAxisParm->AxisScaleParm.lCycleQeiPulseCount< 0)
    {
        return 0;
    }

    //回零参数检查
    if (pAxisParm->AxisHomeParm.nHomeMode>= AXIS_HOME_MODE_COUNT||
        pAxisParm->AxisHomeParm.nHomeDir < 0 ||
        pAxisParm->AxisHomeParm.nHomeDir > 1 ||
        pAxisParm->AxisHomeParm.dHomeRapidSpeed< ZERO ||
        pAxisParm->AxisHomeParm.dHomeApproachSpeed< ZERO ||
        pAxisParm->AxisHomeParm.dHomeLocatSpeed< ZERO ||
        pAxisParm->AxisHomeParm.dHomeIndexSpeed< ZERO)
    {
        return 0;
    }

    return 1;
}



//检验轴螺距误差补偿参数合法性，参数合法返回1，否则返回0
int SPM_CheckAxisPitchCmpParm(AXIS_PITCH_CMP_PARM* pAxisPitchCmpParm)
{
    return 1;
}

//映射表合法性检查
int SPM_CheckCrdAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount)
{
    int i, j, k;
    int iMotorNO;
    int MapMotorNO[AXIS_MAX_COUNT];

    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        MapMotorNO[i] = -1;
    }

    for (i = 0; i < iCrdSysCount; i++)
    {
        if (pCrdAxisMapTable[i].nCrdAxisCount > CRDSYS_AXIS_MAX_COUNT)
        {
            return 0;
        }

        for (j = 0; j < pCrdAxisMapTable[i].nCrdAxisCount; j++)
        {
            if (pCrdAxisMapTable[i].CrdSysAxisMap[j].cAxisName[0] == 0)
            { //未设置轴名
                return 0;
            }

            if (pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[0] >= PAX_GetRegCount())
            {
                return 0;
            }

            for (k = 0; k < AXIS_MAP_MOTOR_MAX_COUNT; k++)
            {
                if (k > 0)
                {
                    if (pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] != 0xFF && pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] >= PAX_GetRegCount())
                    {
                        return 0;
                    }
                }

                if (pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] != 0xFF)
                {
                    iMotorNO = pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k];

                    if (MapMotorNO[iMotorNO] > 0)
                    { //电机重复分配
                        return 0;
                    }

                    MapMotorNO[iMotorNO] = iMotorNO;
                }
            }
        }
    }

    return 1;
}
