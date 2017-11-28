/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_ErrorCodeDef.h

摘    要：定义设备驱动管理模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：Alan

修改日期：2015年10月17日
*/

#ifndef DVM_ERROR_CODE_DEF_H
#define DVM_ERROR_CODE_DEF_H



//------------------------------定义设备管理模块指令类型-----------------------------------------
#define DVM_CMD_DRVREG                    (1)    //总线注册
#define DVM_CMD_GET_DEV_INFO            (2)    //获取设备信息
#define DVM_CMD_BLINK_LED                (3)    //设备快闪
#define DVM_CMD_SET_COFIG                (4)    //设置硬件配置信息
#define DVM_CMD_GET_DEV_VER                (5)    //获取设备软件版本
#define DVM_CMD_GET_DIV_VER                (6)    //获取驱动软件版本
#define DVM_CMD_GET_DEV_STATA            (7)    //获取设备状态
#define DVM_CMD_UPDATE_DEV_APP            (8)    //升级设备APP
#define DVM_CMD_UPDATE_DEV_BOOT            (9)    //升级设备BOOT
#define DVM_CMD_SCAN                    (10)//设备扫描
#define DVM_CMD_GET_LAST_EXE_STATE        (12)//获取最后指令执行状态
#define DVM_CMD_SEND_AXIS_PARM            (13)//发送轴控参数
#define DVM_CMD_SEND_SYS_PARM            (14)//发送系统控制参数
#define DVM_CMD_SET_AXIS_FEED            (15)//设置轴进给
#define DVM_CMD_ENABLE_AXIS                (16)//使能轴
#define DVM_CMD_HOME                    (17)//轴回零
#define DVM_CMD_LOCK_AXIS                (18)//锁定轴
#define DVM_CMD_SET_AXIS_CMDPOS            (19)//设置轴当前位置
#define DVM_CMD_SET_AXIS_QEIPOS            (20)//设置轴编码器位置
#define DVM_CMD_SET_AXIS_VERIFY            (21)//设置校验模式
#define DVM_CMD_GET_AXIS_STATE            (22)//获取轴卡状态
#define DVM_CMD_SET_IO_OUTPUT            (23)//设置IO输出
#define DVM_CMD_SET_IO_PWM_MODE            (24)//设置IO口PWM模式
#define DVM_CMD_SET_IO_PWM_DUTY            (25)//设置IO口PWM占空比
#define DVM_CMD_SET_IO_PWM_FREQ            (26)//设置IO口PWM频率
#define DVM_CMD_GET_IO_STATE            (27)//获取IO卡状态
#define DVM_CMD_SET_ADDA_DA                (28)//获取DA输出状态
#define DVM_CMD_GET_ADDA_STATE            (29)//获取ADDA卡状态
#define DVM_CMD_RESET                    (30)//轴复位
#define DVM_CMD_SEND_SV_PARM            (31)//发送伺服参数
#define DVM_CMD_GET_SV_PARM                (32)//获取伺服参数


