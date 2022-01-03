#include <stdio.h>
#include "list.h"
#include "os.h"
#include "task.h"

static os_tcb_t     Task1Tcb;
static os_tcb_t     Task2Tcb;

void taskfunc1 (void)
{
    for(;;);
}

cpu_add_t task1stack[10 * ALIGN_BYTE];

int main (void)
{
    task_return_t  ret = TASK_ERR_NONE;

    ret = TaskCreatWithStack((ptask_fun_t)taskfunc1, 
                                NULL, 
                                &Task1Tcb, 
                                (puchar_t)"task1", 
                                task1stack, 
                                10 * ALIGN_BYTE);

    return 0;
}
