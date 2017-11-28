#ifndef _CMD_PROCESS_H_
#define _CMD_PROCESS_H_

#include "ComDataStructDef.h"
#include "..\\CMU_DataStructDef.h"
#include "..\\UIN_GlobalDataDef.h"



#ifdef RS_MONITOR_ENALBE        
extern CMU_RS_MS_DATA g_sCmuRsMonitorData;
#endif

extern RCV_TIME_OUT_CTRL_DATA m_sRcvTimeOutCtrlData;

#ifdef __cplusplus
extern "C" {
#endif
 /*
函数名称：uBit32 CMU_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

功    能：数据块处理线程，如果处理成功则发生成功回包

参    数：pRcvCtrlData--接收控制数据区

返 回 值：0--成功
          非0--错误码
注意事项：数据块接收完成后调用
*/ 
uBit32 CMU_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

#ifdef __cplusplus
}
#endif

#endif