//------------------------------定义设备管理模块错误编号-----------------------------------------
#define DVM_ERR_SUCCESS                        (0)
#define DVM_ERR_UNKNOW_BUS                    (1)//未知总线
#define DVM_ERR_UNKNOW_DEV                    (2)//不认识的设备
#define DVM_ERR_UNKNOW_CMD                    (3)//无法识别的指令
#define DVM_ERR_DIV_LIMIT                    (4)//注册驱动超过系统允许的最大驱动数
#define DVM_ERR_CONF_DISMATCH                (5)//硬件配置信息与实际不匹配
#define DVM_ERR_CMD_SEND                    (6)//指令发送失败
#define DVM_ERR_RCV_TIMEOUT                    (7)//等待回应超时
#define DVM_ERR_EXECUTTING                    (8)//指令正在执行
#define DVM_ERR_BUFFER                        (9)//存储缓冲区错误（未指定设备信息、状态等首地址，无法再存储多余的数据）
#define DVM_ERR_INVALID_CH                    (10)//无效的总线通道（通道未打开）
#define DVM_ERR_INVALID_FRAME                (11)//非法包(过时数据包)
#define DVM_ERR_BUS_BUSY                    (12)//总线忙(通道被锁定、正在执行其他指令[等待指令回应包])
#define DVM_ERR_PULSE_RANGE                    (13)//单周期脉冲超限
#define DVM_ERR_ASKFOR_RS_TIMEOUT            (14)//请求发送权限响应超时(系统运行不正常，导致中断不能正常执行)
#define DVM_ERR_BUS_INIT_FAIL                (15)//总线初始化失败
#define DVM_ERR_DEVNO_DISMATCH                (16)//设备编号不匹配
#define DVM_ERR_DEV_OFFLINE                    (17)//设备掉线
#define DVM_ERR_UNDEF_FUN                    (18)//未定义接口
#define DVM_ERR_MOTOR_DISABLE                (19)//电机未使能

//设备端错误-------------------                                    
#define DVM_ERR_INVALID_CMD                    (20)//设备不认识的指令
#define DVM_ERR_RCV_OVERFLOW                (21)//设备端接收缓冲区溢出
#define DVM_ERR_WRONG_DATA                    (22)//数据错误
#define DVM_ERR_PACK_LOST                    (23)//设备端数据包存在丢失
#define DVM_ERR_RCV_CHECK                    (24)//设备端接收数据校验出错
#define DVM_ERR_FLASH_W_FAIL                (25)//设备端写FLASH出错
#define DVM_ERR_FEED_WHILE_HOME                (26)//轴回零时收到进给指令
#define DVM_ERR_SETPOS_WHILE_HOME            (27)//轴回零时收到设置位置指令
#define DVM_ERR_SETPOS_WHILE_MOVE            (28)//轴运动时收到设置位置指令
#define DVM_ERR_UPDATE_FILE_CHECK            (29)//升级文件校验错误
#define DVM_ERR_PLUSE_BUFFER_FULL            (30)//脉冲缓冲区满
#define DVM_ERR_DEV_BUSY                    (31)//设备忙
#define DVM_ERR_JUMPTO_BOOT                    (32)//正在跳转到BOOTLOADER
#define DVM_ERR_UPDATE_FINISHED                (33)//升级完毕
#define DVM_ERR_DISABLE_AXIS_FEED            (34)//轴收到进给数据但轴没有使能

//MIII常规错误
#define DVM_ERR_MIII_RAM_CHECK                (40)//MIII总线ASICRAM检查出错    
#define DVM_ERR_MIII_ASIC_SETUP                (41)//MIII总线ASIC设置失败
#define DVM_ERR_MIII_D_ALRM                    (42)//设备报警
#define DVM_ERR_MIII_D_WAR                    (43)//设备警告
#define DVM_ERR_MIII_CMD_UNALLOWED            (44)//不满足执行条件
#define DVM_ERR_MIII_PHASE                    (46)//层错误
#define DVM_ERR_MIII_FCS                    (47)//FCS错误
#define DVM_ERR_MIII_UNRCV_INQ                (48)//没有接收到指定数据
#define DVM_ERR_MIII_UNRCV_SYNC                (49)//没有接收到同步帧
#define DVM_ERR_MIII_SYNC_GAP                (50)//同步间隔故障
#define DVM_ERR_MIII_SYNC_WDT                (51)//WDT故障

