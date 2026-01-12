#include "os.h"

os_tcb_t os_task_list[MAX_TASKS];
os_uint32_t os_task_stack[MAX_TASKS][OS_TASK_STACK_SIZE]
    __attribute__((aligned(8)));

os_uint32_t os_taskCount = 0;
os_tcb_t *os_currentTask = 0;

void os_task_create(TaskFunction task_func, os_priority_t priority,
                    os_uint32_t time_slice) {
  if (os_taskCount >= MAX_TASKS) {
    return;
  }
  os_uint32_t *stackTop = &os_task_stack[os_taskCount][OS_TASK_STACK_SIZE - 1];
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
  os_task_list[os_taskCount].stackPointer = stackTop + 1;
  os_task_list[os_taskCount].state = OS_TASK_READY;
  os_task_list[os_taskCount].count = 0;
  os_task_list[os_taskCount].priority = priority;
  os_task_list[os_taskCount].time_slice = time_slice;
  os_task_list[os_taskCount].time_slice_set = time_slice;
  os_task_list[os_taskCount].delay_ticks = 0;
  os_taskCount++;
}

void os_idle_task(void) {
  while (1) {
    OS_IDLE();
  }
}
