#include "os.h"

void os_init(void) {
    os_nvic_init();
    os_time_init();
    os_task_init();
    /* USER INIT CODE START*/
    /* USER INIT CODE END*/
}

void os_start(void) {
    if (os_taskCount == 0)
        return;
    // set idle task as the default task
    os_currentTask = &os_task_list[0];
    os_currentTask->state = OS_TASK_RUNNING;
    if (os_currentTask->stackPointer == 0)
        return;
    // shcheduler first time
    os_scheduler();
    first_to_psp(os_currentTask->stackPointer);
}
