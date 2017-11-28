
//

#ifndef SPM_PARM_CHECK_H
#define SPM_PARM_CHECK_H

#include "..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"




#ifdef __cplusplus
extern "C" {
#endif


    int SPM_CheckSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm);
    int SPM_CheckCrdSysParm(CRDSYS_PARM* pCrdSysParm);
    int SPM_CheckAxisParm(AXIS_PARM* pAxisParm);
    int SPM_CheckAxisPitchCmpParm(AXIS_PITCH_CMP_PARM* pAxisPitchCmpParm);
    int SPM_CheckCrdAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount);

#ifdef __cplusplus
}
#endif





#endif 