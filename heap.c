/*Copyright 2025 Popa Marius*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "tree.h"


/**
 * This function creates a heap.
 */
PriQueue* initQueue(int maxElem) {
    PriQueue *q = malloc(sizeof(*q));

    q->maxHeapSize = maxElem;
    q->size = 0;
    q->elem = calloc(maxElem, sizeof(TNode*));

    return q;
}

int getParent(int i) {
    if (i == 0)
        return 0;
    return ((i - 1) >> 1);
}

int getLeftChild(int i) {
    return ((i << 1) + 1);
}

int getRightChild(int i) {
    return ((i << 1) + 2);
}

/**
 * This function maintains property of min-heap, starting
 * from the end position.
 */
void siftUp(PriQueue *q, int idx) {
    if (idx >= q->size || idx < 0) {
        return;
    }
    int parent_id = getParent(idx);

    while (idx > 0 &&
        (q->elem[parent_id]->values.freq >= q->elem[idx]->values.freq)) {
        TNode *parent = q->elem[parent_id];
        TNode *son = q->elem[idx];

        // parent node is smaller in ASCII
        if (parent->values.freq == son->values.freq &&
            (strcmp(parent->values.name, son->values.name) < 0)) {
            break;
        }

        // switch nodes
        TNode *tmp = q->elem[idx];
        q->elem[idx] = q->elem[parent_id];
        q->elem[parent_id] = tmp;

        idx = parent_id;
        parent_id = getParent(idx);
    }
}

/**
 * This function maintains property of min-heap,
 * starting from the root position.
 */
void siftDown(PriQueue *q, int idx) {
    int left = getLeftChild(idx);
    int right = getRightChild(idx);
    int smallest = idx;

    if (left < q->size) {
        if (q->elem[left]->values.freq < q->elem[smallest]->values.freq ||
            (q->elem[left]->values.freq == q->elem[smallest]->values.freq &&
            strcmp(q->elem[left]->values.name, q->elem[smallest]->values.name) < 0)) {
            smallest = left;
        }
    }

    if (right < q->size) {
        if (q->elem[right]->values.freq < q->elem[smallest]->values.freq ||
            (q->elem[right]->values.freq == q->elem[smallest]->values.freq &&
            strcmp(q->elem[right]->values.name, q->elem[smallest]->values.name) < 0)) {
            smallest = right;
        }
    }

    if (smallest != idx) {
        // swap
        TNode* tmp = q->elem[smallest];
        q->elem[smallest] = q->elem[idx];
        q->elem[idx] = tmp;

        // recursive call
        siftDown(q, smallest);
    }
}

/**
 * This function inserts an element to queue, 
 * maintaining property of min-heap.
 */
void insertInHeap(PriQueue *q, TNode* e) {
    if (q->size == q->maxHeapSize) {
        q->maxHeapSize *= 2;
        TNode **tmp = realloc(q->elem, q->maxHeapSize * sizeof(*(q->elem)));
        if (!tmp) {
            perror("Memory reallocation failed!");
            return;
        }
        q->elem = tmp;
    }
    q->elem[q->size] = e;
    q->size++;
    siftUp(q, q->size - 1);
}

int isHeapEmpty(PriQueue *q) {
    if (q->size == 0) {
        return 1;
    }
    return 0;
}

/**
 * Removes first element from queue, maintaining
 * property of min-heap.
 */
void removeMin(PriQueue *q) {
    q->elem[0] = q->elem[q->size - 1];
    q->size--;
    siftDown(q, 0);
}

/**
 * Extract first tree node and delete it from the heap.
 */
TNode* extractMin(PriQueue *q) {
    if (isHeapEmpty(q)) {
        return NULL;
    }
    TNode *node = q->elem[0];
    removeMin(q);
    return node;
}

/**
 * Remove queue.
 */
void freeQueue(PriQueue **q) {
    free((*q)->elem);
    free(*q);
    (*q) = NULL;
}
