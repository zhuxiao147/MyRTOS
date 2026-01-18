#include "os_mutex.h"

#include "os_task.h" // Need to get current task ID
#include "port.h"

// Global variable to track current task (if os_currentTask is accessible)
extern os_task_t *os_currentTask;

// Initialize mutex
void os_mutex_init(os_mutex_t *mutex) {
    OS_DISABLE_INTERRUPTS();
    mutex->value = 1;      // 1 means unlocked state
    mutex->owner = 0;      // No owner
    mutex->lock_count = 0; // Lock count is 0
    OS_ENABLE_INTERRUPTS();
}

// Acquire mutex
void os_mutex_lock(os_mutex_t *mutex) {
    // Loop until acquiring lock
    while (1) {
        OS_DISABLE_INTERRUPTS();

        // If mutex is available
        if (mutex->value == 1) {
            // Lock the mutex
            mutex->value = 0;
            if (os_currentTask != NULL) {
                mutex->owner = os_currentTask->id; // Record owner
            }
            mutex->lock_count = 1; // Set lock count to 1
            OS_ENABLE_INTERRUPTS();
            break; // Successfully acquired lock, exit loop
        }
        // If the same thread requests the lock again (recursive lock)
        else if (os_currentTask != NULL && mutex->owner == os_currentTask->id) {
            mutex->lock_count++; // Increase lock count
            OS_ENABLE_INTERRUPTS();
            break; // Successfully acquired lock, exit loop
        }

        OS_ENABLE_INTERRUPTS();
        // A brief delay or yield CPU could be added here to avoid busy waiting
        // But for simplicity, we keep spin waiting
    }
}

// Try to acquire mutex without blocking
os_bool_t os_mutex_trylock(os_mutex_t *mutex) {
    OS_DISABLE_INTERRUPTS();

    // If mutex is available
    if (mutex->value == 1) {
        mutex->value = 0;
        if (os_currentTask != NULL) {
            mutex->owner = os_currentTask->id;
        }
        mutex->lock_count = 1;
        OS_ENABLE_INTERRUPTS();
        return 1; // Successfully acquired lock
    }
    // If the same thread requests the lock again (recursive lock)
    else if (os_currentTask != NULL && mutex->owner == os_currentTask->id) {
        mutex->lock_count++;
        OS_ENABLE_INTERRUPTS();
        return 1; // Successfully acquired lock
    }

    OS_ENABLE_INTERRUPTS();
    return 0; // Failed to acquire lock
}

// Release mutex
void os_mutex_unlock(os_mutex_t *mutex) {
    OS_DISABLE_INTERRUPTS();

    // Check if current thread owns the lock
    if (os_currentTask != NULL && mutex->owner == os_currentTask->id) {
        mutex->lock_count--; // Decrease lock count

        // Only truly release the lock when lock count is 0
        if (mutex->lock_count == 0) {
            mutex->value = 1; // Set to unlocked state
            mutex->owner = 0; // Clear owner
        }
    }

    OS_ENABLE_INTERRUPTS();
}