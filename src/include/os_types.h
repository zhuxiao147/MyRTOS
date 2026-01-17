#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#define OS_UINT32_MAX 0xFFFFFFFFU

typedef signed char os_int8_t;
typedef short os_int16_t;
typedef int os_int32_t;
typedef long long os_int64_t;
typedef unsigned char os_uint8_t;
typedef unsigned short os_uint16_t;
typedef unsigned int os_uint32_t;
typedef unsigned long long os_uint64_t;

typedef enum { OS_FALSE = 0, OS_TRUE = 1 } os_bool_t;

typedef unsigned int os_task_handle_t;

// 任务优先级 (Task priority)
typedef enum {
    OS_PRIORITY_IDLE = 0,
    OS_PRIORITY_LOW = 1,
    OS_PRIORITY_MEDIUM = 2,
    OS_PRIORITY_HIGH = 3
} os_priority_t;

/* 任务状态 */
typedef enum {
    OS_TASK_DELETED = 0,
    OS_TASK_READY,
    OS_TASK_RUNNING,
    OS_TASK_BLOCKED,
    OS_TASK_SUSPENDED
} os_task_state_t;

typedef struct {
    os_uint32_t *stackPointer;  // 任务的栈指针 (Task stack pointer)
    os_uint32_t time_slice;     // 时间片计数 (Time slice counter)
    os_uint32_t time_slice_set; // 设置的时间片长度 (Set time slice)
    os_uint32_t delay_ticks;    // 延时计数器 (Delay counter)
    os_int32_t scheduler_count; // 用于调度的计数器 (Counter for scheduling)
    os_uint32_t id;             // 任务ID (Task ID)
    os_task_state_t state;      // 任务状态 (Task state)
    os_priority_t priority;     // 任务优先级 (Task priority)
} os_tcb_t;

typedef void (*TaskFunction)(void); // Task function pointer type

#endif // __OS_TYPES_H__