/*Copyright 2025 Popa Marius*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "heap.h"
#include "queue.h"

#define BUFFER_LENGTH 1000
#define max_path 100

void performTask(char *task, FILE *f_in, FILE *f_out);
void task1(FILE *f_in, FILE *f_out);
TNode *readAndCreateTree(FILE *f_in);
void task2(FILE *f_in, FILE *f_out);
void task3(FILE *f_in, FILE *f_out);
void task4(FILE *f_in, FILE *f_out);
int containsAllNodes(TNode *node, char **names, int size);
void addMultiWayTree(TNode *mainNode, FILE *f_in);
void task5(FILE *f_in, FILE *f_out);
int findCostOfPath(TNode *node1, TNode *node2);

int main(int argc, char *argv[]) {
    // verify number of arguments
    if (argc != 4) {
        return 0;
    }

    FILE *f_in = fopen(argv[2], "r");
    if (!f_in) {
        return 0;
    }

    // delete previous content of .out file
    FILE *f_del = fopen(argv[3], "w");
    if (!f_del) {
        fclose(f_in);
        return 0;
    }
    fclose(f_del);

    FILE *f_out = fopen(argv[3], "a");
    if (!f_out) {
        fclose(f_in);
        return 0;
    }

    // Select necessary function for the task
    performTask(argv[1], f_in, f_out);

    fclose(f_in);
    fclose(f_out);

    return 0;
}

/**
 * Perform specified task.
 */
void performTask(char *task, FILE *f_in, FILE *f_out) {
    if (!strcmp(task, "-c1")) {
        task1(f_in, f_out);
    } else if (!strcmp(task, "-c2")) {
        task2(f_in, f_out);
    } else if (!strcmp(task, "-c3")) {
        task3(f_in, f_out);
    } else if (!strcmp(task, "-c4")) {
        task4(f_in, f_out);
    } else if (!strcmp(task, "-c5")) {
        task5(f_in, f_out);
    }
}

/**
 * Perform task1 -> read, create and print the binary tree.
 */
void task1(FILE *f_in, FILE *f_out) {
    // Read from file and create tree-like structure
    TNode *root = readAndCreateTree(f_in);
    if (!root) {
        fprintf(f_out, "Error: Tree creation failed.\n");
        return;
    }

    // Print tree on levels
    Queue *q = createQueue();
    enqueue(q, root);

    // for the end of the level
    enqueue(q, NULL);

    while (!isQueueEmpty(q)) {
        TNode *crt = front(q);
        dequeue(q);

        // end of the level
        if (!crt) {
            fprintf(f_out, "\n");
            if (!isQueueEmpty(q)) {
                enqueue(q, NULL);
            }
            continue;
        }

        fprintf(f_out, "%d-%s ", crt->values.freq, crt->values.name);

        if (crt->leftChild) {
            enqueue(q, crt->leftChild);
        }
        if (crt->rightChild) {
            enqueue(q, crt->rightChild);
        }
    }
    destroyQueue(q);

    freeTree(root);
}

/**
 * Perform task 2 -> find nodes based on binary paths.
 */
void task2(FILE *f_in, FILE *f_out) {
    // Read from file and create tree-like structure
    TNode *root = readAndCreateTree(f_in);

    char buffer[BUFFER_LENGTH];

    // Number of paths
    fgets(buffer, BUFFER_LENGTH, f_in);
    int n = atoi(buffer);

    for (int i = 0; i < n; i++) {
        // Read path
        fgets(buffer, BUFFER_LENGTH, f_in);

        TNode *crt = root;

        // Take bit by bit
        for (int j = 0; buffer[j] != '\0' && buffer[j] != '\n'; j++) {
            // Transform 1/0 string into 1/0 number
            int move = buffer[j] - '0';

            if (move == 1) {
                crt = crt->rightChild;
            } else {
                crt = crt->leftChild;
            }

            // No kids, so I found the node
            if (!crt->leftChild && !crt->rightChild) {
                fprintf(f_out, "%s ", crt->values.name);
                crt = root;
            }
        }
        fprintf(f_out, "\n");
    }
    freeTree(root);
}

/**
 * Perform task 3 -> read satellites and find
 * the path to them.
 */
void task3(FILE *f_in, FILE *f_out) {
    TNode *root = readAndCreateTree(f_in);

    char buffer[BUFFER_LENGTH];
    // Number of satellites
    fgets(buffer, BUFFER_LENGTH, f_in);
    int inputs = atoi(buffer);

    TNode *crt;
    for (int i = 0; i < inputs; i++) {
        // Name of satellite
        fgets(buffer, BUFFER_LENGTH, f_in);
        buffer[strcspn(buffer, "\n")] = '\0';

        char name[name_len];
        strcpy(name, buffer);

        crt = findNode(root, name);

        // Find number of moves from root to node
        int len = findCostOfPath(root, crt);
        int moves[max_path];
        int copy_len = len;

        while (crt != root) {
            if (crt->parent->leftChild == crt) {
                moves[len - 1] = 0;
            } else {
                moves[len - 1] = 1;
            }
            len--;
            crt = crt->parent;
        }

        for (int i = 0; i < copy_len; i++) {
            fprintf(f_out, "%d", moves[i]);
        }   
    }
    freeTree(root);
}

/**
 * Perform task 4 -> find common lowest ancestor.
 */
