/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： 

修改了内容：

修改日期： 

原 作 者：毛军

完成日期：2015年08月19日        
*/
#ifndef DEV_DRIVE_STRUCT_DEF_H
#define DEV_DRIVE_STRUCT_DEF_H


#include "DEV_COM_CmdDef.h"
#include "..\\CMU\\UIN_GlobalDataDef.h"
#include "..\\DataStructDef\\DEV_DataStructDef.h"
#include "..\\DataStructDef\\AXIS_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"

#include "..\\..\\Version\\VER_DataStructDef.h"


//设备驱动数据结构体(要求各种总线设备必须提供下面列出的标准接口)
typedef struct _BusDeviceDriveData {

#ifdef RS_MONITOR_ENALBE
    BUS_RS_MS_DATA (*pf_DEV_GetBusRsMsData)(void);
#endif
    //-------------------------------------------系统配置------------------------------------------------------
    /*
    函 数 名：pf_BusInit
    功    能：总线初始化
    参    数：ulBusChMask --总线通道屏蔽字
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时调用
    注意事项：
    */
    uBit32 (*pf_DEV_BusInit)(uBit32 ulBusChMask);

    
    /*
    函 数 名：pf_BusFree
    功    能：检查总线上所有有效通道是否空闲
    参    数：
    返 回 值：空闲返回1，否则返回0
    调用位置：在设置配置信息等操作所有总线的指令时调用
    注意事项：
    */
    uBit32 (*pf_DEV_IsBusFree)(void);


    /*
    函 数 名：pf_DEV_SetConfig
    功    能：配置设备编号
    参    数：pDeviceConfig --设备配置数据存储地址
              ulDeviceCount --要配置的设备数量
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：配置设备编号前需先调用pf_DEV_SetDeviceInfoAddr和pf_DEV_SetDeviceStateAddr
    */
    uBit32 (*pf_DEV_SetConfig)(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount);   

    /*
    函 数 名：pf_Dev_SetAxisParmReadAddr
    功    能：设置轴参数读取起始地址
    参    数：pAxisParm --轴参数读取起始地址
    返 回 值：无
    调用位置：
    注意事项：发送轴进给指令前必须先设置轴参数
    */
    void (*pf_DEV_SetAxisParmReadAddr)(const AXIS_PARM* pAxisParm);  

    /*
    函 数 名：pf_DEV_SetSysCtrlParmReadAddr
    功    能：设置系统控制参数读取地址
    参    数：pSysCtrlParm --系统控制参数地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时，总线注册完成后调用
    注意事项：
    */
    void (*pf_DEV_SetSysCtrlParmReadAddr)(const SYS_CTRL_PARM* pSysCtrlParm); 


    /*
    函 数 名：pf_DEV_SetDeviceInfoAddr
    功    能：设置设备信息数据的存储地址
    参    数：pDeviceInfo --设备信息数据存储起始地址
              ulCount      --设备信息数据缓冲区长度（表示可存放多少个设备的信息数据）
    返 回 值：无
    调用位置：
    注意事项：
    */
    void (*pf_DEV_SetDeviceInfoAddr)(DEVICE_INFO* pDeviceInfo, uBit32 ulCount); 

    /*
    函 数 名：pf_DEV_SetDeviceStateAddr
    功    能：设置设备状态数据的存储地址
    参    数：pDeviceState    --设备状态数据存储起始地址
              ulCount        --设备状态数据缓冲区长度（表示可存放多少个设备的状态数据）
    返 回 值：无
    调用位置：
    注意事项：
    */
    void (*pf_DEV_SetDeviceStateAddr)(DEVICE_STATE* pDeviceState, uBit32 ulCount);


    //-------------------------------------------获取硬件信息---------------------------------------------------
    /*
    函 数 名：pf_Dev_Scan
    功    能：扫描总线上的设备
    参    数：
              pDeviceInfo  --设备硬件信息存储缓冲区起始地址
              ulDevBufLen  --设备硬件信息存储缓冲区长度（表示缓存区最大可以存放多少个设备的硬件信息）
              *pRecordDevCount --上次扫描已经记录的设备数量（出参）
              *pTotalDevCount --扫描发现的设备总数量（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_Scan)(DEVICE_INFO* pDeviceInfo, uBit32 ulDevBufLen, uBit32* pRecordDevCount, uBit32* pTotalDevCount);

    //-------------------------------------------执行控制指令---------------------------------------------------
    /*
    函 数 名：pf_DEV_SendFeed
    功    能：设置轴进给
    参    数：ulAxisNO --轴编号(设备编号)
                    IPOAxisState --轴插补状态数据
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_SendFeed)(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA  IPOAxisState);

    /*
    函 数 名：pf_DEV_SetCmd
    功    能：设置设备控制指令
    参    数：ulDevNO --设备编号
                    ulCmdType --指令类型，取值为：
                        DEV_CMD_INQ_INFO                    (0)    //查询设备信息
                        DEV_CMD_SET_NO                    (1)    //设置设备编号
                        DEV_CMD_BLINK_LED                (2)    //设置LED闪烁（设备测试）
                        DEV_CMD_INQ_VER                    (3)    //查询版本信息
                        DEV_CMD_CLR_ALM                    (4)    //报警清除
                        DEV_CMD_ENABLE_SV                (5)    //使能电机
                        DEV_CMD_SET_HOME                (6)    //回零
                        DEV_CMD_SET_LOCK                    (7)    //轴锁定(表示轴正在进行插补运动) （待完善）
                        DEV_CMD_RESET                        (8)    //系统复位
                        DEV_CMD_SET_CMD_POS            (9)    //设置指令位置
                        DEV_CMD_SET_QEI_POS            (10)    //设置编码器位置
                        DEV_CMD_SET_VERIFY_MODE        (11)    //设置进入校验模式
                        DEV_CMD_SET_IO_PWM_MODE    (12)    //设置IO进入PWM模式
                        DEV_CMD_SET_IO_PWM_DUTY    (13)    //设置IO进入PWM占空比
                        DEV_CMD_SET_IO_PWM_FREQ    (14)    //设置IO进入PWM频率
                        DEV_CMD_SET_FEED                    (15)    //设置进给
                        DEV_CMD_SET_OUTPUT                (16)    //设置IO输出
                        DEV_CMD_SET_FSTP                    (17)   //CCW驱动禁止                              
                        DEV_CMD_SET_RSTP                    (18)   //CW驱动禁止                                
                        DEV_CMD_SET_CLESC1ZERO        (19)   //偏差计算清零/速度选择1/零速箝位      
                        DEV_CMD_SET_INH_SC2            (20)   //指令脉冲禁止/速度选择2              
                        DEV_CMD_SET_FIL                    (21)   //CCW转矩限制                            
                        DEV_CMD_SET_RIL                    (22)   //CW转矩限制 
                        DEV_CMD_INQ_SV_ALM                (23)   //查询伺服报警
                        DEV_CMD_SET_IPO_CYCLE            (24)   //设置插补周期
                        DEV_CMD_GET_SV_PARM            (25)    //获取伺服参数

                        lCmdPara1，lCmdPara2 --控制参数，对应各控制指令其意义为：
                        DEV_CMD_BLINK_LED                --lCmdPara1：设备ID；lCmdPara2：无效
                        DEV_CMD_INQ_VER                    --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_CLR_ALM                    --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_ENABLE_SV                --lCmdPara1：0-断使能，1-使能；lCmdPara2：无效
                        DEV_CMD_SET_HOME                --lCmdPara1：0-回零停止，1-回零开始；lCmdPara2：无效
                        DEV_CMD_SET_LOCK                    --lCmdPara1：0-解除锁定，1-锁定；lCmdPara2：无效
                        DEV_CMD_RESET                        --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_SET_CMD_POS            --lCmdPara1：指令位置；lCmdPara2：无效
                        DEV_CMD_SET_QEI_POS            --lCmdPara1：编码器位置；lCmdPara2：无效
                        DEV_CMD_SET_VERIFY_MODE        --lCmdPara1：0-关闭校验模式，1-打开校验模式；lCmdPara2：无效
                        DEV_CMD_SET_IO_PWM_MODE    --lCmdPara1：要设置PWM模式的IO点编号；lCmdPara2：0-关闭PWM模式，1-开启PWM模式；
                        DEV_CMD_SET_IO_PWM_DUTY    --lCmdPara1：IO点编号；lCmdPara2：占空比
                        DEV_CMD_SET_IO_PWM_FREQ    --lCmdPara1：IO点编号；lCmdPara2：频率
                        DEV_CMD_SET_OUTPUT                --当为OUTPUT输出设备时，lCmdPara1：输出状态值；lCmdPara2：输出位掩码
                        --当为DA输出设备时，lCmdPara1：DA输出值；lCmdPara2：DA通道编号
                        DEV_CMD_SET_FSTP                    --lCmdPara1：0-CCW驱动禁止关闭，1-CCW驱动禁止打开；lCmdPara2：无效
                        DEV_CMD_SET_RSTP                    --lCmdPara1：0-CW驱动禁止关闭，1-CW驱动禁止打开；lCmdPara2：无效
                        DEV_CMD_SET_CLESC1ZERO        --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_SET_INH_SC2            --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_SET_FIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_SET_RIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_INQ_SV_ALM                --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_GET_SV_PARM            --lCmdPara1：要获取的伺服参数编号 lCmdPara2:无效
                    iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：内部限制同一总线上的设备同一时间只有一条控制指令在执行
    */
    uBit32 (*pf_DEV_SetCmd)(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult);
    

    /*
    函 数 名：pf_DEV_CheckFree
    功    能：检测设备是否空闲（没有指令在执行）
    参    数：ulDevNO --设备编号
    返 回 值：空闲返回1，否则返回0
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_CheckFree)(uBit32 ulDevNO);


    /*
    函 数 名：pf_DEV_LockForIPO
    功    能：锁定设备
    参    数：ulDevNO --设备编号
    返 回 值：成功返回0，否则返回失败
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_LockForIPO)(uBit32 ulDevNO, uBit32 ulLockFlag, uBit32 ulWaitExeResult);

    /*
    函 数 名：pf_DEV_CheckLastCmdExeState
    功    能：检测上一次的指令执行状态
    参    数：ulDevNO --设备编号
              *pCmdType --指令类型（出参）
    返 回 值：执行成功返回0，否则返回错误码（执行失败、正在执行）
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_CheckLastCmdExeState)(uBit32 ulDevNO, uBit32* pCmdType);        
    

    //-------------------------------------------设置参数---------------------------------------------------
    /*
    函 数 名：pf_DEV_SetParm
    功    能：设置设备参数
    参    数：ulDevNO --设备编号
              ulParmType --参数类型，取值为为：
                            DEV_PARM_TYPE_SV                                //伺服参数
                            DEV_PARM_TYPE_UPDATE_APP                //更新应用程序数据
                            DEV_PARM_TYPE_UPDATE_BOOT                //更新BOOTLOADER数据
              pParmBuf --参数数据缓冲区
              ulParmLen --参数数据长度
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_SetParm)(uBit32 ulDevNO, uBit32 ulParmType, uBit8* pParmBuf, uBit32 ulParmLen);


    //-------------------------------------------服务程序---------------------------------------------------
    /*
    函 数 名：pf_DEV_SynCycleService
    功    能：同步性周期服务函数
    参    数：无
    返 回 值：成功返回1，否则返回错误码
    调用位置：在插补中断中调用
    注意事项：
    */
    uBit32 (*pf_DEV_SynCycleService)(void);

    
    /*
    函 数 名：pf_DEV_AsynCycleService
    功    能：非同步性周期服务函数
    参    数：无
    返 回 值：成功返回1，否则返回错误码
    调用位置：(大循环中调用)
    注意事项：
    */
    uBit32 (*pf_DEV_AsynCycleService)(void);

    /*
    函 数 名：pf_DEV_GetSvParm
    功    能：获取伺服参数
    参    数：ulDevNO --设备编号
                    nParmNo --参数编号
                    pParmData -- 保存获取的参数
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_GetSvParm)(uBit32 ulDevNO, uBit16 nParmNo, uBit8* pParmData);

    //-------------------------------------------获取版本信息---------------------------------------------------
    /*
    函 数 名：pf_DEV_GetVersion
    功    能：获取设备软件版本信息
    参    数：ulDevNO --设备编号
              *pVersion --版本信息（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_GetVersion)(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion);

    /*
    函 数 名：pf_DEV_GetDriverVersion
    功    能：获取驱动库版本信息
    参    数：*pVersion --版本信息（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 (*pf_DEV_GetDriverVersion)(SOFTWARE_VERSION* pVersion);
}BUS_DEVICE_DRIVE;


#endif        //

