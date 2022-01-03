/*****************************************************************************************************
 * �ļ���task.c
 * ���ܣ��� task ����ⲿ�����ӿڵ�ʵ��
 * ���ߣ�lxx
 * ���ڣ�2022/01/02
*****************************************************************************************************/
/*****************************************************************************************************
 *                                              ͷ�ļ�����
*****************************************************************************************************/
#include <string.h>
#include "task.h"

/*****************************************************************************************************
 *                                              ����ʵ��
*****************************************************************************************************/
/*****************************************************************************************************
 * ������_TaskStackAddrAndSizeCheck
 * ���ܣ����񴴽�ʱջ�ļ��
 * ���룺pTcb		     ������ƿ�ָ��
 *      pStackAddr      ջ����ʼ��ַ
 *      StackSize       ջ�ĳ���
 * �����TASK_ERR_NONE	 ջ���ɹ�
 *      TASK_ERR_ARGS	��������쳣��ջ���ʧ��
*****************************************************************************************************/
static task_return_t _TaskStackAddrAndSizeCheck (pos_tcb_t   pTcb,
                                                    pcpu_add_t  pStackAddr,
                                                    uint32_t    StackSize)
{
    cpu_add_t   AlignTopSatackAddr  = 0x00;
    cpu_add_t   AlignStackAddr      = 0x00;
    /*
        ���ڴ���ĸ��ֲ��������ж�
    */
    if ((!pTcb)      \
        || (!pStackAddr)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        �涨����� StackSize Ӧ���� ALGIN_BYTE �������������Լ��� cpu ���ʵĴ���
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
 * ������_TaskNameCpyAndCheck
 * ���ܣ����񴴽�ʱ���Ƹ��ƺͼ��
 * ���룺pTcb		     ������ƿ�ָ��
 *      pTaskName       ���������
 * �����TASK_ERR_NONE	 ���Ƹ��ƺͼ��ɹ�
 *      TASK_ERR_ARGS	��������쳣�����ƺͼ��ʧ��
*****************************************************************************************************/
static task_return_t _TaskNameCpyAndCheck (pos_tcb_t   pTcb,
                                            puchar_t  pTaskName)
{
    /*
        ���ڴ���ĸ��ֲ��������ж�
    */
    if ((!pTcb)      \
        || (!pTaskName)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        �������Ƴ��������� TASK_NAME_LENGTH �涨�ĳ��ȣ����ȡ�涨�ĳ��ȣ�ʣ�µĶ���
    */
    pTcb->pTcbName = pTaskName;
    if (strlen(pTaskName) >= TASK_NAME_LENGTH) {
        pTcb->pTcbName[TASK_NAME_LENGTH - 1] = '\0';
    }

    return(TASK_ERR_NONE);
}
/*****************************************************************************************************
 * ������_TaskExitError
 * ���ܣ������쳣���غ�������������������ѭ����������ַ��أ���ֹ�����쳣��ʱ�򣬿��Է��ص�����
 * ���룺NONA
 * �����NONA
*****************************************************************************************************/
static void _TaskExitError( void )
{
    for(;;);
}
/*****************************************************************************************************
 * ������_TaskStackInit
 * ���ܣ�����ջ�ĳ�ʼ��
 * ���룺pTaskFun		 ������
 *      pFunArgs        �������Ĳ���
 *      pTcb            ����Ŀ��ƿ�
 * �����TASK_ERR_NONE	 ����ջ��ʼ���ɹ�
 *      TASK_ERR_ARGS	��������쳣����ʼ��ʧ��
*****************************************************************************************************/
/*
    Cortex-M3 �ļĴ����洢��ʽ
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
        ���ڴ���ĸ��ֲ��������ж�
    */
    if ((!pTaskFun)     \
        || (!pTcb)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        ��� tcb ��ջ�Ƿ����
    */
    if (!pTcb->pTcbStkAddr) {
        os_err("%s %d pTcbStkAddr is error! \r\n", __FUNCTION__, __LINE__);
        return(TASK_ERR_ARGS);
    }
    /*
        ��ջ�ķ�ʽ�� sp �ȼ����ٴ洢
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
 * ������TaskCreatWithStack
 * ���ܣ����񴴽�����Ҫ���з���ջ��
 * ���룺pTaskFun		 ������
 *      pFunArgs        ���������������
 * 		pTcb		    ������ƿ�ָ��
 *      pTaskName       ���������
 *      pStackAddr      ջ����ʼ��ַ
 *      StackSize       ջ�ĳ���
 * �����TASK_ERR_NONE   ���񴴽��ɹ�
 *      TASK_ERR_ARGS   ��������쳣������ʧ��
 *      TASK_ERR_STACK_CHECK    ջ�����쳣������ʧ��
 *      TASK_ERR_TASK_NAME      ���������쳣������ʧ��
 *      TASK_ERR_LIST_ITEM      �����������쳣������ʧ��
 *      TASK_ERR_STACK_INIT     ����ջ��ʼ���쳣������ʧ��
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
        ���ڴ���ĸ��ֲ��������ж�
    */
    if ((!pTaskFun)     \
        || (!pTcb)      \
        || (!pTaskName) \
        || (!pStackAddr)) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_ARGS);
    }

    /*
        Stack ��ַ�Ͳ����ļ��
    */
    if (_TaskStackAddrAndSizeCheck(pTcb, pStackAddr, StackSize)) {
        os_err("%s %d StackSize is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_STACK_CHECK);
    }

    /*
        �������Ƴ��������� TASK_NAME_LENGTH �涨�ĳ��ȣ����ȡ�涨�ĳ��ȣ�ʣ�µĶ���
    */
    if (_TaskNameCpyAndCheck(pTcb, pTaskName)) {
        os_err("%s %d pTaskName is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_TASK_NAME);
    }
    
    /*
        ��ʼ�� TCB ������
    */
    if (ListMonoItemInit(&pTcb->TcbListMonoItem)) {
        os_err("%s %d pTcb is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_LIST_ITEM);
    }

    /*
        Stack �ĳ�ʼ��
    */
    if (_TaskStackInit(pTaskFun, pFunArgs, pTcb)) {
        os_err("%s %d pTcb is error! \r\n", __FUNCTION__, __LINE__);
        return (TASK_ERR_STACK_INIT);
    }
    
    return (TASK_ERR_NONE);
}   
