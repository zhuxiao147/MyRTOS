#ifndef SEM_H
#define SEM_H

#include<stdint.h>
#include<stdio.h>
#include"stm32f7xx_hal.h"   //后期需加到项目配置中
typedef struct {
    int value;  // 信号量的值
} Semaphore;

// 初始化信号量
void sem_init(Semaphore *sem, int value);

// 等待（P操作）
void sem_wait(Semaphore *sem);

// 释放（V操作）
void sem_post(Semaphore *sem);

// 获取信号量的当前值
int sem_get_value(Semaphore *sem);


#endif // SEM_H