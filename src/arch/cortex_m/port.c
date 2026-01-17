#include "os.h"

void os_nvic_init() {
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
    HAL_NVIC_SetPriority(SVCall_IRQn, 5, 0);
}

__attribute__((naked)) void first_to_psp(os_uint32_t *psp) {
    __asm volatile("ldmia   r0!, {r4-r11} \n"
                   "msr psp, r0     \n"
                   "svc 0           \n");
}

void OS_PORT_SVC_Handler(void) {
    __asm volatile("mov r1, #2      \n"
                   "msr control, r1 \n"
                   "mov r14, 0xFFFFFFFD \n"
                   "isb             \n"
                   "bx r14           \n");
}

void OS_PORT_PendSV_Handler(void) {
    __asm volatile("mrs     r0, psp \n"
                   "stmdb   r0!, {r4-r11} \n"
                   "ldr     r1, =os_currentTask \n"
                   "ldr     r2, [r1] \n"
                   "str     r0, [r2] \n"

                   "bl      os_scheduler \n"
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
    os_increment_tick_count();
    HAL_IncTick();

    os_tcb_t *current_task_snapshot = os_currentTask;

    for (int i = 0; i < OS_MAX_TASKS; i++) {
        os_tcb_t *task = &os_task_list[i];
        if (task != 0) {
            if (task->delay_ticks > 0 && task->state != OS_TASK_DELETED) {
                if (--task->delay_ticks == 0) {
                    task->state = OS_TASK_READY;
                    if (current_task_snapshot->priority < task->priority)
                        OS_YIELD_FROM_ISR();
                }
            }
        }
    }

    if (current_task_snapshot->state == OS_TASK_RUNNING) {
        if (current_task_snapshot->time_slice != OS_TASK_INFINITE_TIME_SLICE) {
            if (--current_task_snapshot->time_slice == 0)
                OS_YIELD_FROM_ISR();
        }
    }
}
