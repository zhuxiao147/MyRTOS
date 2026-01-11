#ifndef __MYRTOS_H__
#define __MYRTOS_H__
#include <stdint.h>

#define MAX_TASKS 10  // 最大任务数量 (Maximum number of tasks)
#define TASK_STACK_SIZE 128  // 每个任务的栈大小 (Stack size for each task)
#define TIME_SLICE_DEFAULT 500 // 默认时间片长度 (Default time slice length)

typedef void (*TaskFunction)(void);  // 任务函数指针类型 (Task function pointer type)

// 任务状态 (Task state)
#define TASK_READY 0
#define TASK_RUNNING 1
#define TASK_BLOCKED 2

// 任务优先级 (Task priority)
#define PRIORITY_IDLE 0
#define PRIORITY_LOW 1
#define PRIORITY_MEDIUM 2
#define PRIORITY_HIGH 3

typedef struct {
    uint32_t *stackPointer;  // 任务的栈指针 (Task stack pointer)
    uint8_t state;  // 任务状态 (Task state)
    uint8_t priority; // 任务优先级 (Task priority)
    uint32_t time_slice; // 时间片计数 (Time slice counter)
    uint32_t time_slice_set; // 设置的时间片长度 (Set time slice)
    int32_t count; // 用于调度的计数器 (Counter for scheduling)
    uint32_t delay_ticks; // 延时计数器 (Delay counter)
} TaskControlBlock;

extern TaskControlBlock taskList[MAX_TASKS];    // 任务控制块数组 (Task control block array)
extern uint32_t task_stack[MAX_TASKS][TASK_STACK_SIZE];  // 用于存储任务栈 (Used to store task stack)
extern int taskCount;  // 当前任务数量 (Current number of tasks)
extern TaskControlBlock *currentTask;   // 当前运行的任务 (Currently running task)
extern TaskControlBlock *nextTask;      // 下一个要运行的任务 (Next task to run)

void task_init(TaskFunction task_func, uint8_t priority, uint32_t time_slice);
void scheduler(void);
void os_init(void);
void os_start(void);
extern __attribute__((naked)) void switch_to_psp(uint32_t *psp_top);

void task_delay(uint32_t ticks); // 任务延时函数 (Task delay function)
#endif // __MYRTOS_H__
