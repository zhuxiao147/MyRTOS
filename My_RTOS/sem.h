#ifndef SEM_H
#define SEM_H

#include <stdint.h>
#include <stdio.h>
#include "port.h"

typedef struct {
    int32_t value;  // 信号量的值
} Semaphore;

// 初始化信号量
void sem_init(Semaphore *sem, int32_t value);

// 等待（P操作）
void sem_wait(Semaphore *sem);

// 释放（V操作）
void sem_post(Semaphore *sem);

// 获取信号量的当前值
int32_t sem_get_value(Semaphore *sem);


#endif // SEM_H