#include "os.h"

void OS_PORT_SVC_Handler(void) {
  __asm volatile("mov r1, #2      \n"
                 "msr control, r1 \n"
                 "isb             \n"
                 "bx r14           \n");
}

void OS_PORT_PendSV_Handler(void) {
  __asm volatile("mrs     r0, psp \n"
                 "stmdb   r0!, {r4-r11} \n"
                 "ldr     r1, =os_currentTask \n"
                 "ldr     r2, [r1] \n"
                 "str     r0, [r2] \n"

                 "cpsie i        \n"
                 "bl      os_scheduler \n"
                 "cpsid i        \n"
                 "mov r14, 0xFFFFFFFD \n"

                 "ldr     r1, =os_currentTask \n"
                 "ldr     r2, [r1] \n"
                 "ldr     r0, [r2] \n"
                 "ldmia   r0!, {r4-r11} \n"
                 "msr     psp, r0 \n"
                 "isb     \n"

                 "bx      r14 \n");
}

void OS_PORT_SysTick_Handler(void) {

  for (int i = 0; i < os_taskCount; i++) {
    os_tcb_t *task = &os_task_list[i];
    if (task->delay_ticks > 0) {
      if (--task->delay_ticks == 0)
        task->state = OS_TASK_READY;
    }
  }

  if (os_currentTask->state == OS_TASK_RUNNING) {
    if (--os_currentTask->time_slice == 0)
      OS_YIELD_FROM_ISR();
  }

  HAL_IncTick();
}
