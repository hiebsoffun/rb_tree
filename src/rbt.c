/*
 * 	C implementation of a red-black tree.
 *	
 * 	The colored nodes of a red-black tree have three rquirements:
 *	1.) All leaves are black
 *	2.) Child nodes of a red node are black
 * 	3.) Given a node, every path to its leaves counts the same number of black nodes. 
*/

#include "rbt.h"

#define NIL &_NIL

// tree root
static Node *root = NULL;

// the sentinel node
static Node _NIL = { NULL, NULL, NULL, BLACK, ""};

// helper functions
static Node *grandparent(Node *n) {
	check(n, "Node == NULL\n");
	check(n->parent, "n->parent == NULL\n");
	return n->parent->parent;

error:
	return NULL;
}

static Node *uncle(Node *n) {
	Node *gp = grandparent(n);
	check(gp, "Grandparent == NULL\n");
	if(n->parent == gp->left) {
		return gp->right;
	} else {
		return gp->left;
	}

error:
	return NULL;
}

/*
* An inserted node is always red. This violates maybe requirement 2.) --> repairing necessary
* Given a Node n to be inserted, five cases have to be considered:
* 1.) root == NULL, (obvious)
* 2.) The parent of n is black -> nothing to do 
*/
void insert(const char *new_name) {
	// Is name exceeding MAX_NAME_LENGTH chars?
	unsigned int len = strlen(new_name);
	check(len<MAX_NAME_LENGTH, "Insert: Name exceeds maximal length.\n"); // MAX_NAME_LENGTH is 59 bytes + '\0'

	// Allocate and initialize a new node
	Node *n = malloc(sizeof(Node));
	check_mem(n);
	n->parent = NULL; n->left = NIL; n->right = NIL; n->color = RED;
	strncpy(n->name, new_name, len);
	n->name[len] = '\0';

	// case 1
	if(!root) {
		n->color = BLACK;
		root = n;
		return;
	}

error:
	return;
}
