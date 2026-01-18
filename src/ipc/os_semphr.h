#ifndef __OS_SEMPHR_H__
#define __OS_SEMPHR_H__

#include "os_types.h"

typedef struct {
    os_int32_t value; // Value of semaphore
} os_semaphore_t;

// Initialize semaphore
void os_sem_init(os_semaphore_t *sem, os_int32_t value);

// Wait (P operation)
void os_sem_wait(os_semaphore_t *sem);

// Post (V operation)
void os_sem_post(os_semaphore_t *sem);

// Get current value of semaphore
os_int32_t os_sem_getvalue(os_semaphore_t *sem);

#endif // __OS_SEMPHR_H__