#include "os.h"
#include <string.h>

os_tcb_t os_task_list[MAX_TASKS];
os_uint32_t os_task_stack[MAX_TASKS][OS_TASK_STACK_SIZE]
    __attribute__((aligned(8))) = {0};

os_uint32_t os_taskCount = 0;
os_tcb_t *os_currentTask = 0;

os_uint32_t os_task_create(TaskFunction task_func, os_priority_t priority,
                           os_uint32_t time_slice) {
  os_int32_t index = -1;
  os_tcb_t *nextTask = 0;

  OS_DISABLE_INTERRUPTS();
  for (os_int32_t i = 0; i < MAX_TASKS; i++) {
    nextTask = &os_task_list[i];
    if (nextTask->state == OS_TASK_DELETED) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    return OS_TASK_INVALID_ID;
  }

  memset(nextTask, 0, sizeof(os_tcb_t));

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
  os_task_list[index].count = 0;
  os_task_list[index].id = index;
  os_task_list[index].state = OS_TASK_READY;
  os_task_list[index].priority = priority;

  os_taskCount++;
  OS_ENABLE_INTERRUPTS();
  return index;
}

os_bool_t os_task_delete(os_uint32_t task_id) {

  OS_DISABLE_INTERRUPTS();
  if (task_id >= os_taskCount || task_id == 0 ||
      os_task_list[task_id].state == OS_TASK_DELETED ||
      os_task_list[task_id].state == OS_TASK_RUNNING ||
      os_task_list[task_id].state == OS_TASK_BLOCKED) {
    return OS_FALSE;
  }
  // Mark task as deleted
  os_task_list[task_id].state = OS_TASK_DELETED;

  os_taskCount--;
  OS_ENABLE_INTERRUPTS();
  return OS_TRUE;
}

void os_idle_task(void) {
  while (1) {
    OS_IDLE();
  }
}
