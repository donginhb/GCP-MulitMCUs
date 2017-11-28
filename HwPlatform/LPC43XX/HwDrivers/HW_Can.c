/**
  ******************************************************************************
  * @file    hw_can.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.07.21
  * @brief   CAN drivers
  ******************************************************************************
  * @attention
  * 
  * 引脚,
  *     CAN1_TD  --- P0_1(Fun1),  P0_22(Fun3),
  *     CAN1_RD  --- P0_0(Fun1),  P0_21(Fun3),
  *                  
  *     CAN2_TD  --- P0_5(Fun2),  P2_8(Fun1),
  *     CAN2_RD  --- P0_4(Fun2),  P2_7(Fun1),
  * 
  * V1.1------------
  * 修改描述: 为CAN的发送函数增加超时操作
  * 修改原因: 如果当前CAN总线没有连接外挂设备的时候,调用CAN发送函数,会造成发送阻
  *           塞,从而导致系统卡死.为避免这种情况,为发送函数增加发送超时退出的功能.
  * 修改作者: Duhanfneg
  * 当前版本: V1.1
  * 修改日期: 2017.09.05
  * 
  * 需要补充的功能: 按照位的ID过滤
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Can.h"
#include "HW_SysTick.h"
#include "chip.h"
#include <string.h>
#include <stdio.h>

#pragma diag_suppress=Pe177 //屏蔽以下static函数已定义但未使用的警告


/*****************************************************************************
 * 私有类型/变量定义
 ****************************************************************************/
static const uint32_t CAN_BASE[HW_CAN_NODE_NUM] = {LPC_CAN1_BASE, LPC_CAN1_BASE};  //CAN基地址


/**
  * @brief  CAN_IO_MUX初始化(选定某组引脚作为CAN的引脚).
  * @param  CAN 外设指针
  * @retval None
  */
static void HW_CAN_IO_Init(uint8_t uCanNode)
{
    switch (uCanNode)
    {
    case HW_CAN_NODE0: 
#if 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  1, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  0, IOCON_MODE_INACT, IOCON_FUNC1);
#else
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 22, IOCON_MODE_INACT, IOCON_FUNC3);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 21, IOCON_MODE_INACT, IOCON_FUNC3);
#endif
        break;
    case HW_CAN_NODE1:
#if 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  5, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  4, IOCON_MODE_INACT, IOCON_FUNC2);
#else
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  8, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  7, IOCON_MODE_INACT, IOCON_FUNC1);
#endif
        break;
    default : break;
    }

}


/*****************************************************************************
 * CAN 基本功能接口函数
 ****************************************************************************/

/**
  * @brief  CAN总线初始化
  * @param  uCanNode CAN节点号
  * @param  ulBitRate  波特率
  * @retval None
  */
void HW_CAN_Init(uint8_t uCanNode, uint32_t ulBitRate)
{
    //初始化IO
    HW_CAN_IO_Init(uCanNode);
    
    //初始化模块
    Chip_CAN_Init((LPC_CAN_T *)&CAN_BASE[uCanNode], LPC_CANAF, LPC_CANAF_RAM);
    
    //设置比特率
    Chip_CAN_SetBitRate((LPC_CAN_T *)&CAN_BASE[uCanNode], ulBitRate); 
    
    //设置中断模式
    Chip_CAN_EnableInt((LPC_CAN_T *)&CAN_BASE[uCanNode], 0);
    
    //设置过滤器
    HW_CAN_ResetRecvIDGroup();
    Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_BYBASS_MODE);
    
}


/**
  * @brief  数据发送
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_SendMsg(uint8_t uCanNode, CAN_MSG_T *pMsgObj)
{
    CAN_BUFFER_ID_T TxBuf = Chip_CAN_GetFreeTxBuf((LPC_CAN_T *)&CAN_BASE[uCanNode]);
    Chip_CAN_Send((LPC_CAN_T *)&CAN_BASE[uCanNode], TxBuf, pMsgObj);
    
    uint32_t ulTick = HW_SysTick_GetCount();
    
    //等待发送完成
    while (!(Chip_CAN_GetStatus((LPC_CAN_T *)&CAN_BASE[uCanNode]) & CAN_SR_TCS(TxBuf)))
    {
        //发送超时
        if ((HW_SysTick_GetCount() - ulTick) >= 200)
        {
            return 1;
        }
    }
    
    return 0;
}


/**
  * @brief  数据接收
  * @param  uCanNode CAN节点号
  * @param  pMsgObj 消息对象变量
  * @retval 0-成功 1-失败
  */
