#ifndef __rbtree_h__
#define __rbtree_h__

#include "dbg.h"
#include <stdlib.h>

#define MAX_NAME_LENGTH 64

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
void delete_all();
void delete(const char *);
void print_names();
#endif
