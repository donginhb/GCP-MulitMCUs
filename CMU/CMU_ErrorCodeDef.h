/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CMU_ErrorCodeDef.h

摘    要：定义下位机通信模块错误码

运行环境： LPC43xx / WINXP / WINCE

修改描述：

当前版本：

修改作者：Alan

修改日期：2015年9月17日
*/

#ifndef CMU_ERROR_CODE_DEF_H
#define CMU_ERROR_CODE_DEF_H


#pragma pack(push)
#pragma pack(2)    

//---------------下位机错误指令类型定义-------------------------
//同UIN模块指令定义相同



//--------------下位机通信模块错误编号定义-------------------
#define CMU_ERR_SUCCESS                     (0)     //成功
#define CMU_ERR_CONNECT                     (1)     //表示通信连接出错(接收和发送抽象层硬件出错)
#define CMU_ERR_SEND_FAIL                   (2)     //发送数据失败
#define CMU_ERR_OUTMODED                    (3)     //过时的数据
#define CMU_ERR_DEV_NOT_EXIST               (4)     //设备不存在，或未编号
#define CMU_ERR_CRD_NOT_EXIST               (5)     //通道不存在
#define CMU_ERR_DATA_WRONG                  (6)     //数据错误,接口参数错误
#define CMU_ERR_SEND_OVERFLOW               (7)     //发送缓冲区溢出
#define CMU_ERR_CHECK_FAIL                  (8)     //接收数据校验错误
#define CMU_ERR_TIME_OUT                    (9)     //接收回应包超时
#define CMU_ERR_DATA_LOST                   (10)    //接收数据存在数据丢失
#define CMU_ERR_RCV_OVERFLOW                (11)    //接收缓冲区溢出
#define CMU_ERR_INVALID_CMD                 (12)    //非法指令
#define CMU_ERR_UNBAND                      (13)    //未绑定外部指令接口
#define CMU_ERR_INQ_OUTRANG                 (14)    //周期性查询数据已经到达系统允许最大值
#define CMU_ERR_GET_DATA_FAIL               (15)    //获取数据失败
#define CMU_ERR_FLASH_OPER                  (16)    //Flash操作失败
#define CMU_ERR_UPDATE_FILE_CHECK           (17)    //升级文件校验错误
#define CMU_ERR_SYS_BUSY                    (18)    //系统忙
#define CMU_ERR_IPR_ERROR                   (19)    //解释器错误
#define CMU_ERR_JUMPTO_BOOT                 (20)    //正在跳转到BOOTLOADER
#define CMU_ERR_CMD_DISMATCH                (21)    //指令执行结果检查指令不匹配
#define CMU_ERR_INVALID_COMTYPE             (22)    //无效通信方式
#define CMU_ERR_UREGNO_RANGOUT              (23)    //寄存器编号超限
#define CMU_ERR_IPO_BUSY                    (24)    //插补器不空闲
#define CMU_ERR_CSM_BUSY                    (25)    //通道不空闲
#define CMU_ERR_RS_MALLOC_FAIL              (26)    //接收缓冲区申请空间失败
#define CMU_ERR_IPR_ENABLE                  (27)    //解释器已使能

#pragma pack(pop)
#endif

