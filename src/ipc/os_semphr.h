#ifndef __OS_SEMPHR_H__
#define __OS_SEMPHR_H__

#include "os_types.h"

typedef struct {
    os_int32_t value; // 信号量的值 (Value of semaphore)
} os_semaphore_t;

// 初始化信号量 (Initialize semaphore)
void os_sem_init(os_semaphore_t *sem, os_int32_t value);

// 等待（P操作） (Wait (P operation))
void os_sem_wait(os_semaphore_t *sem);

// 释放（V操作） (Post (V operation))
void os_sem_post(os_semaphore_t *sem);

// 获取信号量的当前值 (Get current value of semaphore)
os_int32_t os_sem_getvalue(os_semaphore_t *sem);

#endif // __OS_SEMPHR_H__