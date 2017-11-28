
#include "string.h"
#include "CMU_ErrorMana.h"

#include "..\\..\\CNC\\DataStructDef\\ERR_DataStructDef.h"

 /*
函数名称：uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID);

功    能：通信模块创建错误码

参    数：ulCodeNo--错误编号
          ulID------通信ID

返 回 值：0或错误码

注意事项：数据块接收完成后调用
*/ 

uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID)
{
    ERROR_CODE sError;
    COM_DATA_ID sID;

    sID.ulFrameID = ulID;
    memset(&sError, 0, sizeof(ERROR_CODE));
    sError.ulCodeNO = ulCodeNo;
    sError.ulCrdNO = sID.ulComDataID.ulCrdNo;
    sError.ulDeviceNO = sID.ulComDataID.ulDevNo;
    
    
    sError.ulModuleNO = SYS_SOFT_MODULE_COM_WITH_UPCPU;

    return *((uBit32*)&sError);
}
