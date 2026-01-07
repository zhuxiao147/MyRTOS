#ifndef __MYRTOS_H__
#define __MYRTOS_H__
#include <stdint.h>

#define MAX_TASKS 10  // 最大任务数量
#define TASK_STACK_SIZE 128  // 每个任务的栈大小
#define TIME_SLICE_DEFAULT 5 // 默认时间片长度

typedef void (*TaskFunction)(void);  // 任务函数指针类型

// 任务状态
#define TASK_READY 0
#define TASK_RUNNING 1
#define TASK_BLOCKED 2

typedef struct {
    uint32_t *stackPointer;  // 任务的栈指针
    uint8_t state;  // 任务状态
    uint8_t time_slice; // 时间片计数 
    int8_t count; // 用于调度的计数器
} TaskControlBlock;

extern TaskControlBlock taskList[MAX_TASKS];    // 任务控制块数组
extern uint32_t task_stack[MAX_TASKS][TASK_STACK_SIZE];  // 用于存储任务栈
extern int taskCount;  // 当前任务数量
extern TaskControlBlock *currentTask;   // 当前运行的任务
extern TaskControlBlock *nextTask;      // 下一个要运行的任务

void scheduler(void);
void os_start(void);
void task_init(TaskFunction task_func);
extern __attribute__((naked,noreturn)) void switch_to_psp(uint32_t *psp_top);
void switch_to_task(TaskControlBlock *tcb); 

#endif
