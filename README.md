This project helped me gain practical experience with data structures such as binary trees and min-heaps, as well as algorithms including Huffman coding, BFS, DFS.

The project description can be found [here](./Tema%202.pdf)

Run:  
  
    make
    ./run_tests.sh

# My implementation

- Since every task needs to read from input file and create a binary tree, I handled this with `readAndCreateTree` function that uses a min-heap that stores pointers to the all tree nodes. The function repeatdly combines the two nodes with the smallest frequencies until only onde remains - the root of tree.
- In ``main.c`` you'll find all you need to understand my approach. The other files contain the core infrastructure for the tree, queue and heap structures.

## Task 1

It uses ``readAndCreateTree`` function, then prints the tree level by level using a queue. To indicate the end of a level, I insert a **NULL** into the queue.

## Task 2

Since we know that all paths are valid and the nodes we're looking for are leaves, I traverse the tree according to the path indications until the I reach a node with no children. If there are still bits left, I restart from the root.

## Task 3

The given satellites are loacted in the tree, then ,using the `findCostOfPath` function, I determine the length of path from root to each satellite . This length represents the number of moves. Starting form a sattelite, I check what kind of child it is: if it is a left child, the move is `0`, otherwise,  it is `1`. The current node become the parent and and the check continues.   
The `findCostOfPath` function stores all the nodes from root to the given nodes in an array, then calcultes common nodes from root to satellites. The distance between the two satellites is calculated as *distance_root_node1 + distance_root_node1 - 2 * common_path_length*

## Task 4

The `containsAllNodes` function checks if a given node contains all the names from a given array. Since the root contains all names, I use the previously mentioned function to check which child also contains all satellites names. When no further child contains all the names, the current node is the lowest common ancestor.

## Task 5

I added the `Tnode* subtree` instance to point to the root of multiway tree. My multiway tree is implemented as **left son right sibling tree**. That approch allows me to reuse the binary tree functions. To find the distance between any two nodes, I use `findCostOfPath` function.

