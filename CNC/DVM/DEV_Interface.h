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
#ifndef DEV_INTERFACE_H
#define DEV_INTERFACE_H

#include "DEV_DriveStructDef.h"
#include "..\\CMU\\UIN_GlobalDataDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"

#include "..\\..\\Version\\VER_DataStructDef.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef RS_MONITOR_ENALBE
    uBit32 DEV_GetBusRsMsData(uBit32 ulBusType, BUS_RS_MS_DATA *pBusRsMsData);
#endif

    /*
    函 数 名：DEV_SynCycleService
    功    能：同步性周期服务函数
    参    数：无
    返 回 值：成功返回1，否则返回错误码
    调用位置：在插补中断中调用
    注意事项：
    */
    uBit32 DEV_SynCycleService(void);

    /*
    函 数 名：DEV_MIIISynCycleService
    功    能：MIII总线同步周期服务函数
    参    数：无
    返 回 值：成功返回1，否则返回错误码
    调用位置：在非插补中断中执行
    注意事项：本接口在插补中断关闭时调用，因为MIII总线必须每个周期都必须收发，否则会导致同步错误
    */
    uBit32 DEV_MIIISynCycleService(void);

    /*
    函 数 名：DEV_AsynCycleService
    功    能：非同步性周期服务函数
    参    数：无
    返 回 值：成功返回1，否则返回错误码
    调用位置：(大循环中调用)
    注意事项：
    */
    uBit32 DEV_AsynCycleService(void);

    /*
    函 数 名：DEV_DrvReg
    功    能：总线驱动注册
    参    数：ulBusType --总线类型
              ulBusChannelMask --总线通道号掩码，bit0-对应通道1，bit1-对应通道2
              pBusDeviceDrive --总线设备驱动接口地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时调用
    注意事项：
    */
    uBit32 DEV_DrvReg(uBit32 ulBusType, uBit32 ulBusChannelMask, BUS_DEVICE_DRIVE* pBusDeviceDrive);

    /*
    函 数 名：DEV_SetAxisParmReadAddr
    功    能：设置轴控制参数读取地址
    参    数：pAxisParm --轴控参数地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时，总线注册完成后调用
    注意事项：
    */
      uBit32 DEV_SetAxisParmReadAddr(const AXIS_PARM *pAxisParm);
  
    /*
    函 数 名：uBit32 DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm);
    功    能：设置系统控制参数读取地址
    参    数：pSysCtrlParm --系统控制参数地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时，总线注册完成后调用
    注意事项：
    */
    uBit32 DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm);   

    /*
    函 数 名：DEV_SendSysCtrlParm
    功    能：发送系统控制参数,更新所有轴设备的系统控制参数
    参    数：pSysCtrlParm --系统控制参数数据存储地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_SendSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm);


    /*
    函 数 名：DEV_BusInit
    功    能：设备总线初始化
    参    数：无
    返 回 值：成功返回0，否则返回错误码
    调用位置：系统初始化时调用
    注意事项：本函数初始化已注册的全部总线
    */
    uBit32 DEV_BusInit(void);

    /*
    函 数 名：DEV_CheckBusInitSuccess
    功    能：检测总线是否初始化成功
    参    数：ulBusType --总线类型
    返 回 值：成功返回1，否则返回0
    调用位置：
    注意事项：
    */
    uBit32 DEV_CheckBusInitSuccess(uBit32 ulBusType);

    /*
    函 数 名：DEV_SetComCycle
    功    能：设置通讯周期
    参    数：ulTimeCycle --通信周期，单位：微秒
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：目前用于M-III总线的通信周期设置，根据系统插补周期设置M-III总线的通信周期
    */
    uBit32 DEV_SetComCycle(uBit32 ulTimeCycle); 

    /*
    函 数 名：DEV_Scan
    功    能：扫描设备
    参    数：无
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_Scan(void);


    /*
    函 数 名：DEV_GetCount
    功    能：获取设备总数量
    参    数：无
    返 回 值：返回设备总数量(已经扫描到的设备数量)
    调用位置：
    注意事项：
    */
    uBit32 DEV_GetCount(void);

    /*
    函 数 名：DEV_GetMotorCount
    功    能：获取电机设置总数
    参    数：无
    返 回 值：返回电机总数量(已经扫描到的设备数量)
    调用位置：
    注意事项：电机设备包括轴卡、CAN伺服、MIII伺服
    */
    uBit32 DEV_GetMotorCount(void);

    /*
    函 数 名：DEV_GetInfo
    功    能：获取设备信息
    参    数：ulDevIndex --设备序号，表示获取扫描到的第几个设备的信息
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_GetInfo(uBit32 ulDevIndex, DEVICE_INFO* pDeviceInfo);

    /*
    函 数 名：DEV_GetRegDevInfo
    功    能：获取已经编号的设备信息
    参    数：ulDevNO --设备编号
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_GetRegDevInfo(uBit32 ulDevNO, DEVICE_INFO* pDeviceInfo);

    /*
    函 数 名：DEV_BlinkLED;
    功    能：获取设备信息
    参    数：ulDeviceID --设备ID
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_BlinkLED(uBit32 ulDeviceID);

    /*
    函 数 名：DEV_SetConfig
    功    能：配置设备编号
    参    数：pDeviceConfig --设备配置数据存储地址
              ulDeviceCount --要配置的设备数量
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：要求:1、一次性设置所有设备编号（便于查错）；2、轴编号在前，其他设备编号在后，编号从1开始
    */
    uBit32 DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount);  


    /*
    函 数 名：DEV_GetVersion
    功    能：获取设备软件版本信息
    参    数：ulDevNO --设备编号
              *pVersion --版本信息（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion);

    /*
    函 数 名：DEV_GetBusDriverVersion
    功    能：获取总线驱动软件版本信息
    参    数：ulBusType --总线类型
              *pVersion --版本信息（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_GetBusDriverVersion(uBit32 ulBusType, SOFTWARE_VERSION* pVersion);

    /*
    函 数 名：DEV_GetState
    功    能：获取设备状态
    参    数：ulDevNO --设备编号
              *pDeviceState --设备状态数据（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：设置设备编号前不能获取设备状态
    */
    uBit32 DEV_GetState(uBit32 ulDevNO, DEVICE_STATE* pDeviceState);


    /*
    函 数 名：DEV_CheckNonAxisDevAlarm
    功    能：检测所有非轴卡设备的报警
    参    数：无
    返 回 值：报警状态
                    #define IO_STATUS_OVER_CURRENT                    (0x01)    //IO模块过流报警
                    #define IO_STATUS_OFF_LINE                                (0X02)    //IO或ADDA脱离总线
                    #define IO_STATUS_SEND_CYCLE_DATA_FAIL      (0x04)    //发送周期数据失败
    调用位置：
    注意事项：检测的是所有非轴设备的报警状态，只要有一个非轴模块报警则报警
    */
    uBit8 DEV_CheckNonAxisDevAlarm(void);

    /*
    函 数 名：DEV_GetStateReadAddr
    功    能：获取设备状态读地址
    参    数：ulDevNO --设备编号
    返 回 值：设备地址
    调用位置：
    注意事项：
    */
    const DEVICE_STATE* DEV_GetStateReadAddr(uBit32 ulDevNO); 

    /*
    函 数 名：const DEVICE_INFO* DEV_GetDevInfoReadAddr(void)
    功    能：获取设备资源信息起始地址
    参    数：无
    返 回 值：资源设备信息起始地址
    调用位置：
    注意事项：
    */
    const DEVICE_INFO* DEV_GetDevInfoReadAddr(void);

    /*
    函 数 名：DEV_SendUpdateAppData
    功    能：发送设备应用程序升级数据
    参    数：ulDevNO --设备编号
              pUpdateData --应用程序升级数据缓冲区
              ulDataLen --应用程序升级数据长度
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_SendUpdateAppData(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 ulDataLen);

    /*
    函 数 名：DEV_SendUpdateBootData
    功    能：发送设备BOOTLOADER程序升级数据
    参    数：ulDevNO --设备编号
              pUpdateData --BOOTLOADER程序升级数据缓冲区
              ulDataLen --BOOTLOADER程序升级数据长度
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 DEV_SendUpdateBootData(uBit32 ulDevNO, uBit8* pUpdateData, uBit32 ulDataLen);

    //
    /*
    函 数 名：DEV_SendCmd
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
    注意事项：内部限制同一总线上的设备同一时间只有一条控制指令在执行*/
    
    uBit32 DEV_SendCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult);


    /*
    函 数 名：DEV_CheckFree
    功    能：检测设备是否空闲（没有指令在执行）
    参    数：ulDevNO --设备编号
    返 回 值：空闲返回1，否则返回0
    调用位置：
    注意事项：
    */
    uBit32 DEV_CheckFree(uBit32 ulDevNO);


    /*
    函 数 名：DEV_CheckLastCmdExeState
    功    能：检测上一次的指令执行状态
    参    数：ulDevNO            --设备编号
                    *pCmdType        --指令类型（出参）
                    pExeRes            --执行结果(出参)
    返 回 值：执行完成返回0，1-正在执行
    调用位置：
    注意事项：
    */
    uBit32 DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType, uBit32 *pExeRes);    


    //----------------------------------------------轴相关操作接口（电机轴）----------------------------------------------
    /*
    函 数 名：PAX_GetRegCount
    功    能：获取已编号的轴（物理轴）数量
    参    数：
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_GetRegCount(void);

    /*
    函 数 名：PAX_SendAxisParm
    功    能：发送轴参数
    参    数：ulAxisNO --轴编号
              pAxisParm --轴参数数据存储地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_SendAxisParm(uBit32 ulAxisNO, AXIS_PARM* pAxisParm);

    /*
    函 数 名：PAX_SendSysCtrlParm
    功    能：发送系统控制参数
    参    数：ulAxisNO --轴编号
              pSysCtrlParm --系统控制参数数据存储地址
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    //uBit32 PAX_SendSysCtrlParm(uBit32 ulAxisNO, SYS_CTRL_PARM* pSysCtrlParm);//修改为设置设备系统控制参数

    /*
    函 数 名：PAX_SendSvParm
    功    能：设置伺服参数
    参    数：ulAxisNO        --轴编号
                    pParmData    --伺服参数编号和参数，前四个字节为参数编号，后四个字节为参数数据
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_SendSvParm(uBit32 ulAxisNO, uBit8 *pParmData);


    /*
    函 数 名：PAX_SendFeed
    功    能：设置轴进给
    参    数：ulAxisNO --轴编号
              IPOAxisState --轴插补状态数据
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState);

    //
    /*
    函 数 名：PAX_Enable
    功    能：设置轴使能
    参    数：ulAxisNO --轴编号
              iEnable --使能标记，0-断使能，1-加使能
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_Enable(uBit32 ulAxisNO, Bit32 iEnable, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_Reset
    功    能：轴复位
    参    数：ulAxisNO --轴编号
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_Reset(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_Home
    功    能：设置轴回零
    参    数：ulAxisNO --轴编号
              iHomeFlag --回零标记，0-回零停止，1-回零开始
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：本函数回零停止执行成功表示回零停止指令成功发送，不表示回零动作已经停止，从回零停止指令成功发送到回零动作完全停止有一个过程；
    */
    uBit32 PAX_Home(uBit32 ulAxisNO, Bit32 iHomeFlag, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_LockForIPO
    功    能：轴锁定（标记轴是否正在进行插补运动）
    参    数：ulAxisNO --轴编号
              iLockFlag --轴锁定标记，0-解锁，1-锁定
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：插补器启动时需要锁定轴，插补器停止后需要解锁轴
    */
    uBit32 PAX_LockForIPO(uBit32 ulAxisNO, Bit32 iLockFlag, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_SetCmdPos
    功    能：设置轴指令位置
    参    数：ulAxisNO --轴编号
              lCmdPos --指令位置，单位：um（旋转轴为0.001度）
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_SetCmdPos(uBit32 ulAxisNO, Bit32 lCmdPos, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_SetQeiPos
    功    能：设置编码器位置
    参    数：ulAxisNO --轴编号
              lQeiPos --编码器位置
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_SetQeiPos(uBit32 ulAxisNO, Bit32 lQeiPos, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_Verify
    功    能：设置轴校验模式
    参    数：ulAxisNO --轴编号
              iVerifyFlag --校验模式标记，0-关闭，1-开启
              iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_Verify(uBit32 ulAxisNO, uBit32 iVerifyFlag, Bit32 iWaitExeResult);

    /*
    函 数 名：PAX_GetStateData
    功    能：获取轴状态
    参    数：ulAxisNO --轴编号
              *pAxisStateData --轴状态数据（出参）
    返 回 值：成功返回1，否则返回0
    调用位置：
    注意事项：
    */
    uBit32 PAX_GetStateData(uBit32 ulAxisNO, AXIS_STATE_DATA* pAxisStateData);

    /*
    函 数 名：PAX_GetCtrlStatus
    功    能：获取轴控制状态字
    参    数：ulAxisNO --轴编号
    返 回 值：轴控制状态字
    调用位置：
    注意事项：
    */
    uBit32 PAX_GetCtrlStatus(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetRunningStatus
    功    能：获取轴运行状态字
    参    数：ulAxisNO --轴编号
    返 回 值：运行状态字
    调用位置：
    注意事项：
    */
    uBit32 PAX_GetRunningStatus(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetCmdPos
    功    能：获取轴指令位置
    参    数：ulAxisNO --轴编号
    返 回 值：轴指令位置
    调用位置：
    注意事项：
    */
    double64 PAX_GetCmdPos(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetPlusPos
    功    能：获取轴脉冲位置
    参    数：ulAxisNO --轴编号
    返 回 值：轴脉冲位置
    调用位置：
    注意事项：
    */
    double64 PAX_GetPlusPos(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetQeiPos
    功    能：获取轴编码器位置
    参    数：ulAxisNO --轴编号
    返 回 值：轴编码器位置
    调用位置：
    注意事项：
    */
    Bit32 PAX_GetQeiPos(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetCmdSpeed
    功    能：获取轴指令速度
    参    数：ulAxisNO --轴编号
    返 回 值：轴指令速度
    调用位置：
    注意事项：
    */
    float32 PAX_GetCmdSpeed(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetQeiSpeed
    功    能：获取轴编码器速度
    参    数：ulAxisNO --轴编号
    返 回 值：轴编码器速度
    调用位置：
    注意事项：
    */
    float32 PAX_GetQeiSpeed(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetAccSpeed
    功    能：获取轴加速度
    参    数：ulAxisNO --轴编号
    返 回 值：轴加速度
    调用位置：
    注意事项：
    */
    float32 PAX_GetAccSpeed(uBit32 ulAxisNO);

    /*
    函 数 名：PAX_GetSvStatus
    功    能：获取伺服系统状态
    参    数：ulAxisNO --轴编号
    返 回 值：伺服系统状态
    调用位置：
    注意事项：
    
    uBit32 PAX_GetSvStatus(uBit32 ulAxisNO);*/

    /*
    函 数 名：PAX_GetSvAlarm
    功    能：获取伺服报警状态
    参    数：ulAxisNO --轴编号
    返 回 值：伺服报警状态
    调用位置：
    注意事项：
    
    uBit32 PAX_GetSvAlarm(uBit32 ulAxisNO);*/

    /*
    函 数 名：PAX_GetSvParm
    功    能：获取伺服参数
    参    数：ulAxisNO        --轴编号
                    nParmNo        --伺服参数编号
                    pParmData    --保存获取的参数
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 PAX_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8 *pParmData);

    //----------------------------------------------IO相关操作接口----------------------------------------------
    /*
    函 数 名：IO_SetOutputState
    功    能：设置输出IO状态
    参    数：ulIONO --IO设备编号
              ulState --IO输出状态
              ulMask  --IO输出状态位掩码
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 IO_SetOutputStatus(uBit32 ulIONO, uBit32 ulStatus, uBit32 ulMask);

    /*
    函 数 名：IO_SetOutPutPWMMode
    功    能：设置输出点位PWM模式
    参    数：ulIONO --IO设备编号
              iIOBitNO --IO点编号，取值从1开始
              iMode --IO模式，0-普通模式，1-PWM模式
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 IO_SetOutPutPWMMode(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iMode);

    /*
    函 数 名：IO_SetOutPutPWMDuty
    功    能：设置PWM模式IO点的占空比
    参    数：ulIONO --IO设备编号
              iIOBitNO --IO点编号，取值从1开始
              iDuty --占空比，该变量值 = 实际占空比 * 1000，目的是为了用整数传递浮点数据
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 IO_SetOutPutPWMDuty(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iDuty);

    /*
    函 数 名：IO_SetOutPutPWMFreq
    功    能：设置PWM模式IO点的频率
    参    数：ulIONO --IO设备编号
              iIOBitNO --IO点编号，取值从1开始
              iFreq --频率
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 IO_SetOutPutPWMFreq(uBit32 ulIONO, Bit32 iIOBitNO, Bit32 iFreq);

    /*
    函 数 名：IO_GetStateData
    功    能：获取IO设备状态
    参    数：ulIONO --IO设备编号
              *pIOStateData --IO设备状态（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 IO_GetStateData(uBit32 ulIONO, IO_STATE_DATA* pIOStateData);

    /*
    函 数 名：IO_GetOutputStatus
    功    能：获取IO输出状态
    参    数：ulIONO --IO设备编号
    返 回 值：IO输出状态
    调用位置：
    注意事项：
    */
    uBit32 IO_GetOutputStatus(uBit32 ulIONO);


    /*
    函 数 名：IO_GetInputStatus
    功    能：获取IO输入状态
    参    数：ulIONO --IO设备编号
    返 回 值：IO输入状态
    调用位置：
    注意事项：
    */
    uBit32 IO_GetInputStatus(uBit32 ulIONO);

    /*
    函 数 名：ADDA_SetDAState
    功    能：设置DA输出状态
    参    数：ulADDANO --ADDA设备编号
              iDAChNO --DA通道号，取值从1开始
              iDAVaule --DA值
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 ADDA_SetDAStatus(uBit32 ulADDANO, uBit32 iDAChNO, uBit32 iDAVaule);

    /*
    函 数 名：ADDA_GetStateData
    功    能：获取ADDA设备状态
    参    数：ulADDANO --ADDA设备编号
              *pADDAStateData --ADDA设备状态（出参）
    返 回 值：成功返回0，否则返回错误码
    调用位置：
    注意事项：
    */
    uBit32 ADDA_GetStateData(uBit32 ulADDANO, ADDA_STATE_DATA* pADDAStateData);

    /*
    函 数 名：ADDA_GetADStatus
    功    能：获取AD输入状态
    参    数：ulADDANO --ADDA设备编号
              iADChNO --AD通道号
    返 回 值：AD输入状态
    调用位置：
    注意事项：
    */
    uBit32 ADDA_GetADStatus(uBit32 ulADDANO, uBit32 iADChNO);

    /*
    函 数 名：ADDA_GetDAStatus
    功    能：获取DA输出状态
    参    数：ulADDANO --ADDA设备编号
              iDAChNO --DA通道号
    返 回 值：DA输出状态
    调用位置：
    注意事项：
    */
    uBit32 ADDA_GetDAStatus(uBit32 ulADDANO, uBit32 iDAChNO);
#ifdef __cplusplus
}
#endif

#endif

