/**
  ******************************************************************************
  * @file    GPIO_Man.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.08
  * @brief   IO State Manage
  ******************************************************************************
  * @attention
  * 
  * IO翻转: 翻转IO的电平逻辑,以true为低电平,false为高电平.在实际的应用中,
  *         有些IO设备为高电平使能,有些设备为低电平使能,我们可以将其中一
  *         类设备进行IO逻辑翻转,这样应用层就可以统一设定true为使能的电平,
  *         false为禁止的电平,不需要关心实际上所控制的电平是什么;
  *   
  * 配置:   最大支持64组输入及输出IO的情况下,使能IO管理与关闭IO管理功能,
  *         FLASH相差1K, RAM相差176;
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "GPIO_Man.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Gpio.H"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysCtrl/SysConfig.h"
#include <string.h>


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define IO_MAX_COUNT            (SYS_GPIO_SCAN_GROUP)   //最大支持的IO数
     
     
static GPIO_CTRL_TABLE  m_GpioCtrlTable = {0};          //IO资源配置表

#if SYS_GPIO_MAN_USAGE
     
#define IO_FILTER_TIME          (50)    //滤波时间(MS)
#define IO_SAMP_INTERVAL        (10)    //采样间隔(MS)
#define IO_FILTER_COUNT         (IO_FILTER_TIME/IO_SAMP_INTERVAL)   //滤波次数


#define IO_EACH_GROUP_PIN_COUNT (32)    //每组的IO数量
#define IO_MAX_GROUP            (((IO_MAX_COUNT - 1)/IO_EACH_GROUP_PIN_COUNT) + 1)   //最大支持的组数

static uBit32 m_ulInputStateGroup[IO_MAX_GROUP] = {0};      //输入IO状态

#if SYS_GPIO_OUTPUT_SCAN_USAGE
static uBit32 m_ulOutputStateGroup[IO_MAX_GROUP] = {0};     //输出IO状态
#endif

static uBit32 m_ulInputToggleGroup[IO_MAX_GROUP] = {0};     //输入IO翻转标志
static uBit32 m_ulOutputToggleGroup[IO_MAX_GROUP] = {0};    //输出IO翻转标志

static uBit8  m_uInputChangeCount[IO_MAX_GROUP][IO_EACH_GROUP_PIN_COUNT]  = {0};    //记录每个输入IO的电平改变计数

#if SYS_GPIO_OUTPUT_SCAN_USAGE
static uBit8  m_uOutputChangeCount[IO_MAX_GROUP][IO_EACH_GROUP_PIN_COUNT] = {0};    //记录每个输出IO的电平改变计数
#endif

static SYS_TIME_DATA m_IOUpdateTimer = {1};     //IO更新定时器

static uBit32 GPIO_MAN_BuffInit(void);

#endif



/*****************************************************************************
 * IO配置相关接口
 ****************************************************************************/

/**
  * @brief  IO资源配置表设置
  * @param  pTable IO资源配置表指针
  * @retval 0-成功  非0-失败
  */
uBit32 GPIO_SetCtrlTable(GPIO_CTRL_TABLE *pTable)
{
    if ((pTable != NULL) && (pTable->ulInputGroupLen <= IO_MAX_COUNT) && (pTable->ulOutputGroupLen <= IO_MAX_COUNT))
    {
        m_GpioCtrlTable = *pTable;
        
        return 0;
    }
    
    return 1;
}


/**
  * @brief  IO组初始化
  * @param  pIOGroup IO组指针
  * @param  ulGroupLen 组长度
  * @param  CfgMode  要配置的IO模式
  * @retval 0-成功  非0-失败
  */
uBit32 GPIO_InitIOGroup(GPIO_DATA *pIOGroup, uBit32 ulGroupLen, GPIO_CFG_MODE CfgMode)
{
    switch (CfgMode)
    {
    case GOIO_MODE_INPUT:
        {
            for (int i = 0; i < ulGroupLen; i++)
            {
                HAL_GPIO_ConfigInput(pIOGroup[i].nPort, pIOGroup[i].nPin);
            }
            
            break;
        }
    case GPIO_MODE_OUPUT:
        {
            for (int i = 0; i < ulGroupLen; i++)
            {
                HAL_GPIO_ConfigOutput(pIOGroup[i].nPort, pIOGroup[i].nPin);
            }
            
            break;
        }
    case GPIO_MODE_OD:
        {
            for (int i = 0; i < ulGroupLen; i++)
            {
                HAL_GPIO_ConfigOD(pIOGroup[i].nPort, pIOGroup[i].nPin);
            }
            
            break;
        }
        
    default: break;
    }
    
    return 0;
}


