#include "os.h"

void os_scheduler(void) {
  os_int32_t count = 0;
  os_uint8_t priority = os_currentTask->priority;
  os_tcb_t *nextTask = 0;

  if (os_taskCount == 0)
    return;

  for (int i = 0; i < os_taskCount; i++) {
    if (os_task_list[i].state == OS_TASK_READY) {
      if (os_task_list[i].count >= count &&
          os_task_list[i].priority >= priority) {
        count = os_task_list[i].count;
        priority = os_task_list[i].priority;
        nextTask = &os_task_list[i];
      }
      os_task_list[i].count++;
    }
  }

  // If no suitable task found, continue with current task or idle task
  if (nextTask == 0) {
    if (os_currentTask->state == OS_TASK_RUNNING)
      nextTask = os_currentTask;
    else
      nextTask = &os_task_list[0];
  }

  // Update current task state
  if (os_currentTask->state == OS_TASK_RUNNING)
    os_currentTask->state = OS_TASK_READY;

  // Reset counts and update states
  os_currentTask->count = 0;
  nextTask->state = OS_TASK_RUNNING;
  nextTask->count = 0;
  nextTask->time_slice = nextTask->time_slice_set;
  os_currentTask = nextTask;
}
