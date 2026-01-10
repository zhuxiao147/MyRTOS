#include "malloc.h"
#include <string.h>

// 静态内存池 (Static memory pool)
uint8_t malloc_pool[MALLOC_POOL_SIZE];
static MemBlock *free_list = NULL;

// 初始化内存池 (Initialize memory pool)
void simple_malloc_init(void) {
    free_list = (MemBlock *)malloc_pool;
    free_list->next = NULL;
    free_list->size = MALLOC_POOL_SIZE - sizeof(MemBlock);
    free_list->used = 0;
}

// 简易malloc实现 (Simple malloc implementation)
void *simple_malloc(size_t size) {
    MemBlock *curr = free_list;
    MemBlock *prev = NULL;
    while (curr) {
        if (!curr->used && curr->size >= size) {
            // 如果空间足够且未被使用 (If space is enough and not used)
            if (curr->size >= size + sizeof(MemBlock) + 4) {
                // 分割空闲块 (Split free block)
                MemBlock *new_block = (MemBlock *)((uint8_t *)curr + sizeof(MemBlock) + size);
                new_block->next = curr->next;
                new_block->size = curr->size - size - sizeof(MemBlock);
                new_block->used = 0;
                curr->next = new_block;
                curr->size = size;
            }
            curr->used = 1;
            return (uint8_t *)curr + sizeof(MemBlock);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL; // 没有足够空间 (Not enough space)
}

// 简易free实现 (Simple free implementation)
void simple_free(void *ptr) {
    if (!ptr) return;
    MemBlock *block = (MemBlock *)((uint8_t *)ptr - sizeof(MemBlock));
    block->used = 0;
    // 简单合并相邻空闲块 (Simple merge of adjacent free blocks)
    MemBlock *curr = free_list;
    while (curr) {
        if (!curr->used && curr->next && !curr->next->used) {
            curr->size += sizeof(MemBlock) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}
