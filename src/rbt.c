    /*
 * 	C implementation of a red-black tree.
 *	
 * 	The colored nodes of a red-black tree have three requirements:
 *	1.) All leaves are black
 *	2.) Child nodes of a red node are black
 * 	3.) Given a node, every path to its leaves counts the same number of black nodes. 
*/

#include "rbt.h"

#define NIL &_NIL

// tree root
static Node *root = NULL;

// the sentinel node
static Node _NIL = { 
    NULL, NULL, NULL, BLACK, ""
};

// counters for debug purposes
static unsigned int ins_count;
static unsigned int dbl_count;
static unsigned int del_count;

// static helper function declarations
//--------------------------------------------------------------------------------------------------
static Node *uncle(
    Node *n
);

static Node *grandparent(
    Node *n
);

static void print_names_rec(
    Node *n
);

static Node *find_by_name(
    const char *name
);

static void fix_rb_tree(
    Node *new_node
);

static void rotate_left(
    Node *n
);

static void rotate_right(
    Node *n
);

static void check_requirements(
    Node *n
);

static void delete_all_rec(
	Node *n
);

//--------------------------------------------------------------------------------------------------
Node *grandparent(
    Node *n
) {
    check(n, "Node == NULL");
    check(n->parent, "n->parent == NULL");
    return n->parent->parent;

error:
    return NULL;
}

//--------------------------------------------------------------------------------------------------
Node *uncle(
    Node *n
) {
    Node *gp = grandparent(n);
    check(gp, "Grandparent == NULL");
    if(n->parent == gp->left) {
        return gp->right;
    } else {
        return gp->left;
    }

error:
    return NULL;
}

void insert(
    const char *name
) {
    debug("Insert node: %s", name);

    // Is name exceeding MAX_NAME_LENGTH chars?
    unsigned int len = strlen(name);
    // MAX_NAME_LENGTH is 59 bytes+'\0'
    check(len<MAX_NAME_LENGTH, "Insert: Name exceeds maximal length."); 

    // Allocate and initialize a new node
    Node *n = malloc(sizeof(Node));
    check_mem(n);
    n->parent = NULL; n->left = NIL; n->right = NIL; n->color = RED;
    strncpy(n->name, name, len);
    n->name[len] = '\0';
    
    if(root == NULL) {
		ins_count = 0;
		dbl_count = 0;
		del_count = 0;
        root = n;
    } else {
        // if already exsits skip
        if(find_by_name(n->name)) {
            debug("Insert node: %salready exists", n->name);
			dbl_count++;
            free(n);
            return;
        }
        // search for the proper place to include the node
        Node *current = root;
        while (1) {
    //        debug("Cmp %s with %s", n->name, current->name);
            int res = strcmp(n->name, current->name);
            if(res < 0) {
                if(current->left == NIL) {
                    current->left = n;
                    n->parent = current;
                    break;
                } else {
                    current = current->left;
                }
            } else if(res > 0) {
                if(current->right == NIL) {
                    current->right = n;
                    n->parent = current;
                    break;
                } else {
                    current = current->right;
                }
            }
        }	
    }


    // ensure red black tree properties
    fix_rb_tree(n);

	ins_count++;
	debug("Insert Counter: %d\n", ins_count);
    // check for properties
    check_requirements(root);

error:
    return;
}

//--------------------------------------------------------------------------------------------------
/*
* An inserted node is always red. This violates maybe req. 2.) --> repairing necessary
* Given a Node N to be inserted, it's Uncle (U), it's parent (P) and grandparent (GP), 
* five cases have to be considered:
* 1.) root == NULL, (obvious)
* 2.) The parent of N is black -> nothing to do
* 3.) U and P of N is red -> color U,P black and GP red. -> req. 2.) maybe violated 
* 	  then recursivley insert GP to the tree
* 4.) N has non or a black U while N is the right child of his red P and P is left of GP
*     -> left rotation around P and apply case 5.)
* 5.) N has non or a black U and is the left child of his red P and P is left of GP.
* 	  -> right rotation around GP. Then swap colors of former GP and P.
*/
void fix_rb_tree(
    Node *n
) {
    check(n, "Insert node: new_node == NULL");
    Node *parent = n->parent;
    
    // case 1.)
    if(parent == NULL) {
    //    debug("Insert node: case 1.)");
        n->color = BLACK;
        return;
    }

    // case 2.) The parent is black. There is no need to fix the tree.
    if(parent->color == BLACK) {
    //  debug("Insert node: case 2.)");
      return;
    }

    // case 3.) U and P of N is red -> color U,P black and GP red.
    if(uncle(n)->color == RED) {
      //  debug("Insert node: case 3.)");
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        fix_rb_tree(grandparent(n));
        return;
    }

    // case 4.) N has non or a black U while N is the right child of his red P and P is left of GP
    if(
        n == n->parent->right &&
        n->parent == grandparent(n)->left
    ) {
      //  debug("Fixing: case 4.) rotate left.");
        rotate_left(n->parent);
        n = n->left;
    } else if(
                n == n->parent->left &&
                n->parent == grandparent(n)->right) {
     //   debug("Fixing: case 4.) rotate right.");
        rotate_right(n->parent);
        n = n->right;
    }

    // 5.) N has non or a black U and is the left child of his red P and P is left of GP.
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if(
        n == n->parent->left &&
        n->parent == grandparent(n)->left
    ) {
     //   debug("Fixing: case 5.) rotate right.");
        rotate_right(grandparent(n));
    } else {
     //   debug("Fixing: case 5.) rotate left.");
        rotate_left(grandparent(n));
    }

error:
    return;
}

