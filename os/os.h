/*****************************************************************************************************
 * 文件：os.h
 * 介绍：与 os 相关的数据类型的声明和变量的外部声明
 * 作者：lxx
 * 日期：2022/01/02
*****************************************************************************************************/

#ifndef __OS_H_
#define __OS_H_

/*****************************************************************************************************
 *                                              头文件包含
*****************************************************************************************************/
#include "cpu_type.h"
#include "list.h"

/*****************************************************************************************************
 *                                              数据类型声明
*****************************************************************************************************/
struct _os_tcb {
    cpu_add_t *     pTcbStkAddr;
    uint32_t        TcbStkSize;
    puchar_t        pTcbName;
    list_mono_t     TcbListMonoItem;
};
typedef struct _os_tcb          os_tcb_t;
typedef os_tcb_t *              pos_tcb_t;

typedef void (* ptask_fun_t)(void *);
/*
    入栈方式：
    1、由高到低 STACK_TOP_DOWN 为1
    2、由低到高 STACK_TOP_DOWN 为0
*/
#define STACK_TOP_DOWN      (0)
#define TASK_NAME_LENGTH    (32)

#define ALIGN_OFFSET        (3)
#define ALIGN_BYTE          (1 << ALIGN_OFFSET)
#define UP_ALIGN(type, value, nbyte)   \
                            (((value) + (nbyte) - 1) & (~((type)(nbyte) - 1)))
#define DOWN_ALIGN(type, value, nbyte)  \
                            ((value) & (~((type)(nbyte) - 1)))

#endif                                                                  /* __OS_H_                  */
/************************************* END OF FILES *************************************************/