/**
  * @brief  配置表IO初始化
  * @param  pTable IO资源配置表指针
  * @retval 0-成功  非0-失败
  * @note   此函数会把输出组的IO设置成输出模式,输入组的IO设置为输入模式;若要配置
  *         成开漏模式,需在执行本函数后另行配置(调用GPIO_InitIOGroup);
  */
uBit32 GPIO_InitIOTable(GPIO_CTRL_TABLE *pTable)
{
    if (pTable == NULL)
    {
        return 1;
    }
    
    //配置输出IO
    for (int i = 0; i < pTable->ulOutputGroupLen; i++)
    {
        HAL_GPIO_ConfigOutput(pTable->pOutputGroup[i].nPort, pTable->pOutputGroup[i].nPin);
    }
    
    //配置输入IO
    for (int i = 0; i < pTable->ulInputGroupLen; i++)
    {
        HAL_GPIO_ConfigInput(pTable->pInputGroup[i].nPort, pTable->pInputGroup[i].nPin);
    }
    
#if SYS_GPIO_MAN_USAGE
    GPIO_MAN_BuffInit();
#endif
    
    return 0;
}


/*****************************************************************************
 * 基本IO控制接口
 ****************************************************************************/

/**
  * @brief  IO输出状态设置
  * @param  ulOutputNO 输出IO编号,从0算起
  * @param  bState     要设置的状态
  * @retval None
  */
void GPIO_SetOutputState(uBit32 ulOutputNO, bool bState)
{
    if (ulOutputNO <= m_GpioCtrlTable.ulOutputGroupLen)
    {
        HAL_GPIO_SetOutputState(m_GpioCtrlTable.pOutputGroup[ulOutputNO].nPort,
                                m_GpioCtrlTable.pOutputGroup[ulOutputNO].nPin,
                                bState);
    }
    
}


/**
  * @brief  IO输出状态翻转
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval None
  */
void GPIO_ToggleOutputState(uBit32 ulOutputNO)
{
    GPIO_SetOutputState(ulOutputNO, !GPIO_GetOutputState(ulOutputNO));
    
}


/**
  * @brief  IO 输出端口状态获取
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval IO状态
  */
bool GPIO_GetOutputState(uBit32 ulOutputNO)
{
    if (ulOutputNO <= m_GpioCtrlTable.ulOutputGroupLen)
    {
        return HAL_GPIO_GetIOState(m_GpioCtrlTable.pOutputGroup[ulOutputNO].nPort,
                                   m_GpioCtrlTable.pOutputGroup[ulOutputNO].nPin);
    }
    
    return false;
}


/**
  * @brief  IO 输入端口状态获取
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval IO状态
  */
bool GPIO_GetInputState(uBit32 ulInputNO)
{
    if (ulInputNO <= m_GpioCtrlTable.ulInputGroupLen)
    {
        return HAL_GPIO_GetIOState(m_GpioCtrlTable.pInputGroup[ulInputNO].nPort,
                                   m_GpioCtrlTable.pInputGroup[ulInputNO].nPin);
    }
    
    return false;
}


/*****************************************************************************
 * IO管理初始化接口
 ****************************************************************************/

#if SYS_GPIO_MAN_USAGE
/**
  * @brief  IO管理的Buff刷新
  * @param  None
  * @retval 0-成功 非0-失败
  */
