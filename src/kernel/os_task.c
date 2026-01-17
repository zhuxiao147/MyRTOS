#include "os.h"

#include <string.h>

os_tcb_t os_task_list[OS_MAX_TASKS] = {0};
os_uint32_t os_task_stack[OS_MAX_TASKS][OS_TASK_STACK_SIZE] __attribute__((aligned(8))) = {0};

os_uint32_t os_taskCount = 0;
os_tcb_t *os_currentTask = 0;

void os_task_init(void) {
    os_taskCount = 0;
    os_currentTask = 0;
    memset(os_task_list, 0, sizeof(os_task_list));
    memset(os_task_stack, 0, sizeof(os_task_stack));
    os_task_create(os_idle_task, OS_PRIORITY_IDLE, OS_TIME_SLICE_DEFAULT);
}
os_task_handle_t os_task_create(TaskFunction task_func, os_priority_t priority,
                                os_uint32_t time_slice) {
    os_int32_t index = -1;
    os_tcb_t *nextTask = 0;
    if (task_func == 0 || priority < OS_PRIORITY_IDLE || priority > OS_PRIORITY_HIGH ||
        time_slice == 0) {
        return OS_TASK_INVALID_HANDLE;
    }

    OS_DISABLE_INTERRUPTS();
    for (os_int32_t i = 0; i < OS_MAX_TASKS; i++) {
        nextTask = &os_task_list[i];
        if (nextTask->state == OS_TASK_DELETED) {
            // found a unused task, use it
            index = i;
            break;
        }
    }
    if (index == -1) {
        OS_ENABLE_INTERRUPTS();
        return OS_TASK_INVALID_HANDLE;
    }

    memset(nextTask, 0, sizeof(os_tcb_t));
    memset(os_task_stack[index], 0, sizeof(os_task_stack[index]));

    os_uint32_t *stackTop = &os_task_stack[index][OS_TASK_STACK_SIZE - 1];
    *(stackTop--) = (1 << 24);                  // xPSR
    *(stackTop--) = (os_uint32_t)task_func | 1; // PC
    *(stackTop--) = 0xFFFFFFFD;                 // LR
    *(stackTop--) = 0;                          // R12
    *(stackTop--) = 0;                          // R3
    *(stackTop--) = 0;                          // R2
    *(stackTop--) = 0;                          // R1
    *(stackTop--) = 0;                          // R0
    for (int i = 0; i < 8; i++) {
        *(stackTop--) = 0; // R4 ~ R11
    }
    // Set up task control block
    os_task_list[index].stackPointer = stackTop + 1;
    os_task_list[index].time_slice = time_slice;
    os_task_list[index].time_slice_set = time_slice;
    os_task_list[index].delay_ticks = 0;
    os_task_list[index].scheduler_count = 0;
    os_task_list[index].id = index;
    os_task_list[index].state = OS_TASK_READY;
    os_task_list[index].priority = priority;

    os_taskCount++;
    OS_ENABLE_INTERRUPTS();
    return index;
}

os_bool_t os_task_delete(os_task_handle_t task_handle) {
    os_bool_t yieldflag = OS_FALSE;
    OS_DISABLE_INTERRUPTS();
    if (task_handle >= OS_MAX_TASKS || task_handle == 0 || task_handle == OS_TASK_INVALID_HANDLE ||
        os_task_list[task_handle].state == OS_TASK_DELETED) {
        OS_ENABLE_INTERRUPTS();
        return OS_FALSE;
    }
    if (os_task_list[task_handle].state == OS_TASK_RUNNING)
        yieldflag = OS_TRUE;

    // Mark task as deleted
    os_task_list[task_handle].state = OS_TASK_DELETED;
    os_taskCount--;

    if (yieldflag)
        OS_YIELD_FROM_ISR();

    OS_ENABLE_INTERRUPTS();

    return OS_TRUE;
}

void os_idle_task(void) {
    while (1) {
        OS_IDLE();
    }
}
