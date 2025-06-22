/*Copyright 2025 Popa Marius*/

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node TNode;
typedef TNode* Item;

typedef struct QueueNode {
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef struct Queue {
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

Queue *createQueue();
int isQueueEmpty(Queue *q);
void enqueue(Queue *q, Item elem);
Item front(Queue *q);
void dequeue(Queue *q);
void destroyQueue(Queue *q);

#endif	// QUEUE_H_
