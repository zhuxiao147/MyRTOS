#include "queue.h"
#include "port.h"
#include <string.h>

// 初始化队列 (Initialize queue)
void queue_init(MessageQueue *queue) {
	__disable_irq();
	queue->head = 0;
	queue->tail = 0;
	memset(queue->data, 0, sizeof(queue->data)); // 清空消息存储区 (Clear message storage area)
	__enable_irq();
}

// 判断队列是否为空 (Check if queue is empty)
uint8_t queue_is_empty(MessageQueue *queue) {
	return (queue->head == queue->tail);
}

// 判断队列是否为满 (Check if queue is full)
uint8_t queue_is_full(MessageQueue *queue) {
	return ((queue->tail + 1) % QUEUE_SIZE == queue->head);
}

// 发送消息到队列 (Send message to queue)
// 返回1表示成功，0表示队列已满 (Return 1 for success, 0 for full)
uint32_t send_message(MessageQueue *queue, const char *message) {
	uint32_t ret = 0;
	__disable_irq();
	if (!queue_is_full(queue)) {
		strncpy(queue->data[queue->tail], message, MESSAGE_SIZE - 1);
		queue->data[queue->tail][MESSAGE_SIZE - 1] = '\0'; // 保证字符串结尾 (Ensure string ends)
		queue->tail = (queue->tail + 1) % QUEUE_SIZE;
		ret = 1;
	}
	__enable_irq();
	return ret;
}

// 从队列接收消息 (Receive message from queue)
// 返回1表示成功，0表示队列为空 (Return 1 for success, 0 for empty)
uint32_t receive_message(MessageQueue *queue, char *buffer) {
	uint32_t ret = 0;
	__disable_irq();
	if (!queue_is_empty(queue)) {
		strncpy(buffer, queue->data[queue->head], MESSAGE_SIZE);
		buffer[MESSAGE_SIZE - 1] = '\0'; // 保证字符串结尾 (Ensure string ends)
		queue->head = (queue->head + 1) % QUEUE_SIZE;
		ret = 1;
	}
	__enable_irq();
	return ret;
}
