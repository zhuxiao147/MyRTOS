#ifndef MALLOC_H
#define MALLOC_H

#include <stdint.h>
#include <stddef.h>

//非线程安全，多线程使用请自行增加临界区保护 (Not thread-safe, please add critical section protection for multi-thread use)

// 内存块头部结构 (Memory block header structure)
typedef struct MemBlock {
    struct MemBlock *next; // 下一个空闲块 (Next free block)
    size_t size;          // 当前块大小（不含头部） (Current block size, excluding header)
    uint8_t used;         // 1=已分配，0=空闲 (1=allocated, 0=free)
} MemBlock;

// 管理的内存池 (Managed memory pool)
#define MALLOC_POOL_SIZE 1024
extern uint8_t malloc_pool[MALLOC_POOL_SIZE];

void simple_malloc_init(void);
void *simple_malloc(size_t size);
void simple_free(void *ptr);

#endif // MALLOC_H
