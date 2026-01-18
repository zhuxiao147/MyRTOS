#include "os_shm.h"

#include "port.h"

#include <string.h>

// Initialize shared memory
void os_shm_init(os_share_memory_t *shm, void *buffer, os_uint32_t size) {
    OS_DISABLE_INTERRUPTS();
    shm->buffer = buffer;
    shm->size = size;
    shm->is_locked = 0;
    OS_ENABLE_INTERRUPTS();
}

// Write to shared memory
os_bool_t os_shm_write(os_share_memory_t *shm, const void *src, os_uint32_t size,
                       os_uint32_t offset) {
    os_bool_t ret = OS_FALSE;
    OS_DISABLE_INTERRUPTS();
    if (!shm->is_locked && offset + size <= shm->size) {
        memcpy((os_uint8_t *)shm->buffer + offset, src, size);
        ret = OS_TRUE;
    }
    OS_ENABLE_INTERRUPTS();
    return ret;
}

// Read from shared memory
os_bool_t os_shm_read(os_share_memory_t *shm, void *dst, os_uint32_t size, os_uint32_t offset) {
    os_bool_t ret = OS_FALSE;
    OS_DISABLE_INTERRUPTS();
    if (!shm->is_locked && offset + size <= shm->size) {
        memcpy(dst, (os_uint8_t *)shm->buffer + offset, size);
        ret = OS_TRUE;
    }
    OS_ENABLE_INTERRUPTS();
    return ret;
}

// Lock
void os_shm_lock(os_share_memory_t *shm) {
    OS_DISABLE_INTERRUPTS();
    shm->is_locked = 1;
    OS_ENABLE_INTERRUPTS();
}

// Unlock
void os_shm_unlock(os_share_memory_t *shm) {
    OS_DISABLE_INTERRUPTS();
    shm->is_locked = 0;
    OS_ENABLE_INTERRUPTS();
}

// Check if locked
os_bool_t os_shm_is_locked(os_share_memory_t *shm) {
    return shm->is_locked;
}