/*Copyright 2025 Popa Marius*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "heap.h"

#define HEAP_SIZE 10

/**
 * This function returns the tree based on
 * the min-heap.
 */
TNode* createSatelliteNetwork(PriQueue *q) {
    if (isHeapEmpty(q)) {
        return NULL;
    }
    TNode *leftChild = NULL, *rightChild = NULL;
    while (q->size > 1) {
        leftChild = extractMin(q);
        rightChild = extractMin(q);

        TNode *parent = mergeSatellites(leftChild, rightChild);
        insertInHeap(q, parent);
    }
    TNode *root = extractMin(q);

    return root;
}

/**
 * This function creates a new node in the tree that
 * follows the creation rules.
 */
TNode* mergeSatellites(TNode* first, TNode* second) {
    // concatenate names
    char name[name_len];
    strcpy(name, first->values.name);
    strcat(name, second->values.name);

    // add frequencies
    int freq = first->values.freq + second->values.freq;

    TNode *new = initNode(freq, name, NULL, first, second);
    return new;
}

/**
 * Verify all nodes until find out the node.
 */
TNode* findNode(TNode *root, char *name) {
    if (!strcmp(root->values.name, name)) {
        return root;
    }

    TNode *node;
    if (root->leftChild) {
        node = findNode(root->leftChild, name);
        if (node) return node;
    }
    if (root->rightChild) {
        node = findNode(root->rightChild, name);
        if (node) return node;
    }
    if (root->subtree) {
        node = findNode(root->subtree, name);
        if (node) return node;
    }

    return NULL;
}

/**
 * Creates a tree node.
 */
TNode* initNode(int freq, char *name,
                TNode *parent, TNode *lChild, TNode* rChild) {
    TNode *node = malloc(sizeof(*node));
    if (!node) {
        perror("Malloc failed.");
        // Free memory and exit program
        exit(EXIT_FAILURE);
    }

    node->values.freq = freq;
    strcpy(node->values.name, name);
    node->parent = parent;
    node->leftChild = lChild;
    node->rightChild = rChild;
    node->subtree = NULL;

    if (lChild) lChild->parent = node;
    if (rChild) rChild->parent = node;

    return node;
}

/**
 * This function adds a 'son' node to a 'parent' in multi-way tree.
 */
void addSibling(TNode* parent, TNode *kid) {
    if (!parent || !kid) return;

    TNode *list = parent->leftChild;

    // Parent doesn't have any kids
    if (!list) {
        parent->leftChild = kid;
        return;
    }

    // Insert to last position
    while (list->rightChild) {
        list = list->rightChild;
    }
    list->rightChild = kid;
}

/**
 * This function frees the memory allocated for the
 * tree.
 */
void freeTree(TNode* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->rightChild);
    freeTree(root->leftChild);
    freeTree(root->subtree);
    free(root);
}
