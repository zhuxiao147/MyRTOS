#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "port.h"

#define QUEUE_SIZE 5
#define MESSAGE_SIZE 32

// 消息队列结构
typedef struct {
    char data[QUEUE_SIZE][MESSAGE_SIZE];  // 消息存储区
    int head;  // 队头
    int tail;   // 队尾
} MessageQueue;

void queue_init(MessageQueue *queue);
uint8_t queue_is_empty(MessageQueue *queue);
uint8_t queue_is_full(MessageQueue *queue);
uint32_t send_message(MessageQueue *queue, const char *message);
uint32_t receive_message(MessageQueue *queue, char *buffer);

#endif // QUEUE_H