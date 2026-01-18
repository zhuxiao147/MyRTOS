#include "os_queue.h"

#include "port.h"

#include <string.h>

// Initialize queue
void os_queue_init(os_message_queue_t *queue) {
    OS_DISABLE_INTERRUPTS();
    queue->head = 0;
    queue->tail = 0;
    memset(queue->data, 0, sizeof(queue->data));
    OS_ENABLE_INTERRUPTS();
}

// Check if queue is empty
os_bool_t os_queue_isempty(os_message_queue_t *queue) {
    return (queue->head == queue->tail);
}

// Check if queue is full
os_bool_t os_queue_isfull(os_message_queue_t *queue) {
    return ((queue->tail + 1) % QUEUE_SIZE == queue->head);
}

// Send message to queue
// Return 1 for success, 0 for full
os_uint32_t os_queue_send(os_message_queue_t *queue, const char *message) {
    os_uint32_t ret = 0;
    OS_DISABLE_INTERRUPTS();
    if (!os_queue_isfull(queue)) {
        strncpy(queue->data[queue->tail], message, MESSAGE_SIZE - 1);
        queue->data[queue->tail][MESSAGE_SIZE - 1] = '\0';
        queue->tail = (queue->tail + 1) % QUEUE_SIZE;
        ret = 1;
    }
    OS_ENABLE_INTERRUPTS();
    return ret;
}

// Receive message from queue
// Return 1 for success, 0 for empty
os_uint32_t os_queue_receive(os_message_queue_t *queue, char *buffer) {
    os_uint32_t ret = 0;
    OS_DISABLE_INTERRUPTS();
    if (!os_queue_isempty(queue)) {
        strncpy(buffer, queue->data[queue->head], MESSAGE_SIZE);
        buffer[MESSAGE_SIZE - 1] = '\0';
        queue->head = (queue->head + 1) % QUEUE_SIZE;
        ret = 1;
    }
    OS_ENABLE_INTERRUPTS();
    return ret;
}