#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include "os_types.h"

#define QUEUE_SIZE 5
#define MESSAGE_SIZE 32

// Message queue structure
typedef struct {
    char data[QUEUE_SIZE][MESSAGE_SIZE]; // Message storage area
    int head;                            // Queue head
    int tail;                            // Queue tail
} os_message_queue_t;

void os_queue_init(os_message_queue_t *queue);
os_bool_t os_queue_isempty(os_message_queue_t *queue);
os_bool_t os_queue_isfull(os_message_queue_t *queue);
os_uint32_t os_queue_send(os_message_queue_t *queue, const char *message);
os_uint32_t os_queue_receive(os_message_queue_t *queue, char *buffer);
#endif // __OS_QUEUE_H__