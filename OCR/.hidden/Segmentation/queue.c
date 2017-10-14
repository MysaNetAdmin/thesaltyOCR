# include <assert.h>
# include <err.h>
# include <stdio.h>
# include <stdlib.h>
 
# include "queue.h"
 
/*
 * queue_init(queue) initialize an empty queue container
 * allocation of the container is kept external for more flexibility
 */
void queue_init(struct queue *queue) {

	queue = malloc(sizeof (struct queue));
	queue->store = NULL;
	queue->size = 0;
}
 
/*
 * queue_is_empty(queue) test for emptyness
 */
int queue_is_empty(struct queue *queue) {

	if (queue->size == 0) {
		return (1);
	} else {
		return (0);
	}
}
 
/*
 * queue_push(queue, elm) push elm
 */
void queue_push(struct queue *queue, void *elm) {

	struct list* node = malloc(sizeof (struct list));
	node->next = (queue_is_empty(queue) ? node : queue->store->next);
	node->data = elm;

	if (!queue_is_empty(queue)) {
		queue->store->next = node;
	}
	queue->store = node;
	queue->size += 1;
}
 
/*
 * queue_pop(queue) pop the next element (FIFO order)
 * returns NULL if the queue is empty
 */
void* queue_pop(struct queue *queue) {

	if (queue_is_empty(queue)) {
		return (NULL);
	} else {
		struct list* tmp = queue->store->next;
		queue->store->next = queue->store->next->next;
		void* elm = tmp->data;
		//free(tmp);
		queue->size -= 1;
		return (elm);
	}
}