uint32_t HW_CAN_RecvMsg(uint8_t uCanNode, CAN_MSG_T *pMsgObj)
{
    uint32_t IntStatus = Chip_CAN_GetGlobalStatus((LPC_CAN_T *)&CAN_BASE[uCanNode]);
    
    if (IntStatus & CAN_GSR_RBS) 
    {
        Chip_CAN_Receive((LPC_CAN_T *)&CAN_BASE[uCanNode], pMsgObj);
        return 0;
    }
    
    return 1;
}


/*****************************************************************************
 * CAN过滤器私有类型/变量定义
 ****************************************************************************/

static CAN_STD_ID_RANGE_ENTRY_T SffGrpSection[HW_CANAF_MAX_ID_GROUP] = {0};    //标准帧过滤组
static CAN_EXT_ID_RANGE_ENTRY_T EffGrpSection[HW_CANAF_MAX_ID_GROUP] = {0};    //扩展帧过滤组

//CAN过滤器变量
static CANAF_LUT_T AFSections = 
{
    NULL, 0,
    NULL, 0,
    SffGrpSection, 1,
    NULL, 0,
    EffGrpSection, 1,
};

/**
  * @brief  过滤器设置
  * @param  pAFSections 过滤器结构变量指针
  * @retval None
  */
static void HW_CAN_SetupAFLUT(CANAF_LUT_T *pAFSections)
{
    Chip_CAN_SetAFLUT(LPC_CANAF, LPC_CANAF_RAM, pAFSections);
    Chip_CAN_SetAFMode(LPC_CANAF, CAN_AF_NORMAL_MODE);
    
}


/*****************************************************************************
 * CAN过滤器接口函数
 ****************************************************************************/

/**
  * @brief  CAN ID 接收过滤器组设置
  * @param  uCanNode CAN节点号
  * @param  GrpEntry 过滤器组
  * @retval None
  */
void HW_CAN_SetupRecvIDGroup(uint8_t uCanNode, HW_CANAF_ID_GROUP_T *GrpEntry)
{
    GrpEntry->nEntryNum = (GrpEntry->nEntryNum > HW_CANAF_MAX_ID_GROUP) ? HW_CANAF_MAX_ID_GROUP : GrpEntry->nEntryNum;
    
    if (GrpEntry->nFrameMode == HW_CANAF_ID_EXT_FRAME)   //扩展帧
    {
        for (int i = 0; i < GrpEntry->nEntryNum; i++)
        {
            EffGrpSection[i].LowerID.CtrlNo = uCanNode;
            EffGrpSection[i].LowerID.ID_29  = GrpEntry->IDRange[i].ulLowerID & CAN_EXT_ENTRY_ID_MASK;
            
            EffGrpSection[i].UpperID.CtrlNo = uCanNode;
            EffGrpSection[i].UpperID.ID_29  = GrpEntry->IDRange[i].ulUpperID & CAN_EXT_ENTRY_ID_MASK;
        }
        
        AFSections.EffGrpEntryNum = GrpEntry->nEntryNum;
    }
    else    //数据帧
    {
        for (int i = 0; i < GrpEntry->nEntryNum; i++)
        {
            SffGrpSection[i].LowerID.CtrlNo  = uCanNode;
            SffGrpSection[i].LowerID.Disable = 0;
            SffGrpSection[i].LowerID.ID_11   = GrpEntry->IDRange[i].ulLowerID & CAN_STD_ENTRY_ID_MASK;
            
            SffGrpSection[i].UpperID.CtrlNo  = uCanNode;
            SffGrpSection[i].UpperID.Disable = 0;
            SffGrpSection[i].UpperID.ID_11   = GrpEntry->IDRange[i].ulUpperID & CAN_STD_ENTRY_ID_MASK;
        }
        
        AFSections.SffGrpEntryNum = GrpEntry->nEntryNum;
    }
    
    HW_CAN_SetupAFLUT(&AFSections);
    
}


/**
  * @brief  CAN ID 接收ID过滤器复位
  * @param  None
  * @retval None
  */
void HW_CAN_ResetRecvIDGroup(void)
{
    memset(SffGrpSection, 0, sizeof(CAN_STD_ID_RANGE_ENTRY_T) * HW_CANAF_MAX_ID_GROUP);
    memset(EffGrpSection, 0, sizeof(CAN_EXT_ID_RANGE_ENTRY_T) * HW_CANAF_MAX_ID_GROUP);
    AFSections.EffGrpEntryNum = 1;
    AFSections.SffGrpEntryNum = 1;
    
    HW_CAN_SetupAFLUT(&AFSections);
    
}


/*****************************************************************************
 * CAN DEBUG 
 ****************************************************************************/

#if 0

#define CAN_DEBUG_PRINT(...) printf(__VA_ARGS__)


/**
  * @brief  CAN 消息对象打印
  * @param  pMsg CAN消息结构
  * @retval None
  */
