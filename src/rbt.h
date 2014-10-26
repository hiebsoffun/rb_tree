#include "dbg.h"
#include <stdlib.h>

typedef enum {
    RED, BLACK
} Color;

typedef struct {
    Node *left;
    Node *right;
    Color color;
} Node;
