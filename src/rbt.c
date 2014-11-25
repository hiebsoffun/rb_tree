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

static void delete_tree_rec(
	Node *n
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
	Node *new_node = malloc(sizeof(Node));
	check_mem(new_node);
	new_node->parent = NULL; new_node->left = NIL; new_node->right = NIL; new_node->color = RED;
	strncpy(new_node->name, name, len);
	new_node->name[len] = '\0';
	
	if(root == NULL) {
		root = new_node;
	} else {
		// search for the proper place to include the node
		Node *current = root;
		Node *parent = root;
		debug("Cmp %s with %s", new_node->name, root->name);
		int res = strcmp(new_node->name, root->name);
		if(res < 0) {
			current = root->left;
		} else if(res > 0) {
			current = root->right;
		} else {
			// new node has the same name as root
			// nothing to do -> free new_node and return
			debug("Insert node: %s already exists", new_node->name);
			free(new_node);
			return;
		}
	
		while (current != NIL) {
			parent = current;
			debug("Cmp %s with %s", new_node->name, root->name);
			res = strcmp(new_node->name, current->name);
			if(res < 0) {
				current = current->left;
			} else if(res > 0) {
				current = current->right;
			} else {
				debug("Insert node: %s already exists", new_node->name);
				free(new_node);
				return;
			}
		}	

		// insert new node
		new_node->parent = parent;
		if(res < 0) {
			parent->left = new_node;
		} else {
			parent->right = new_node;
		}
	}
	// ensure red black tree properties
	fix_rb_tree(new_node);

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
	Node *new_node
) {
	debug("BLACK is %d and RED is %d", BLACK, RED);
	check(new_node, "Insert node: new_node == NULL");
	Node *parent = new_node->parent;
	
	// case 1.)
	if(new_node->parent == NULL) {
		debug("Insert node: case 1.)");
		new_node->color = BLACK;
		root = new_node;
		return;
	}

	// case 2.) The parent is black. There is no need to fix the tree.
	if(parent->color == BLACK) {
		debug("Insert node: case 2.)");
		return;
	}
	
	// case 3.) U and P of N is red -> color U,P black and GP red.
	if(uncle(new_node) != NULL && uncle(new_node)->color == RED) {
		debug("Insert node: case 3.)");
		if(grandparent(new_node)->parent != NULL) {
			new_node->parent->color = BLACK;
			uncle(new_node)->color = BLACK;
			grandparent(new_node)->color = RED;
			fix_rb_tree(grandparent(new_node));
			return;
		} else {
			grandparent(new_node)->color = BLACK;
			new_node->color = BLACK;
		}
	}

	// case 4.) N has non or a black U while N is the right child of his red P and P is left of GP
	if(uncle(new_node) == NIL || uncle(new_node)->color == BLACK) {
		if(
			new_node == new_node->parent->right && 
			new_node->parent->color == RED &&
			new_node->parent == grandparent(new_node)->left
		) {
			debug("Fixing: case 4.) rotate left.");
			debug("******** before rotate:  new_node->parent: %s ", new_node->parent->name);
			rotate_left(new_node->parent);
			new_node = new_node->left;
			debug("******** new_node: %s ", new_node->name);
			debug("******** new_node->left: %s ", new_node->left->name);
			debug("******** new_node->right: %s ", new_node->right->name);
			debug("******** new_node->parent: %s ", new_node->parent->name);
			debug("******** new_node->parent->left: %s ", new_node->parent->left->name);
			debug("******** new_node->parent->right: %s ", new_node->parent->right->name);
			debug("******** new_node->parent->parent: %s ", new_node->parent->parent->name);
			debug("******** new_node->parent->parent->right: %s ", new_node->parent->parent->right->name);
			debug("******** new_node->parent->parent->left: %s ", new_node->parent->parent->left->name);
			
		} else if(
					new_node == new_node->parent->left && 
					new_node->parent->color == RED &&
					new_node->parent == grandparent(new_node)->right) {
			debug("Fixing: case 4.) rotate right.");
			rotate_right(new_node->parent);
			new_node = new_node->right;
			debug("******** new_node: %s ", new_node->name);
			debug("******** new_node->left: %s ", new_node->left->name);
			debug("******** new_node->right: %s ", new_node->right->name);
			debug("******** new_node->parent: %s ", new_node->parent->name);
			debug("******** new_node->parent->left: %s ", new_node->parent->left->name);
			debug("******** new_node->parent->right: %s ", new_node->parent->right->name);
			debug("******** new_node->parent->parent: %s ", new_node->parent->parent->name);
			debug("******** new_node->parent->parent->right: %s ", new_node->parent->parent->right->name);
			debug("******** new_node->parent->parent->left: %s ", new_node->parent->parent->left->name);
		}

		// 5.) N has non or a black U and is the left child of his red P and P is left of GP.
		if(uncle(new_node) == NIL || uncle(new_node)->color == BLACK) {
			if(
				new_node == new_node->parent->left &&
				new_node->parent->color == RED &&
				new_node->parent == grandparent(new_node)->left
			) {
				debug("Fixing: case 5.) rotate right.");
				debug("******** before rotate:  grandparent(new_node): %s ", grandparent(new_node)->name);
				rotate_right(grandparent(new_node));
				debug("******** new_node: %s ", new_node->name);
				debug("******** new_node->left: %s ", new_node->left->name);
				debug("******** new_node->right: %s ", new_node->right->name);
				debug("******** new_node->parent: %s ", new_node->parent->name);
				debug("******** new_node->parent->left: %s ", new_node->parent->left->name);
				debug("******** new_node->parent->right: %s ", new_node->parent->right->name);
				// swap colors of P and former GP
				Color tmp = new_node->parent->color;
				new_node->parent->color = new_node->parent->right->color;
				new_node->parent->right->color = tmp;
			} else if (
				new_node == new_node->parent->right &&
				new_node->parent->color == RED &&
				new_node->parent == grandparent(new_node)->right
			) {
				debug("Fixing: case 5.) rotate left.");
				rotate_left(grandparent(new_node));
				// swap colors of P and former GP
				Color tmp = new_node->parent->color;
				new_node->parent->color = new_node->parent->left->color;
				new_node->parent->left->color = tmp;
			}
		}
	}

error:
	return;
}

//-------------------------------------------------------------------------------------------------
void rotate_left(
	Node *n
) {
	check_node(n, "Rotate left: n == NIL");
	check_node(n->right, "Rotate left: n->right == NIL")	

	Node *p = n->parent;
	Node *r = n->right->left;

	n->parent = n->right;
	n->right->parent = p;
	if(p) {
		p->left = n->right;
	} else {
		root = n->right;
	}
	n->right->left = n;
	n->right = r;
	
error:
	return;	
}

//-------------------------------------------------------------------------------------------------
void rotate_right(
	Node *n
) {
	check_node(n, "Rotate right: n == NIL");
	check_node(n->left, "Rotate right: n->left == NIL")	

	Node *p = n->parent;
	Node *l = n->left->right;

	n->parent = n->left;
	n->left->parent = p;
	if(p) {
		p->right = n->left;
	} else {
		root = n->left;
	}
	n->left->right = n;
	n->left = l;
	
error:
	return;	
}

//--------------------------------------------------------------------------------------------------
void delete_tree(
) {
	check(root, "Delete tree: Node ==  NULL");
	delete_tree_rec(root);

error:
	return;	
}

//-------------------------------------------------------------------------------------------------
void delete_tree_rec(
	Node *n
) {
	
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

