#include "os_semphr.h"

#include "port.h"

// 初始化信号量 (Initialize semaphore)
void os_sem_init(os_semaphore_t *sem, os_int32_t value) {
    OS_DISABLE_INTERRUPTS();
    sem->value = value;
    OS_ENABLE_INTERRUPTS();
}

// 等待（P操作）  (P operation))
void os_sem_wait(os_semaphore_t *sem) {
    while (sem->value <= 0)
        ; // 自旋等待 (Spin wait)

    OS_DISABLE_INTERRUPTS();
    sem->value--;
    OS_ENABLE_INTERRUPTS();
}

// 释放（V操作）  (V operation))
void os_sem_post(os_semaphore_t *sem) {
    OS_DISABLE_INTERRUPTS();
    sem->value++;
    OS_ENABLE_INTERRUPTS();
}

// 获取信号量的当前值 (Get current value of semaphore)
os_int32_t os_sem_getvalue(os_semaphore_t *sem) {
    return sem->value;
}
