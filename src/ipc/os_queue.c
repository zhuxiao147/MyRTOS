#include "os_queue.h"

#include "port.h"

#include <string.h>

// 初始化队列 (Initialize queue)
void os_queue_init(os_message_queue_t *queue) {
    OS_DISABLE_INTERRUPTS();
    queue->head = 0;
    queue->tail = 0;
    memset(queue->data, 0, sizeof(queue->data));
    OS_ENABLE_INTERRUPTS();
}

// 判断队列是否为空 (Check if queue is empty)
os_bool_t os_queue_isempty(os_message_queue_t *queue) {
    return (queue->head == queue->tail);
}

// 判断队列是否为满 (Check if queue is full)
os_bool_t os_queue_isfull(os_message_queue_t *queue) {
    return ((queue->tail + 1) % QUEUE_SIZE == queue->head);
}

// 发送消息到队列 (Send message to queue)
// 返回1表示成功，0表示队列已满 (Return 1 for success, 0 for full)
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

// 从队列接收消息 (Receive message from queue)
// 返回1表示成功，0表示队列为空 (Return 1 for success, 0 for empty)
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
