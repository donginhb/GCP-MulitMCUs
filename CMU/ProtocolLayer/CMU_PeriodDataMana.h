#ifndef _CMU_PERIOD_DATA_MANA_H_
#define _CMU_PERIOD_DATA_MANA_H_

#include "CMU_ExApi.h"
#include "ComDataStructDef.h"

#ifdef __cplusplus
extern "C" {
#endif

 /*
函数名称：void CMU_RefreshInqRecordTick()

功    能：刷新所有周期性查询记录的计时时间

参    数：无

返 回 值：无

注意事项：通信模块周期性数据处理函数中调用
*/  
void CMU_RefreshInqRecordTick();

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
void CMU_PeriodDataManaInit();

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
void CMU_PeriodSendSwitch(BooLean bOpen);

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
uBit32 CMU_AddInqRecord(COM_DATA_ID ulID, uBit32 ulCycle);

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
uBit32 CMU_SendPeriodDataProc(COM_DATA_ID ulID);

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
uBit32 CMU_MainPeriodInqProc();

/*
函数名称：void CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：无

注意事项：通信模块初始化时调用
*/  
void CMU_ReportErrorProc();                //主动向上位机发送错误处理

#ifdef __cplusplus
}
#endif
#endif