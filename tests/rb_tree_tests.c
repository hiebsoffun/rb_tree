#include "../src/rbt.h"

int main(int argc, char *argv[]) {

	insert("Konrad");
	insert("Heinrich");
	insert("Karl");
	insert("Friedrich");
	insert("Anton");
	insert("Anette");
	insert("Olga");
	insert("Eva");
	insert("Adam");

	print_names();
	delete_tree();

return 0;    
}

