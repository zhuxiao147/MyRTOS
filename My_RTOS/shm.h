#ifndef SHM_H
#define SHM_H

#include <stdint.h>

// 共享内存结构体 (Shared memory structure)
typedef struct {
    void *buffer;        // 指向共享内存的指针 (Pointer to shared memory)
    uint32_t size;         // 共享内存大小 (Shared memory size)
    uint8_t is_locked;   // 简单互斥锁标志 (Simple mutex lock flag)
} SharedMemory;

// 初始化共享内存 (Initialize shared memory)
void shm_init(SharedMemory *shm, void *buffer, uint32_t size);
// 写入共享内存 (Write to shared memory)
uint8_t shm_write(SharedMemory *shm, const void *src, uint32_t size, uint32_t offset);
// 从共享内存读取 (Read from shared memory)
uint8_t shm_read(SharedMemory *shm, void *dst, uint32_t size, uint32_t offset);
// 上锁 (Lock)
void shm_lock(SharedMemory *shm);
// 解锁 (Unlock)
void shm_unlock(SharedMemory *shm);
// 判断是否上锁 (Check if locked)
uint8_t shm_is_locked(SharedMemory *shm);

#endif // SHM_H
