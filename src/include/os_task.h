#ifndef __OS_TASK_H__
#define __OS_TASK_H__
#include "os_config.h"
#include "os_types.h"

extern os_tcb_t os_task_list[MAX_TASKS];
extern os_uint32_t os_task_stack[MAX_TASKS][OS_TASK_STACK_SIZE];
extern os_uint32_t os_taskCount;
extern os_tcb_t *os_currentTask;

void os_task_create(TaskFunction task_func, os_uint8_t priority,
                    os_uint32_t time_slice);
void os_delay(os_uint32_t ticks);
void os_idle_task(void);

#endif // __OS_TASK_H__