//MIII总线初始化错误
#define DVM_ERR_MIII_INVALID_USRPAR            (0X40)//MIII无效用户参数
#define DVM_ERR_MIII_TOUT_MODE_SET            (DVM_ERR_MIII_INVALID_USRPAR|0x01)//设置MIII通信超时
#define DVM_ERR_MIII_TOUT_USRPRM_SET        (DVM_ERR_MIII_INVALID_USRPAR|0x02)//设置MIII用户参数超时
#define DVM_ERR_MIII_REQ_SIZE_OVER            (DVM_ERR_MIII_INVALID_USRPAR|0x03)//请求数据超限
#define DVM_ERR_MIII_INVALID_HIFPTR            (DVM_ERR_MIII_INVALID_USRPAR|0x04)//无效HIF
#define DVM_ERR_MIII_RAM_SIZE_OVER            (DVM_ERR_MIII_INVALID_USRPAR|0x05)//RAM空间不足
#define DVM_ERR_MIII_INVALID_TMCYC            (DVM_ERR_MIII_INVALID_USRPAR|0x06)//无效通信周期
#define DVM_ERR_MIII_TOUT_CSTART            (DVM_ERR_MIII_INVALID_USRPAR|0x07)//请求循环通信未完成
#define DVM_ERR_MIII_TOUT_ASTART            (DVM_ERR_MIII_INVALID_USRPAR|0x08)//请求非循环通信未完成
#define DVM_ERR_MIII_INFM_CMODE                (DVM_ERR_MIII_INVALID_USRPAR|0x09)//模式请求未完成
#define DVM_ERR_MIII_WRONG_SEQ                (DVM_ERR_MIII_INVALID_USRPAR|0x0a)//执行顺序错误
#define DVM_ERR_MIII_SLV_AXIS_NO            (DVM_ERR_MIII_INVALID_USRPAR|0x0b)//从站编号重复
#define DVM_ERR_MIII_IOMAP_SIZE                (DVM_ERR_MIII_INVALID_USRPAR|0x0c)//IOMAP空间错误
#define DVM_ERR_MIII_VERIFY_MICRO            (DVM_ERR_MIII_INVALID_USRPAR|0x0d)//MICRO下载未完成
#define DVM_ERR_MIII_PHY_RESET                (DVM_ERR_MIII_INVALID_USRPAR|0x0e)//PHY复位出错
#define DVM_ERR_MIII_RESET_ASIC                (DVM_ERR_MIII_INVALID_USRPAR|0x0f)//ASIC复位出错
#define DVM_ERR_MIII_TX_FRM                    (DVM_ERR_MIII_INVALID_USRPAR|0x10)//帧发送出错

//MIII总线通信连接错误
#define DVM_ERR_MIII_TOUT_CHANG_RBUFF        (0X60)                                                  //交换接收缓冲区未完成
#define DVM_ERR_MIII_TOUT_CHANG_CBUFF        (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x01)//交换指令缓冲区未完成
#define DVM_ERR_MIII_INVALID_STNO            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x02)//从站不存在
#define DVM_ERR_MIII_REQ_SIZE                (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x03)//请求数据超RAM区
#define DVM_ERR_MIII_INVALID_SIZE            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x04)//请求空间无效
#define DVM_ERR_MIII_INVALID_FTYPE            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x05)//无效帧类型
#define DVM_ERR_MIII_TOUT_RCV_FRAME            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x06)//接收帧超时
#define DVM_ERR_MIII_TOUT_SEND_FRAME        (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x07)//发送帧超时
#define DVM_ERR_MIII_SEND_SYNCFRAM            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x08)//接收同步帧超时
#define DVM_ERR_MIII_MDIO_BUSY                (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x09)//MDIO BUSY
#define DVM_ERR_MIII_NOT_SYNC                (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x0a)//非同步模式
#define DVM_ERR_MIII_GBL_NOT_RCV            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x0b)//未知错误
#define DVM_ERR_MIII_TOUT_CMD_GTX            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x0c)//未知错误
#define DVM_ERR_MIII_ASYNC_FTYPE            (DVM_ERR_MIII_TOUT_CHANG_RBUFF|0x0d)//非同步类型错误


#define DVM_ERR_UNKNOW                        (255)//未知错误

//--------------------定义设备错误码--------------------------------------
//说明：设备端只传错误号上来，由总线设备驱动模块组装错误码

#endif

