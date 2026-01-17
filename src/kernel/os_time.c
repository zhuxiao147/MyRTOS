#include "os.h"

static volatile os_uint32_t os_tick_count = 0;

void os_time_init(void) {
    os_tick_count = 0;
}

void os_increment_tick_count(void) {
    os_tick_count++;
}

os_uint32_t os_get_tick_count(void) {
    os_uint32_t tick_count;
    OS_DISABLE_INTERRUPTS();
    tick_count = os_tick_count;
    OS_ENABLE_INTERRUPTS();
    return tick_count;
}

void os_delay(os_uint32_t ticks) {
    os_tcb_t *current_task_snapshot = os_currentTask;
    if (current_task_snapshot == 0)
        return;

    OS_DISABLE_INTERRUPTS();
    current_task_snapshot->delay_ticks = ticks;
    current_task_snapshot->state = OS_TASK_BLOCKED;
    OS_ENABLE_INTERRUPTS();
    OS_YIELD_FROM_ISR();
}

void os_delay_ms(os_uint32_t ms) {
    if (os_currentTask == 0)
        return;

    os_uint32_t ticks_per_ms = OS_SYSTICKFREQ / 1000;
    if (ms > OS_UINT32_MAX / ticks_per_ms)
        os_delay(OS_UINT32_MAX);
    else {
        os_delay(ms * ticks_per_ms);
    }
}
