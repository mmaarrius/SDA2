/*Copyright 2025 Popa Marius*/

#ifndef TREE_H_
#define TREE_H_
#include "heap.h"

#define name_len 300

typedef struct {
    int freq;
    char name[name_len];
} data;

typedef struct node {
    data values;
    struct node *parent;
    struct node *leftChild;     // Or left son for multiWay tree
    struct node *rightChild;    // Or right sibling for multiWay tree
    struct node *subtree;       // Bind binary tree to multi-way tree
} TNode;

TNode *createSatelliteNetwork(PriQueue *q);
TNode *mergeSatellites(TNode *first, TNode *second);
TNode *initNode(int, char*, TNode*, TNode*, TNode*);
TNode* findNode(TNode *root, char *name);
void addSibling(TNode* parent, TNode *kid);
void printTree(TNode *t, FILE *f_out);
void freeTree(TNode *root);

#endif  // TREE_H_
