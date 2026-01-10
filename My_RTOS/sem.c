#include "sem.h"
#include "port.h"

// 初始化信号量 (Initialize semaphore)
void sem_init(Semaphore *sem, int32_t value)
{
    __disable_irq(); 
    sem->value = value;
    __enable_irq();
}

// 等待（P操作） (Wait (P operation))
void sem_wait(Semaphore *sem)
{
    while (sem->value <= 0)
    ; // 自旋等待 (Spin wait)

    __disable_irq(); 
    sem->value--;
    __enable_irq();  
}

// 释放（V操作） (Post (V operation))
void sem_post(Semaphore *sem)
{
    __disable_irq();
    sem->value++;
    __enable_irq();
}

// 获取信号量的当前值 (Get current value of semaphore)
int32_t sem_get_value(Semaphore *sem)
{
    return sem->value;
}