static uBit32 GPIO_MAN_BuffInit(void)
{
    //小段缓冲时间以保证IO电平稳定
    SysTime_SimpleDelay(20000);
    
    //刷新IO缓冲区状态
    
    //刷新输入IOBuff
    for (int i = 0; i < m_GpioCtrlTable.ulInputGroupLen; i++)
    {
        m_ulInputStateGroup[i/IO_EACH_GROUP_PIN_COUNT] |= GPIO_GetInputState(i) << (i%IO_EACH_GROUP_PIN_COUNT);
    }
    
#if SYS_GPIO_OUTPUT_SCAN_USAGE
    //刷新输出IOBuff
    for (int i = 0; i < m_GpioCtrlTable.ulOutputGroupLen; i++)
    {
        m_ulOutputStateGroup[i/IO_EACH_GROUP_PIN_COUNT] = GPIO_GetOutputState(i) << (i%IO_EACH_GROUP_PIN_COUNT);
    }
#endif
    
    return 0;
}

#endif

/*****************************************************************************
 * 输入IO管理接口
 ****************************************************************************/

/**
  * @brief  输入引脚翻转状态设置
  * @param  uInputNO 输入IO编号(从0算起)
  * @param  bState (true-对应的IO进行状态翻转, false-不处理)
  * @retval None
  * @note   IO翻转: 翻转IO的电平逻辑,以true为低电平,false为高电平.在实际的应用中,
  *                 有些IO设备为高电平使能,有些设备为低电平使能,我们可以将其中一
  *                 类设备进行IO逻辑翻转,这样应用层就可以统一设定true为使能的电平,
  *                 false为禁止的电平,不需要关心实际上所控制的电平是什么
  */
