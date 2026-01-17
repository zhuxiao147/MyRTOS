#ifndef __OS_TASK_H__
#define __OS_TASK_H__
#include "os_config.h"
#include "os_types.h"

#define OS_MAX_TASKS 10           // Maximum number of tasks
#define OS_TASK_STACK_SIZE 128    // Stack size for each task
#define OS_TIME_SLICE_DEFAULT 500 // Default time slice length

#define OS_TASK_INVALID_HANDLE 0xFFFFFFFF      // Invalid task ID
#define OS_TASK_INFINITE_TIME_SLICE 0xFFFFFFFF // Infinite time slice length

extern os_tcb_t os_task_list[OS_MAX_TASKS];
extern os_uint32_t os_task_stack[OS_MAX_TASKS][OS_TASK_STACK_SIZE];
extern os_uint32_t os_taskCount;
extern os_tcb_t *os_currentTask;

void os_task_init(void);
os_task_handle_t os_task_create(TaskFunction task_func, os_priority_t priority,
                                os_uint32_t time_slice);
os_bool_t os_task_delete(os_task_handle_t task_handle);

void os_idle_task(void);

#endif // __OS_TASK_H__
