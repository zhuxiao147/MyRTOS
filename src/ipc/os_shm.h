#ifndef __OS_SHM_H__
#define __OS_SHM_H__

#include "os_types.h"

// Shared memory structure
typedef struct {
    void *buffer;        // Pointer to shared memory
    os_uint32_t size;    // Shared memory size
    os_bool_t is_locked; // Simple mutex lock flag
} os_share_memory_t;

// Initialize shared memory
void os_shm_init(os_share_memory_t *shm, void *buffer, os_uint32_t size);
// Write to shared memory
os_bool_t os_shm_write(os_share_memory_t *shm, const void *src, os_uint32_t size,
                       os_uint32_t offset);
// Read from shared memory
os_bool_t os_shm_read(os_share_memory_t *shm, void *dst, os_uint32_t size, os_uint32_t offset);
// Lock
void os_shm_lock(os_share_memory_t *shm);
// Unlock
void os_shm_unlock(os_share_memory_t *shm);
// Check if locked
os_bool_t os_shm_is_locked(os_share_memory_t *shm);
#endif // __OS_SHM_H__