void GPIO_MAN_SetInputPinLogicToggle(uBit32 ulIntputNO, bool bState)
{
#if SYS_GPIO_MAN_USAGE
    if (bState)
    {
        m_ulInputToggleGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] |=  ((bState) << (ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    }
    else 
    {
        m_ulInputToggleGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] &= ~((bState) << (ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    }
    
#endif
    
}


/**
  * @brief  输入引脚状态获取
  * @param  uInputNO 输入IO编号(从0算起)
  * @retval 引脚状态(true为有效电平, false为无效电平)
  */
bool GPIO_MAN_GetInputPinState(uBit32 ulIntputNO)
{
#if SYS_GPIO_MAN_USAGE
    
    //获取对应IO的状态
    uBit32 ulPinBit = m_ulInputStateGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    
    //根据m_ulInputToggleGroup变量决定是否需要翻转逻辑
    ulPinBit ^= m_ulInputToggleGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    
    //布尔化数据
    return (ulPinBit != 0);
    
#else 
    
    return GPIO_GetInputState(ulIntputNO);
#endif
}


/*****************************************************************************
 * 输出IO管理接口
 ****************************************************************************/

/**
  * @brief  输出引脚翻转状态设置
  * @param  uInputNO 输入IO编号(从0算起)
  * @param  bState 翻转状态(true-对应的IO进行状态翻转, false-不处理)
  * @retval None
  * @note   IO翻转: 翻转IO的电平逻辑,以true为低电平,false为高电平.在实际的应用中,
  *                 有些IO设备为高电平使能,有些设备为低电平使能,我们可以将其中一
  *                 类设备进行IO逻辑翻转,这样应用层就可以统一设定true为使能的电平,
  *                 false为禁止的电平,不需要关心实际上所控制的电平是什么
  */
void GPIO_MAN_SetOutputPinLogicToggle(uBit32 ulOutputNO, bool bState)
{
#if SYS_GPIO_MAN_USAGE
    if (bState)
    {
        m_ulOutputToggleGroup[ulOutputNO/IO_EACH_GROUP_PIN_COUNT] |=  ((bState) << (ulOutputNO%IO_EACH_GROUP_PIN_COUNT));
    }
    else 
    {
        m_ulOutputToggleGroup[ulOutputNO/IO_EACH_GROUP_PIN_COUNT] &= ~((bState) << (ulOutputNO%IO_EACH_GROUP_PIN_COUNT));
    }
    
#endif
    
}


/**
  * @brief  输出引脚状态获取
  * @param  uOutputNO 输出IO编号(从0算起)
  * @retval 引脚状态(true为有效电平, false为无效电平)
  */
bool GPIO_MAN_GetOutputPinState(uBit32 ulIntputNO)
{
#if SYS_GPIO_MAN_USAGE
#if SYS_GPIO_OUTPUT_SCAN_USAGE
    //获取对应IO的状态
    uBit32 ulPinBit = m_ulOutputStateGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    
    //根据m_ulOutputToggleGroup变量决定是否需要翻转逻辑
    ulPinBit ^= m_ulOutputToggleGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(ulIntputNO%IO_EACH_GROUP_PIN_COUNT));

#else 
    
    //获取对应IO的状态
    uBit32 ulPinBit = GPIO_GetOutputState(ulIntputNO);
    
    //根据m_ulOutputToggleGroup变量决定是否需要翻转逻辑
    ulPinBit ^= m_ulOutputToggleGroup[ulIntputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(ulIntputNO%IO_EACH_GROUP_PIN_COUNT));
    
#endif //SYS_GPIO_OUTPUT_SCAN_USAGE
    
    //布尔化数据
    return (ulPinBit != 0);
    
#else 
    
    return GPIO_GetOutputState(ulIntputNO);
#endif
}


/**
  * @brief  输出引脚状态设置
  * @param  uOutputNO 输出IO编号(从0算起)
  * @param  uState    IO状态,true为有效电平,false是无效电平
  * @retval None
  */
void GPIO_MAN_SetOutputPinState(uBit32 uOutputNO, bool bState)
{
#if SYS_GPIO_MAN_USAGE
    if (m_ulOutputToggleGroup[uOutputNO/IO_EACH_GROUP_PIN_COUNT] & (0x1<<(uOutputNO%IO_EACH_GROUP_PIN_COUNT)))
    {
        bState = !bState;
    }
    
    GPIO_SetOutputState(uOutputNO, bState);
    
#else 
    GPIO_SetOutputState(uOutputNO, bState);
#endif
    
}


/*****************************************************************************
 * IO状态刷新线程
 ****************************************************************************/

#if SYS_GPIO_MAN_USAGE

/**
  * @brief  输入IO状态更新
  * @param  None
  * @retval None
  */
static void GPIO_MAN_UpdateInputIOState(void)
{
    uBit32 ulNewInputStateGroup[IO_MAX_GROUP] = {0};
    
    if ((m_GpioCtrlTable.ulInputGroupLen == 0) || (m_GpioCtrlTable.pInputGroup == NULL))
    {
        return ;
    }
    
    //获取最新的IO状态
    for (int iPinIndex = 0; iPinIndex < m_GpioCtrlTable.ulInputGroupLen; iPinIndex++)
    {
        ulNewInputStateGroup[iPinIndex/IO_EACH_GROUP_PIN_COUNT] |= GPIO_GetInputState(iPinIndex) << (iPinIndex%IO_EACH_GROUP_PIN_COUNT);
    }
    
    //状态滤波
    for (int iGrpIndex = 0; iGrpIndex < ((m_GpioCtrlTable.ulInputGroupLen - 1)/IO_EACH_GROUP_PIN_COUNT + 1); iGrpIndex++)
    {
        //假如当前组状态无变化,则整组的滤波计数清空;若有变化,则对每一位进行比较
        if (ulNewInputStateGroup[iGrpIndex] == m_ulInputStateGroup[iGrpIndex])
        {
            //清空IO的电平改变计数
            memset(&m_uInputChangeCount[iGrpIndex], 0, IO_EACH_GROUP_PIN_COUNT);
            
            continue;
        }
        else
        {
            //计算当前组有效引脚的数量
            int iCurGroupPinCount = 0;  
            if (iGrpIndex == ((m_GpioCtrlTable.ulInputGroupLen - 1)/IO_EACH_GROUP_PIN_COUNT))
            {
                iCurGroupPinCount = m_GpioCtrlTable.ulInputGroupLen % IO_EACH_GROUP_PIN_COUNT;
            }
            else 
            {
                iCurGroupPinCount = IO_EACH_GROUP_PIN_COUNT;
            }
            
            //对当前组的每一位进行比较,假如对应的IO改变,则对应的计数+1
            for (int iPinIndex = 0; iPinIndex < iCurGroupPinCount; iPinIndex++)
            {
                if ((ulNewInputStateGroup[iGrpIndex] & (0x1<<iPinIndex)) == (m_ulInputStateGroup[iGrpIndex] & (0x1<<iPinIndex)))
                {
                    m_uInputChangeCount[iGrpIndex][iPinIndex] = 0;
                }
                else 
                {
                    m_uInputChangeCount[iGrpIndex][iPinIndex]++;
                    
                    //假如达到了相应的次数,则写入对应的数值
                    if (m_uInputChangeCount[iGrpIndex][iPinIndex] >= IO_FILTER_COUNT)
                    {
                        m_ulInputStateGroup[iGrpIndex] ^= (0x1<<iPinIndex);
                    }
                }
            }
        }
    }

}


#if SYS_GPIO_OUTPUT_SCAN_USAGE
/**
  * @brief  输出IO状态更新
  * @param  None
  * @retval None
  */
static void GPIO_MAN_UpdateOutputIOState(void)
{
    uBit32 ulNewOutputStateGroup[IO_MAX_GROUP] = {0};
    
    if ((m_GpioCtrlTable.ulOutputGroupLen == 0) || (m_GpioCtrlTable.pOutputGroup == NULL))
    {
        return ;
    }
    
    //获取最新的IO状态
    for (int iPinIndex = 0; iPinIndex < m_GpioCtrlTable.ulOutputGroupLen; iPinIndex++)
    {
        ulNewOutputStateGroup[iPinIndex/IO_EACH_GROUP_PIN_COUNT] |= GPIO_GetOutputState(iPinIndex) << (iPinIndex%IO_EACH_GROUP_PIN_COUNT);
    }
    
    //状态滤波
    for (int iGrpIndex = 0; iGrpIndex < ((m_GpioCtrlTable.ulOutputGroupLen - 1)/32 + 1); iGrpIndex++)
    {
        //假如当前组状态无变化,则整组的滤波计数清空;若有变化,则对每一位进行比较
        if (ulNewOutputStateGroup[iGrpIndex] == m_ulOutputStateGroup[iGrpIndex])
        {
            //清空IO的电平改变计数
            memset(&m_uOutputChangeCount[iGrpIndex], 0, IO_EACH_GROUP_PIN_COUNT);
            
            continue;
        }
        else
        {
            //计算当前组有效引脚的数量
            int iCurGroupPinCount = 0;  
            if (iGrpIndex == ((m_GpioCtrlTable.ulOutputGroupLen - 1)/IO_EACH_GROUP_PIN_COUNT))
            {
                iCurGroupPinCount = m_GpioCtrlTable.ulOutputGroupLen % IO_EACH_GROUP_PIN_COUNT;
            }
            else 
            {
                iCurGroupPinCount = IO_EACH_GROUP_PIN_COUNT;
            }
            
            //对当前组的每一位进行比较,假如对应的IO改变,则对应的计数+1
            for (int iPinIndex = 0; iPinIndex < iCurGroupPinCount; iPinIndex++)
            {
                if ((ulNewOutputStateGroup[iGrpIndex] & (0x1<<iPinIndex)) == (m_ulOutputStateGroup[iGrpIndex] & (0x1<<iPinIndex)))
                {
                    m_uOutputChangeCount[iGrpIndex][iPinIndex] = 0;
                }
                else 
                {
                    m_uOutputChangeCount[iGrpIndex][iPinIndex]++;
                    
                    //假如达到了相应的次数,则写入对应的数值
                    if (m_uOutputChangeCount[iGrpIndex][iPinIndex] >= IO_FILTER_COUNT)
                    {
                        m_ulOutputStateGroup[iGrpIndex] ^= (0x1<<iPinIndex);
                    }
                }
            }
        }
    }

}
#endif

#endif


/**
  * @brief  IO状态更新处理(放在while循环中执行)
  * @param  None
  * @retval None
  */
void GPIO_MAN_UpdateProc(void)
{
#if SYS_GPIO_MAN_USAGE
    
    if (SysTime_CheckExpiredState(&m_IOUpdateTimer))
    {
        SysTime_Start(&m_IOUpdateTimer, IO_SAMP_INTERVAL);
        
        //更新输入IO的电平状态
        GPIO_MAN_UpdateInputIOState();
        
#if SYS_GPIO_OUTPUT_SCAN_USAGE
        //更新输出IO的电平状态
        GPIO_MAN_UpdateOutputIOState();
#endif
        
    }
    
#endif
    
}