void CAN_Debug_PrintCANMsg(CAN_MSG_T *pMsg)
{
    uint8_t i;
    CAN_DEBUG_PRINT("\t**************************\r\n");
    CAN_DEBUG_PRINT("\tMessage Information: \r\n");
    CAN_DEBUG_PRINT("\tMessage Type :");
    if (pMsg->ID & CAN_EXTEND_ID_USAGE) {
        CAN_DEBUG_PRINT("Extend ID Message");
    }
    else {
        CAN_DEBUG_PRINT("Standard ID Message");
    }
    if (pMsg->Type & CAN_REMOTE_MSG) {
        CAN_DEBUG_PRINT(", Remote Message");
    }
    CAN_DEBUG_PRINT("\r\n");
    CAN_DEBUG_PRINT("\tMessage ID   :0x%.2X\r\n", (pMsg->ID & (~CAN_EXTEND_ID_USAGE)));
    CAN_DEBUG_PRINT("\tMessage Data :");
    for (i = 0; i < pMsg->DLC; i++)
        CAN_DEBUG_PRINT("0x%.2X ", pMsg->Data[i]);
    CAN_DEBUG_PRINT("\r\n\t**************************\r\n");
}


/**
  * @brief  CAN 硬件过滤器过滤配置打印
  * @param  None
  * @retval None
  */
void CAN_Debug_PrintAFLUT(void)
{
    uint16_t i, num;
    CAN_STD_ID_ENTRY_T StdEntry;
    CAN_EXT_ID_ENTRY_T ExtEntry;
    CAN_STD_ID_RANGE_ENTRY_T StdGrpEntry;
    CAN_EXT_ID_RANGE_ENTRY_T ExtGrpEntry;
    
    /* Standard ID Table */
    CAN_DEBUG_PRINT("Individual Standard ID Table: \r\n");
    num = Chip_CAN_GetEntriesNum(LPC_CANAF, LPC_CANAF_RAM, CANAF_RAM_SFF_SEC);
    for (i = 0; i < num; i++) {
        Chip_CAN_ReadSTDEntry(LPC_CANAF, LPC_CANAF_RAM, i, &StdEntry);
        CAN_DEBUG_PRINT("\t%d: Controller ID: %d, ID: 0x%x, Dis: %1d\r\n",
                 i, StdEntry.CtrlNo, StdEntry.ID_11, StdEntry.Disable);
    }
    
    /* Group Standard ID Table */
    CAN_DEBUG_PRINT("Group Standard ID Table: \r\n");
    num = Chip_CAN_GetEntriesNum(LPC_CANAF, LPC_CANAF_RAM, CANAF_RAM_SFF_GRP_SEC);
    for (i = 0; i < num; i++) {
        Chip_CAN_ReadGroupSTDEntry(LPC_CANAF, LPC_CANAF_RAM, i, &StdGrpEntry);
        CAN_DEBUG_PRINT("\t%d: Controller ID: %d, ID: 0x%x-0x%x, Dis: %1d\r\n",
                 i, StdGrpEntry.LowerID.CtrlNo, StdGrpEntry.LowerID.ID_11,
                 StdGrpEntry.UpperID.ID_11, StdGrpEntry.LowerID.Disable);
    }
    
    /* Extended ID Table */
    CAN_DEBUG_PRINT("Extended ID Table: \r\n");
    num = Chip_CAN_GetEntriesNum(LPC_CANAF, LPC_CANAF_RAM, CANAF_RAM_EFF_SEC);
    for (i = 0; i < num; i++) {
        Chip_CAN_ReadEXTEntry(LPC_CANAF, LPC_CANAF_RAM, i, &ExtEntry);
        CAN_DEBUG_PRINT("\t%d: Controller ID: %d, ID: 0x%x,\r\n",
                 i, ExtEntry.CtrlNo, ExtEntry.ID_29);
    }
    
    /* Group Extended ID Table */
    CAN_DEBUG_PRINT("Group Extended ID Table: \r\n");
    num = Chip_CAN_GetEntriesNum(LPC_CANAF, LPC_CANAF_RAM, CANAF_RAM_EFF_GRP_SEC);
    for (i = 0; i < num; i++) {
        Chip_CAN_ReadGroupEXTEntry(LPC_CANAF, LPC_CANAF_RAM, i, &ExtGrpEntry);
        CAN_DEBUG_PRINT("\t%d: Controller ID: %d, ID: 0x%x-0x%x\r\n",
                 i, ExtGrpEntry.LowerID.CtrlNo, ExtGrpEntry.LowerID.ID_29,
                 ExtGrpEntry.UpperID.ID_29);
    }

}
#endif
