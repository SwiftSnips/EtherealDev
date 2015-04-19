#ifndef BinTable
#define BinTable

#include "engine.h"

extern int KEY_SIZE;
extern int LAST_KEY;

typedef struct BinaryTable{
	int size;
	int elements;
	struct BinaryTree ** trees;
} BinaryTable;

typedef struct BinaryTree{
	int elements;
	struct Node * root;
} BinaryTree;

typedef struct Node{
	int value;
	unsigned int * key;
	struct Node * left;
	struct Node * right;
} Node;


BinaryTable * createTable(int size);
BinaryTree * createTree();
Node * createNode(int value, unsigned int * key);

void destroyTable(BinaryTable * table);
void destroyTree(BinaryTree * tree);
void destroyNode(Node * node);

void insertElement(BinaryTable * table, int depth, int value, unsigned int * key);
Node * getElement(BinaryTable * table, int depth, unsigned int * key);

unsigned int * encodeBoard(Board * b, int enpass, int turn);

#endif
