/*Copyright 2025 Popa Marius*/

#include "queue.h"

Queue* createQueue() {
    Queue *q = malloc(sizeof(*q));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int isQueueEmpty(Queue *q) {
    if (q->size == 0) {
        return 1;
    }
    return 0;
}

void enqueue(Queue *q, Item elem) {
    QueueNode *qn = malloc(sizeof(*qn));
    qn->elem = elem;
    qn->next = NULL;

    if (isQueueEmpty(q)) {
        q->front = qn;
        q->rear = qn;
    } else {
        q->rear->next = qn;
        q->rear = qn;
    }
    q->size++;
}

Item front(Queue* q) {
    return q->front->elem;
}

void dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        return;
    }

    QueueNode *tmp = q->front;
    q->front = q->front->next;
    if (q->size == 1) {
        q->rear = NULL;
    }

    free(tmp);
    q->size--;
}

void destroyQueue(Queue *q) {
    while (q->front != NULL) {
        dequeue(q);
    }
    free(q);
}