void task4(FILE *f_in, FILE *f_out) {
    TNode *root = readAndCreateTree(f_in);

    char buffer[BUFFER_LENGTH];
    fgets(buffer, BUFFER_LENGTH, f_in);
    int inputs = atoi(buffer);

    // Keep all satellites
    char **arr = malloc(inputs * sizeof(*arr));
    if (!arr) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < inputs; i++) {
        fgets(buffer, BUFFER_LENGTH, f_in);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Memorise satellite
        arr[i] = malloc(strlen(buffer) + 1);
        if (!arr[i]) {
            perror("Malloc faildes");
            exit(EXIT_FAILURE);
        }
        strcpy(arr[i], buffer);
    }

    TNode *crt = root;
    while (1) {
        if (containsAllNodes(crt->leftChild, arr, inputs)) {
            crt = crt->leftChild;
        } else if (containsAllNodes(crt->rightChild, arr, inputs)) {
            crt = crt->rightChild;
        } else {
            break;
        }
    }

    fprintf(f_out, "%s", crt->values.name);
    for (int i = 0; i < inputs; i++) {
        free(arr[i]);
    }
    free(arr);
    freeTree(root);
}

/**
 * Peform task 5 -> Add multi-way tree and calculate
 * length between any 2 nodes.
 */
void task5(FILE *f_in, FILE *f_out) {
    TNode *mainRoot = readAndCreateTree(f_in);
    char buffer[BUFFER_LENGTH];

    // number of multi-way trees that will be added
    fgets(buffer, BUFFER_LENGTH, f_in);
    int trees = atoi(buffer);

    // perform addings
    for (int i = 0; i < trees; i++) {
        // the node that points to subtree
        fgets(buffer, BUFFER_LENGTH, f_in);
        buffer[strcspn(buffer, "\n")] = '\0';
        TNode *node = findNode(mainRoot, buffer);

        // add subtree
        addMultiWayTree(node, f_in);
    }

    // read the two nodes
    fgets(buffer, BUFFER_LENGTH, f_in);
    buffer[strcspn(buffer, "\n")] = '\0';
    char name1[name_len + 1];
    char name2[name_len + 1];

    sscanf(buffer, "%s %s", name1, name2);

    TNode *start = findNode(mainRoot, name1);
    TNode *end = findNode(mainRoot, name2);

    int len = findCostOfPath(start, end);

    fprintf(f_out, "%d", len);

    freeTree(mainRoot);
}

/**
 * Read the satellites from input file and
 * create the binary tree.
 */
TNode* readAndCreateTree(FILE *f_in) {
    char buffer[BUFFER_LENGTH];
    fgets(buffer, BUFFER_LENGTH, f_in);
    int n = atoi(buffer);

    int freq;
    char name[name_len + 1];

    PriQueue *q = initQueue(n);
    for (int i = 0; i < n; i++) {
        fgets(buffer, BUFFER_LENGTH, f_in);

        if (sscanf(buffer, "%d %s", &freq, name) != 2) {
            perror("Reading from input failed");
            exit(EXIT_FAILURE);
        }

        TNode *new = initNode(freq, name, NULL, NULL, NULL);
        insertInHeap(q, new);
    }

    TNode *root = createSatelliteNetwork(q);
    freeQueue(&q);

    return root;
}

/**
 * Verify if the node contains all nodes's
 * names given in the array.
 */
int containsAllNodes(TNode *node, char **names, int size) {
    if (!node) {
        return 0;
    }
    for (int i = 0; i < size; i++) {
        if (!strstr(node->values.name, names[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Read multi-way tree.
 */
void addMultiWayTree(TNode *mainNode, FILE *f_in) {
    char buffer[BUFFER_LENGTH];

    // read data about root
    fgets(buffer, BUFFER_LENGTH, f_in);

    // extract data
    int freq;
    char name[name_len + 1];
    // remove '\n'
    buffer[strcspn(buffer, "\n")] = '\0';
    if (sscanf(buffer, "%d %s", &freq, name) != 2) {
        perror("Reading from input failed");
        exit(EXIT_FAILURE);
    }

    // set node and add subtree
    TNode *root = initNode(freq, name, mainNode, NULL, NULL);
    mainNode->subtree = root;

    // read number of parents
    fgets(buffer, BUFFER_LENGTH, f_in);

    int parents = atoi(buffer);

    for (int i = 0; i < parents; i++) {
        // read parent name
        fgets(buffer, BUFFER_LENGTH, f_in);
        // remove '\n'
        buffer[strcspn(buffer, "\n")] = '\0';

        TNode *parent = findNode(root, buffer);

        // read number of childs
        fgets(buffer, BUFFER_LENGTH, f_in);
        int kids = atoi(buffer);

        for (int j = 0; j < kids; j++) {
            // read kid
            fgets(buffer, BUFFER_LENGTH, f_in);
            // remove '\n'
            buffer[strcspn(buffer, "\n")] = '\0';
            if (sscanf(buffer, "%d %s", &freq, name) != 2) {
                perror("Reading from input failed");
                exit(EXIT_FAILURE);
            }

            // add sibling to tree
            TNode* newNode = initNode(freq, name, parent, NULL, NULL);
            addSibling(parent, newNode);
        }
    }
}

/**
 * Find the length of the path between two nodes.
 */
int findCostOfPath(TNode *node1, TNode *node2) {
    TNode *path1[max_path];
    TNode *path2[max_path];

    int len1 = 0;
    while (node1) {
        path1[len1++] = node1;
        node1 = node1->parent;
    }

    int len2 = 0;
    while (node2) {
        path2[len2++] = node2;
        node2 = node2->parent;
    }

    int index1 = len1 - 1;
    int index2 = len2 - 1;

    while (path1[index1] == path2[index2]) {
        index1--;
        index2--;
    }

    return index1 + index2 + 2;
}
