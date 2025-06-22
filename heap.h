/*Copyright 2025 Popa Marius*/
#ifndef HEAP_H_
#define HEAP_H_


typedef struct node TNode;

typedef struct heap {
    int maxHeapSize;
    int size;
    TNode **elem;
} PriQueue;


PriQueue *initQueue(int maxHeapSize);
int getParent(int i);
void siftUp(PriQueue *q, int idx);
void insertInHeap(PriQueue *q, TNode *e);
void removeMin(PriQueue *q);
TNode *extractMin(PriQueue *q);
int isHeapEmpty(PriQueue *q);
void freeQueue(PriQueue **q);

#endif  // HEAP_H_
