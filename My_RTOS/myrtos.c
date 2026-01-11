#include "myrtos.h"
#include "port.h"

TaskControlBlock taskList[MAX_TASKS];    // 任务控制块数组 (Task control block array)
uint32_t task_stack[MAX_TASKS][TASK_STACK_SIZE] __attribute__((aligned(8)));  // 用于存储任务栈 (Used to store task stack)
int taskCount = 0;  // 当前任务数量 (Current number of tasks)

TaskControlBlock *currentTask = 0;   // 当前运行的任务 (Currently running task)

__attribute__((naked)) void switch_to_psp(uint32_t* psp)
{
    __asm volatile (
        "ldmia   r0!, {r4-r11} \n"
        "msr psp, r0     \n"
        "svc 0           \n"
    );
}


void static idle_task(void)
{
    while (1)
    {
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); 
    }
}

void task_init(TaskFunction task_func, uint8_t priority, uint32_t time_slice)
{
    if (taskCount >= MAX_TASKS)
    {
        return;  // 任务数量超过最大限制 (Number of tasks exceeds maximum limit)
    }
    
    uint32_t *stackTop = &task_stack[taskCount][TASK_STACK_SIZE - 1];

    // 栈帧顺序从下到上，模拟 ARM 上的任务状态 (Stack frame order from bottom to top, simulating task state on ARM)
    *(stackTop--) = (1 << 24); // xPSR，标志任务已经启动（T-bit = 1） (xPSR, indicates task started (T-bit = 1))
    *(stackTop--) = (uint32_t) task_func | 1; // PC，指向任务入口 (PC, points to task entry)
    *(stackTop--) = 0xFFFFFFFD; // LR (Link Register)
    *(stackTop--) = 0; // R12
    *(stackTop--) = 0; // R3
    *(stackTop--) = 0; // R2
    *(stackTop--) = 0; // R1
    *(stackTop--) = 0; // R0

    for (int i = 0; i < 8; i++) 
    {
        *(stackTop--) = 0; // R4 ~ R11
    }

    // 任务栈指针指向栈顶 (Task stack pointer points to top of stack)
    taskList[taskCount].stackPointer = stackTop + 1;

    // 任务状态设置为就绪 (Set task state to ready)
    taskList[taskCount].state = TASK_READY;
    taskList[taskCount].count = 0; // 初始化调度计数器 (Initialize schedule counter)
    taskList[taskCount].priority = priority; // 设置任务优先级 (Set task priority)
    taskList[taskCount].time_slice = time_slice; // 初始化时间片计数器 (Initialize time slice counter)
    taskList[taskCount].time_slice_set = time_slice; // 设置时间片长度 (Set time slice length)
    taskList[taskCount].delay_ticks = 0; // 初始化延时计数器 (Initialize delay counter)
    taskCount++;
} 

void os_init(void)
{
    taskCount = 0;
    currentTask = 0;
    task_init(idle_task, PRIORITY_IDLE, TIME_SLICE_DEFAULT); // 初始化空闲任务 (Initialize idle task)

    HAL_NVIC_SetPriority(PendSV_IRQn, 0x04, 0); // 设置PendSV中断优先级为最低 (Set PendSV interrupt priority to lowest)
    HAL_NVIC_SetPriority(SysTick_IRQn, 0x03, 0); // 设置SysTick中断优先级高于PendSV (Set SysTick interrupt priority higher than PendSV)

}

void scheduler(void) {
    int32_t count = 0;
    uint8_t priority = currentTask->priority;
    TaskControlBlock *nextTask = 0;      // 下一个要运行的任务 (Next task to run)

    if (taskCount == 0) 
    {
        return; // 没有任务可运行 (No task to run)
    }

    for (int i = 0; i < taskCount; i++) 
    {
        if (taskList[i].state == TASK_READY)
        {
            if(taskList[i].count >= count && taskList[i].priority >= priority) 
            {
                count = taskList[i].count;
                priority = taskList[i].priority;
                nextTask = &taskList[i];
            }
            taskList[i].count++; // 增加调度计数  (Increase schedule counter)
        }
    }
    if (nextTask == 0 )
    {
        if(currentTask->state == TASK_RUNNING)
            nextTask = currentTask; // 如果没有就绪任务，继续运行当前任务 (If no ready task, continue running current task)
        else
            nextTask = &taskList[0]; // 运行空闲
    }

    if(currentTask->state == TASK_RUNNING)
    {
        currentTask->state = TASK_READY;
    }
    currentTask->count = 0; // 重置当前任务的调度计数 (Reset current task's schedule counter)

    nextTask->state = TASK_RUNNING;
    nextTask->count = 0; // 重置下一个任务的调度计数 (Reset next task's schedule counter)
    nextTask->time_slice = nextTask->time_slice_set; // 重置时间片计数器 (Reset time slice counter)
    currentTask = nextTask;
}

void os_start(void)
{
    if (taskCount == 0)
    {
        return; // 没有任务可运行 (No task to run)
    }
    // 选择第一个任务作为当前任务 (Select the first task as current task)
    currentTask = &taskList[0];
    currentTask->state = TASK_RUNNING;
    scheduler(); //调度一次，防止低优先级先运行
    switch_to_psp(currentTask->stackPointer);
}

void task_delay(uint32_t ticks) {
    __disable_irq(); // 进入临界区 (Enter critical section)
    currentTask->delay_ticks = ticks; // 设置延时计数器 (Set delay counter)
    currentTask->state = TASK_BLOCKED; // 设置任务状态为阻塞 (Set task state to blocked)
    __enable_irq(); // 退出临界区 (Exit critical section)
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // 触发PendSV中断进行任务切换 (Trigger PendSV interrupt for task switch)
}
