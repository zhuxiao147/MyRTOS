#include "sem.h"

// 初始化信号量
void sem_init(Semaphore *sem, int32_t value)
{
    __disable_irq(); 
    sem->value = value;
    __enable_irq();
}

// 等待（P操作）
void sem_wait(Semaphore *sem)
{
    while (sem->value <= 0)
    {
        // 在实际RTOS中，这里会有任务阻塞的实现
    }
    __disable_irq(); 
    sem->value--;
    __enable_irq();  
}

// 释放（V操作）
void sem_post(Semaphore *sem)
{
    __disable_irq();
    sem->value++;
    __enable_irq();
}

// 获取信号量的当前值
int32_t sem_get_value(Semaphore *sem)
{
    return sem->value;
}
