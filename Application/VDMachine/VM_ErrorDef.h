#ifndef __VM_ERROR_DEF_H
#define __VM_ERROR_DEF_H

#if 0
#define VM_ERR_SUCCEED                          (0)             //成功
#define VM_ERR_SELLING                          (1)             //正在出货 

#define VM_ERR_CONF_INVALID                     (10)            //系统硬件配置不正确
#define VM_ERR_SYS_ALARM		                (11)            //系统存在报警
#define VM_ERR_DEV_ERR			                (12)            //设备故障
#define VM_ERR_CHMOTOR_ERR		                (13)            //货道电机故障
#define VM_ERR_LIFT_MOTOR                       (14)            //升降电机故障
#define VM_ERR_DOOR_CLOSE_IO	                (15)            //门控关门信号故障
#define VM_ERR_DOOR_OPEN_IO		                (16)            //门控开门信号故障
#define VM_ERR_DOOR_MOTOR		                (17)            //门电机故障
#define VM_ERR_DELIVERY_MOTOR	                (18)            //传送带电机故障
#define VM_ERR_GOODS_CHECK_IO	                (19)            //传送带检测信号故障
#define VM_ERR_HMD_IO                           (20)            //加湿失败  
#define VM_ERR_TMP_IO                           (21)            //温控失败  
#define VM_ERR_DOOR_GOODS_CHECK_IO              (22)            //货物检测

#define VM_ERR_BUSY				                (30)            //系统忙(正在出货、复位、回收)
#define VM_ERR_WITHOUT_INQ		                (31)            //确认出货前未请求出货
#define VM_ERR_CH_INVALID		                (32)            //货道不存在
#define VM_ERR_CONNECT_ERR		                (33)            //连接错误
#define VM_ERR_INVALID_CMD                      (34)            //非法指令
#define VM_ERR_DATA_LEN_ERR		                (36)            //数据长度错误
#define VM_ERR_SOLD_CANCEL		                (37)            //出货取消
#define VM_ERR_DATA_WRONG		                (38)            //数据错误
#define VM_ERR_POWER_OFF		                (39)            //控制器掉电
#define VM_ERR_ORDER_NONEXIST	                (40)            //订单不存在
#define VM_ERR_CRD_MODE                         (41)            //坐标系模式错误
#define VM_ERR_P_LIMIT                          (42)            //系统已到正限位
#define VM_ERR_N_LIMIT                          (43)            //系统已到负限位
#define VM_ERR_INIT_UN_FINISHED                 (44)            //系统未复位或控制参数未设置
#define VM_ERR_GOODS_EXIST                      (45)            //货物未取走
#define VM_ERR_MOTOR_DISABLE                    (46)            //升降电机未使能

#define VM_ERR_SEND_PACK                        (0XFF)          //发送数据包失败

#endif

//售货机V4.0 错误码定义
#define VM_ERR_SUCCEED                          (0x00000000)    //成功
#define VM_ERR_CONF_INVALID                     (0x00000001)    //系统硬件配置不正确
#define VM_ERR_DEV_ERR			                (0x00000002)    //设备故障
#define VM_ERR_CHMOTOR_ERR		                (0x00000004)    //货道电机故障
#define VM_ERR_LIFT_MOTOR                       (0x00000008)    //升降电机故障
#define VM_ERR_CH_INVALID		                (0x00000010)    //货道不存在
#define VM_ERR_CH_CONFLICT		                (0x00000020)    //货道不存在
#define VM_ERR_CRD_MODE                         (0x00000040)    //坐标系模式错误
#define VM_ERR_P_LIMIT                          (0x00000080)    //系统已到正限位
#define VM_ERR_N_LIMIT                          (0x00000100)    //系统已到负限位
#define VM_ERR_INIT_UN_FINISHED                 (0x00000200)    //系统未复位或控制参数未设置


#endif /* __VM_ERROR_DEF_H */
