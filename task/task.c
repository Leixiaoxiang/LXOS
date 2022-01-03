/*****************************************************************************************************
 * 文件：task.c
 * 介绍：与 task 相关外部函数接口的实现
 * 作者：lxx
 * 日期：2022/01/02
*****************************************************************************************************/
/*****************************************************************************************************
 *                                              头文件包含
*****************************************************************************************************/
#include <string.h>
#include "task.h"

/*****************************************************************************************************
 *                                              函数实现
*****************************************************************************************************/
/*****************************************************************************************************
 * 函数：_TaskStackAddrAndSizeCheck
 * 介绍：任务创建时栈的检测
 * 输入：pTcb		     任务控制块指针
 *      pStackAddr      栈的起始地址
 *      StackSize       栈的长度
 * 输出：TASK_ERR_NONE	 栈检测成功
 *      TASK_ERR_ARGS	输入参数异常，栈检测失败
*****************************************************************************************************/
static task_return_t _TaskStackAddrAndSizeCheck (pos_tcb_t   pTcb,
                                                    pcpu_add_t  pStackAddr,
                                                    uint32_t    StackSize)
{
    cpu_add_t   AlignTopSatackAddr  = 0x00;
    cpu_add_t   AlignStackAddr      = 0x00;
    /*
        对于传入的各种参数进行判断
    */
    if ((!pTcb)      \
        || (!pStackAddr)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        规定传入的 StackSize 应该是 ALGIN_BYTE 的整数倍，可以减少 cpu 访问的次数
    */
    if (!(StackSize >> ALIGN_OFFSET)) {
        os_err("%s %d StackSize too small! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }

    AlignTopSatackAddr = DOWN_ALIGN(cpu_add_t, ((cpu_add_t)pStackAddr + StackSize), ALIGN_BYTE);
    AlignStackAddr = UP_ALIGN(cpu_add_t, (cpu_add_t)pStackAddr, ALIGN_BYTE);
    if (AlignTopSatackAddr <= AlignStackAddr) {
        os_err("%s %d StackSize too small! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
#if STACK_TOP_DOWN
    pTcb->pTcbStkAddr = (cpu_add_t *)AlignTopSatackAddr;
#else
    pTcb->pTcbStkAddr = (cpu_add_t *)AlignStackAddr;
#endif
    pTcb->TcbStkSize  = AlignTopSatackAddr - AlignStackAddr;

    return(TASK_ERR_NONE);
}
/*****************************************************************************************************
 * 函数：_TaskNameCpyAndCheck
 * 介绍：任务创建时名称复制和检测
 * 输入：pTcb		     任务控制块指针
 *      pTaskName       任务的名称
 * 输出：TASK_ERR_NONE	 名称复制和检测成功
 *      TASK_ERR_ARGS	输入参数异常，复制和检测失败
*****************************************************************************************************/
static task_return_t _TaskNameCpyAndCheck (pos_tcb_t   pTcb,
                                            puchar_t  pTaskName)
{
    /*
        对于传入的各种参数进行判断
    */
    if ((!pTcb)      \
        || (!pTaskName)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        任务名称长度若超过 TASK_NAME_LENGTH 规定的长度，则截取规定的长度，剩下的丢弃
    */
    pTcb->pTcbName = pTaskName;
    if (strlen(pTaskName) >= TASK_NAME_LENGTH) {
        pTcb->pTcbName[TASK_NAME_LENGTH - 1] = '\0';
    }

    return(TASK_ERR_NONE);
}
/*****************************************************************************************************
 * 函数：_TaskExitError
 * 介绍：任务异常返回函数，理论上任务都是死循环，不会出现返回，防止出现异常的时候，可以返回到这里
 * 输入：NONA
 * 输出：NONA
*****************************************************************************************************/
static void _TaskExitError( void )
{
    for(;;);
}
/*****************************************************************************************************
 * 函数：_TaskStackInit
 * 介绍：任务栈的初始化
 * 输入：pTaskFun		 任务函数
 *      pFunArgs        任务函数的参数
 *      pTcb            任务的控制块
 * 输出：TASK_ERR_NONE	 任务栈初始化成功
 *      TASK_ERR_ARGS	输入参数异常，初始化失败
*****************************************************************************************************/
/*
    Cortex-M3 的寄存器存储形式
    ---------------------
    <     xPSR          >       Progarm status, include APSR\IPSR\EPSR.
    <                   >
    ---------------------
    <       R15         >       Program counter, instruct fetches must be halfword aligned.
    <                   >       Reset value from address 0x00000004.
    ---------------------
    <       R14         >       Link register, return information for subroutines,function call and
    <                   >       exceptions. Reset value is 0xFFFFFFFF.
    ---------------------
    <       R13         >       Stack pointer, CONTROL Bit[1] indicates use MSP(0) or PSP(1).
    <                   >       Reset value from address 0x000000000.
    ---------------------
    <       R12         >
    ---------------------
    <       R3          >
    ---------------------
    <       R2          >
    ---------------------
    <       R1          >
    ---------------------
    <       R0          >       Task args.
    ---------------------
    <       R11         >
    ---------------------
    <       R10         >
    ---------------------
    <       R9          >
    ---------------------
    <       R8          >
    ---------------------
    <       R7          >
    ---------------------
    <       R6          >
    ---------------------
    <       R5          >
    ---------------------
    <       R4          >
    ---------------------
*/
static task_return_t _TaskStackInit (ptask_fun_t  pTaskFun,
                                    void *      pFunArgs,
                                    pos_tcb_t   pTcb)
{
    /*
        对于传入的各种参数进行判断
    */
    if ((!pTaskFun)     \
        || (!pTcb)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        检查 tcb 的栈是否存在
    */
    if (!pTcb->pTcbStkAddr) {
        os_err("%s %d pTcbStkAddr is error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        入栈的方式是 sp 先减，再存储
    */
    *(--pTcb->pTcbStkAddr) = INITIAL_XPSR;
    *(--pTcb->pTcbStkAddr) = (cpu_add_t)pTaskFun & 0xfffffffeUL;
    *(--pTcb->pTcbStkAddr) = (cpu_add_t)_TaskExitError;
    pTcb->pTcbStkAddr = pTcb->pTcbStkAddr - 5;
    pTcb->pTcbStkAddr = (cpu_add_t)pFunArgs;
    pTcb->pTcbStkAddr = pTcb->pTcbStkAddr - 8;

    return (TASK_ERR_NONE);
}
/*****************************************************************************************************
 * 函数：TaskCreatWithStack
 * 介绍：任务创建（需要自行分配栈）
 * 输入：pTaskFun		 任务函数
 *      pFunArgs        任务函数的输入参数
 * 		pTcb		    任务控制块指针
 *      pTaskName       任务的名称
 *      pStackAddr      栈的起始地址
 *      StackSize       栈的长度
 * 输出：TASK_ERR_NONE   任务创建成功
 *      TASK_ERR_ARGS   输入参数异常，创建失败
 *      TASK_ERR_STACK_CHECK    栈参数异常，创建失败
 *      TASK_ERR_TASK_NAME      任务名称异常，创建失败
 *      TASK_ERR_LIST_ITEM      任务链表项异常，创建失败
 *      TASK_ERR_STACK_INIT     任务栈初始化异常，创建失败
*****************************************************************************************************/
task_return_t TaskCreatWithStack (ptask_fun_t  pTaskFun,
                                    void *      pFunArgs,
                                    pos_tcb_t   pTcb,
                                    puchar_t    pTaskName,
                                    pcpu_add_t  pStackAddr,
                                    uint32_t    StackSize)
{
    task_return_t   ret = TASK_ERR_NONE;
    /*
        对于传入的各种参数进行判断
    */
    if ((!pTaskFun)     \
        || (!pTcb)      \
        || (!pTaskName) \
        || (!pStackAddr)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_ARGS);
    }

    /*
        Stack 地址和参数的检测
    */
    if (_TaskStackAddrAndSizeCheck(pTcb, pStackAddr, StackSize)) {
        os_err("%s %d StackSize is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_STACK_CHECK);
    }

    /*
        任务名称长度若超过 TASK_NAME_LENGTH 规定的长度，则截取规定的长度，剩下的丢弃
    */
    if (_TaskNameCpyAndCheck(pTcb, pTaskName)) {
        os_err("%s %d pTaskName is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_TASK_NAME);
    }
    
    /*
        初始化 TCB 链表项
    */
    if (ListMonoItemInit(&pTcb->TcbListMonoItem)) {
        os_err("%s %d pTcb is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_LIST_ITEM);
    }

    /*
        Stack 的初始化
    */
    if (_TaskStackInit(pTaskFun, pFunArgs, pTcb)) {
        os_err("%s %d pTcb is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_STACK_INIT);
    }
    
    return (TASK_ERR_NONE);
}   
