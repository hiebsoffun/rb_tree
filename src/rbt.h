#include "dbg.h"
#include <stdlib.h>

typedef enum {
    RED, BLACK
} Color;

typedef struct _Node{
    struct _Node *left;
    struct _Node *right;
    Color color;
} Node;

void print(void);
