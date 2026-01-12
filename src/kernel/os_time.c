#include "os.h"

void os_delay(os_uint32_t ticks) {
  OS_DISABLE_INTERRUPTS();
  os_currentTask->delay_ticks = ticks;
  os_currentTask->state = OS_TASK_BLOCKED;
  OS_ENABLE_INTERRUPTS();
  OS_YIELD_FROM_ISR();
}