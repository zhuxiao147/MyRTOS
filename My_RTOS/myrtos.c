#include "myrtos.h"

TaskControlBlock taskList[MAX_TASKS];    // 任务控制块数组
uint32_t task_stack[MAX_TASKS][TASK_STACK_SIZE] __attribute__((aligned(8)));  // 用于存储任务栈
int taskCount = 0;  // 当前任务数量

TaskControlBlock *currentTask = 0;   // 当前运行的任务
TaskControlBlock *nextTask = 0;      // 下一个要运行的任务

__attribute__((naked,noreturn)) void switch_to_psp(uint32_t* psp)
{
    __asm volatile (
        "msr psp, r0     \n"
        "mov r1, #2      \n"
        "msr control, r1 \n"
        "isb             \n"
        "bx r14           \n"
    );
}

void task_init(TaskFunction task_func)
{
    if (taskCount >= MAX_TASKS)
    {
        return;  // 任务数量超过最大限制
    }
    
    uint32_t *stackTop = &task_stack[taskCount][TASK_STACK_SIZE - 1];

    // 栈帧顺序从下到上，模拟 ARM 上的任务状态
    *(stackTop--) = (1 << 24); // xPSR，标志任务已经启动（T-bit = 1）
    *(stackTop--) = (uint32_t) task_func | 1; // PC，指向任务入口
    *(stackTop--) = 0xFFFFFFFD; // LR
    *(stackTop--) = 0; // R12
    *(stackTop--) = 0; // R3
    *(stackTop--) = 0; // R2
    *(stackTop--) = 0; // R1
    *(stackTop--) = 0; // R0

    for (int i = 0; i < 8; i++) 
    {
        *(stackTop--) = 0; // R4 ~ R11
    }

    // 任务栈指针指向栈顶
    taskList[taskCount].stackPointer = stackTop + 1;

    // 任务状态设置为就绪
    taskList[taskCount].state = TASK_READY;
    taskList[taskCount].count = 0; // 初始化调度计数器
    taskList[taskCount].time_slice = TIME_SLICE_DEFAULT; // 初始化时间片计数器
    taskCount++;
} 

void scheduler(void) {
    int8_t biggercount = 0;
    if (taskCount == 0) {
        return; // 没有任务可运行
    }

    // 简单的轮询调度
    for (int i = 0; i < taskCount; i++) {
        if (taskList[i].state == TASK_READY)
        {
            if(taskList[i].count >= biggercount) {
                biggercount = taskList[i].count;
                nextTask = &taskList[i];
            }
            else
            {         
                taskList[i].count++; // 增加时间片计数  
            }
        }

    }
    currentTask->state = TASK_READY;
    currentTask->count = 0; // 重置当前任务的时间片计数

    nextTask->state = TASK_RUNNING;
    nextTask->count = 0; // 重置下一个任务的时间片计数
    nextTask->time_slice = TIME_SLICE_DEFAULT; // 重置时间片计数器
    currentTask = nextTask;
}


void os_start(void)
{
    if (taskCount == 0)
    {
        return; // 没有任务可运行
    }

    // 选择第一个任务作为当前任务
    currentTask = &taskList[0];
    currentTask->state = TASK_RUNNING;
    switch_to_task(currentTask);
}

void switch_to_task(TaskControlBlock *tcb) {
    switch_to_psp((uint32_t*)(tcb->stackPointer));  // 切换到指定任务的栈
}

