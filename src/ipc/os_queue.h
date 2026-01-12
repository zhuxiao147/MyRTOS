#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include "os_types.h"

#define QUEUE_SIZE 5
#define MESSAGE_SIZE 32

// 消息队列结构 (Message queue structure)
typedef struct {
  char data[QUEUE_SIZE][MESSAGE_SIZE]; // 消息存储区 (Message storage area)
  int head;                            // 队头 (Queue head)
  int tail;                            // 队尾 (Queue tail)
} os_message_queue_t;

void os_queue_init(os_message_queue_t *queue);
os_bool_t os_queue_isempty(os_message_queue_t *queue);
os_bool_t os_queue_isfull(os_message_queue_t *queue);
os_uint32_t os_queue_send(os_message_queue_t *queue, const char *message);
os_uint32_t os_queue_receive(os_message_queue_t *queue, char *buffer);
#endif // __OS_QUEUE_H__