//-------------------------------------------------------------------------------------------------
void rotate_left(
    Node *n
) {
    check_node(n, "Rotate left: n == NIL");
    check_node(n->right, "Rotate left: n->right == NIL");	
	
	Node *r = n->right;
	if (n->parent == NULL) {
        root = r;
    } else {
        if (n == n->parent->left)
            n->parent->left = r;
        else
            n->parent->right = r;
    }
    if (r != NULL) {
       r->parent = n->parent;
    }
    n->right = r->left;
    if (r->left != NULL) {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
    
	/*
	Node *p = n->parent;
    Node *r = n->right->left;

    n->parent = n->right;
    n->right->parent = p;
    if(p) {
        if(n == p->right) {
            p->right = n->right;
        } else {
            p->left = n->right;
        }
    } else {
        root = n->right;
    }
    n->right->left = n;
    n->right = r;
	
	*/
    
error:
    return;	
}

//-------------------------------------------------------------------------------------------------
void rotate_right(
    Node *n
) {
    check_node(n, "Rotate right: n == NIL");
    check_node(n->left, "Rotate right: n->left == NIL");	

	Node *l = n->left;
	if (n->parent == NULL) {
        root = l;
    } else {
        if (n == n->parent->left)
            n->parent->left = l;
        else
            n->parent->right = l;
    }
    if (l != NULL) {
		l->parent = n->parent;
    }
    n->left = l->right;
    if (l->right != NULL) {
        l->right->parent = n;
    }
    l->right = n;
    n->parent = l;
    
	/*
	Node *p = n->parent;
    Node *l = n->left->right;

    n->parent = n->left;
    n->left->parent = p;
    if(p) {
        if(n == p->left) {
            p->left = n->left;
        } else {
            p->right = n->left;
        }
    } else {
        root = n->left;
    }
    n->left->right = n;
    n->left = l;
    
	*/
error:
    return;	
}

//--------------------------------------------------------------------------------------------------
void check_requirements(
    Node *n
) {
    if(n == NIL) {
        return;
    }
    if(n->color == RED) {
        check(n->left->color == BLACK, "check_requirements: n->left->color != BLACK");
        check(n->right->color == BLACK, "check_requirements: n->right->color != BLACK");
        check(n->parent->color == BLACK, "check_requirements: n->parent->color != BLACK");
    }
    check_requirements(n->left);
    check_requirements(n->right);

error:
    return;

}

//--------------------------------------------------------------------------------------------------
void delete_all(
) {
	debug("Deleting all");
    check(root, "Delete tree: root ==  NULL");
	
	delete_all_rec(root);
	
	debug("Double counter: %d", dbl_count);
	debug("Delete counter: %d", del_count);
	check((ins_count + dbl_count) == (del_count + dbl_count),
			"Number of inserted deleted nodes not matching");

	ins_count = 0;
	root = NULL;
	

error:
    return;	
}

//--------------------------------------------------------------------------------------------------
void delete_all_rec(
	Node *n
) {
	if(n->left != NIL) {
		delete_all_rec(n->left);
	}
	if(n->right != NIL) {
		delete_all_rec(n->right);
	}
	free(n);
	del_count++;
}

//--------------------------------------------------------------------------------------------------
void delete(
    const char *name
) {
    debug("Delete: deleting %s", name);
    check(root, "Delete tree: root ==  NULL");
    
    Node *n = find_by_name(name);
    if(!n) {
        debug("Delete: Name not found");
        return;
    } else {
        debug("Delete: Name %s found", n->name);
        
    }

error:
    return;	
}

//-------------------------------------------------------------------------------------------------
Node *find_by_name (
    const char *name
) {
    Node *n = root;
    int res;
    while(n != NIL) {
        res = strcmp(name, n->name);
        if(res < 0) {
            n = n->left;
        } else if(res > 0) {
            n = n->right;
        } else {
            return n;
        }
    }		
    return NULL;
}

//--------------------------------------------------------------------------------------------------
void print_names(

) {
    check(root, "Print tree: root is NULL");
    log_info("Printing tree...");	
    print_names_rec(root);

error:
    return;	
}

//-------------------------------------------------------------------------------------------------
void print_names_rec(
    Node *n
) {
    if(n == NIL) {
        return;
    }
    print_names_rec(n->left);
    char c = n->color == BLACK ? 'B' : 'R';
    if(n != root) {
        printf("%s %c, left: %s, right %s\n", n->name, c, n->left->name, n->right->name);
    } else {
        printf("%s %c root, left: %s, right %s\n", n->name, c, n->left->name, n->right->name);
    }
    print_names_rec(n->right);
}

