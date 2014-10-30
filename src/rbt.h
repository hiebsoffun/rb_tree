#ifndef __rbtree_h__
#define __rbtree_h__

#include "dbg.h"
#include <stdlib.h>

#define MAX_NAME_LENGTH 60

typedef enum {
    RED, BLACK
} Color;

typedef struct _Node{
	struct _Node *parent;
    struct _Node *left;
    struct _Node *right;
    Color color;
	// data
	char name[MAX_NAME_LENGTH];
} Node;

void insert(const char *);
void delete_tree();
void print_names();
#endif
