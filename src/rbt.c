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
static Node _NIL = { NULL, NULL, NULL, BLACK, ""};

// helper functions
//---------------------------------------------------------------------------------------------------
static Node *grandparent(Node *n) {
	check(n, "Node == NULL\n");
	check(n->parent, "n->parent == NULL\n");
	return n->parent->parent;

error:
	return NULL;
}

//---------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------
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
	Node *new_node = malloc(sizeof(Node));
	check_mem(new_node);
	new_node->parent = NULL; new_node->left = NIL; new_node->right = NIL; new_node->color = RED;
	strncpy(new_node->name, new_name, len);
	new_node->name[len] = '\0';

	// case 1
	if(!root) {
		new_node->color = BLACK;
		root = new_node;
		return;
	}

	// root existed
	// search for the proper place to include the node
	Node *current = root;
	Node *parent = root;
	int res = strcmp(new_node->name, root->name);
	if(res < 0) {
		current = root->left;
	} else if(res > 0) {
		current = root->right;
	} else {
		// new node has the same name as root
		// nothing to do -> free new_node and return
		free(new_node);
		return;
	}
	
	while (current != NIL) {
		parent = current;
		res = strcmp(new_node->name, current->name);
		if(res < 0) {
			current = current->left;
		} else if(res > 0) {
			current = current->right;
		} else {
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

	// In case the parent is black there is no need to balance the tree
	if(parent->color == BLACK) {
		return;
	}

	// Parent and child are red. Requirement 2.) violated. -> fixing...
	fix_rb_tree(new_node);

error:
	return;
}

//---------------------------------------------------------------------------------------------------
void fix_rb_tree(Node *new_node) {
	
}

//---------------------------------------------------------------------------------------------
static void delete_tree_rec(Node *n) {
	
}

//---------------------------------------------------------------------------------------------------
void delete_tree() {
	check(root, "Delete tree: root is NULL");
	delete_tree_rec(root);

error:
	return;	
}


//----------------------------------------------------------------------------------------
static void print_names_rec(Node *n) {
	if(!n) {
		return;
	}
	print_names_rec(n->left);
	printf("%s\n", n->name);
	print_names_rec(n->right);
}

//---------------------------------------------------------------------------------------------------
void print_names() {
	check(root, "Print tree: root is NULL");
	print_names_rec(root);

error:
	return;	
}
