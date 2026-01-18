#ifndef __OS_MUTEX_H__
#define __OS_MUTEX_H__

#include "os_types.h"

// Mutex structure
typedef struct {
    os_int32_t value;       // Mutex value, 0 means locked, 1 means unlocked
    os_uint32_t owner;      // Current task ID that owns the mutex
    os_uint32_t lock_count; // Recursion count for recursive mutex
} os_mutex_t;

// Initialize mutex
void os_mutex_init(os_mutex_t *mutex);

// Acquire mutex
void os_mutex_lock(os_mutex_t *mutex);

// Try to acquire mutex without blocking
os_bool_t os_mutex_trylock(os_mutex_t *mutex);

// Release mutex
void os_mutex_unlock(os_mutex_t *mutex);

#endif // __OS_MUTEX_H__