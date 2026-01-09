#include "shm.h"
#include <string.h>

// 初始化共享内存
void shm_init(SharedMemory *shm, void *buffer, uint32_t size) {
    __disable_irq();
    shm->buffer = buffer;
    shm->size = size;
    shm->is_locked = 0;
    __enable_irq();
}

// 写入共享内存
// 返回1表示成功，0表示失败（如越界或被锁定）
uint8_t shm_write(SharedMemory *shm, const void *src, uint32_t size, uint32_t offset) {
    uint8_t ret = 0;
    __disable_irq();
    if (!shm->is_locked && offset + size <= shm->size) {
        memcpy((uint8_t*)shm->buffer + offset, src, size);
        ret = 1;
    }
    __enable_irq();
    return ret;
}

// 从共享内存读取
// 返回1表示成功，0表示失败（如越界或被锁定）
uint8_t shm_read(SharedMemory *shm, void *dst, uint32_t size, uint32_t offset) {
    uint8_t ret = 0;
    __disable_irq();
    if (!shm->is_locked && offset + size <= shm->size) {
        memcpy(dst, (uint8_t*)shm->buffer + offset, size);
        ret = 1;
    }
    __enable_irq();
    return ret;
}

// 上锁
void shm_lock(SharedMemory *shm) {
    __disable_irq(); 
    shm->is_locked = 1;
    __enable_irq();
}

// 解锁
void shm_unlock(SharedMemory *shm) {
    __disable_irq();
    shm->is_locked = 0;
    __enable_irq();
}

// 判断是否上锁
uint8_t shm_is_locked(SharedMemory *shm) {
    return shm->is_locked;
}
