#ifndef __QUEUE_H
#define __QUEUE_H

#define MAX 256

#include "graph.h"

/*vertex_t queue[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;*/

struct queue{
	vertex_t *v;
	int front;
	int rear;
	int itemCount;
};

typedef struct queue *queue_t;

vertex_t peek(queue_t q);
int isEmpty(queue_t q);
int isFull(queue_t q);
int isInQueue(vertex_t data, queue_t q);
int size(queue_t q);
void insert(vertex_t data, queue_t q);
vertex_t removeData(queue_t q);

#endif
