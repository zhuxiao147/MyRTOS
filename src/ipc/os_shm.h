#ifndef __OS_SHM_H__
#define __OS_SHM_H__

#include "os_types.h"

// 共享内存结构体 (Shared memory structure)
typedef struct {
  void *buffer;        // 指向共享内存的指针 (Pointer to shared memory)
  os_uint32_t size;    // 共享内存大小 (Shared memory size)
  os_bool_t is_locked; // 简单互斥锁标志 (Simple mutex lock flag)
} os_share_memory_t;

// 初始化共享内存 (Initialize shared memory)
void os_shm_init(os_share_memory_t *shm, void *buffer, os_uint32_t size);
// 写入共享内存 (Write to shared memory)
os_bool_t os_shm_write(os_share_memory_t *shm, const void *src,
                       os_uint32_t size, os_uint32_t offset);
// 从共享内存读取 (Read from shared memory)
os_bool_t os_shm_read(os_share_memory_t *shm, void *dst, os_uint32_t size,
                      os_uint32_t offset);
// 上锁 (Lock)
void os_shm_lock(os_share_memory_t *shm);
// 解锁 (Unlock)
void os_shm_unlock(os_share_memory_t *shm);
// 判断是否上锁 (Check if locked)
os_bool_t os_shm_is_locked(os_share_memory_t *shm);
#endif // __OS_SHM_H__
