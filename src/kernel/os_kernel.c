#include "os.h"

__attribute__((naked)) void first_to_psp(os_uint32_t *psp) {
  __asm volatile("ldmia   r0!, {r4-r11} \n"
                 "msr psp, r0     \n"
                 "svc 0           \n");
}

void os_init(void) {
  os_taskCount = 0;
  os_currentTask = 0;
  os_task_create(os_idle_task, OS_PRIORITY_IDLE, TIME_SLICE_DEFAULT);
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
  HAL_NVIC_SetPriority(SVCall_IRQn, 5, 0);
  /* USER INIT CODE START*/
  /* USER INIT CODE END*/
}

void os_start(void) {
  if (os_taskCount == 0)
    return;
  // set idle task as the first task
  os_currentTask = &os_task_list[0];
  os_currentTask->state = OS_TASK_RUNNING;
  // shcheduler first time
  os_scheduler();
  first_to_psp(os_currentTask->stackPointer);
}
