#ifndef SEM_H
#define SEM_H

#include <stdint.h>

typedef struct {
    int32_t value;  // 信号量的值 (Value of semaphore)
} Semaphore;

// 初始化信号量 (Initialize semaphore)
void sem_init(Semaphore *sem, int32_t value);

// 等待（P操作） (Wait (P operation))
void sem_wait(Semaphore *sem);

// 释放（V操作） (Post (V operation))
void sem_post(Semaphore *sem);

// 获取信号量的当前值 (Get current value of semaphore)
int32_t sem_get_value(Semaphore *sem);


#endif // SEM_H