#include <time.h>

#include "../src/rbt.h"

#define N_TESTRUNS 5

typedef struct _Testrecord {
	unsigned int n_nodes;
	time_t start;
	time_t end;
} Testrecord;

void test_1(void) {
    insert("Konrad");
    print_names();
    insert("Heinrich");
    print_names();
    insert("Karl");
    print_names();
    insert("Anette");
    print_names();
    insert("Friedrich");
    print_names();
    insert("Anton");
    print_names();
    insert("Olga");
    print_names();
    insert("Eva");
    print_names();
    insert("Adam");
    print_names();
    delete("Anton");
    print_names();
}

char *rnd_string(void) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyz";

    int len = rand() % MAX_NAME_LENGTH;
    char *string=(char *)malloc((len+1)*sizeof(char));
    int i;
    for(i=0; i<len; i++) {
        int letter_index = rand() % strlen(charset);
        string[i] = charset[letter_index];
    }
    debug("Generated string: %s\n", string);
    string[i] = '\0';

return string;
}

void test_2(void) {
    time_t t;
    
    time(&t);
    srand((unsigned int) t);
    int i;
    for(i=0; i<10000;i++) {
        char *s = rnd_string();
         insert(s);
        free(s);
    }
    //print_names();
}

void performance_test(Testrecord *t) {
	
}

void visualize_performance() {
	int i;
	Testrecord records[N_TESTRUNS] = { 
								{100, 0, 0},
								{1000, 0, 0},
								{10000, 0, 0},
								{100000, 0, 0},
								{1000000, 0, 0},
							};
	for(i=0; i<N_TESTRUNS; i++) {
		performance_test(&records[i]);
	}
	

	for(i=0; i<N_TESTRUNS; i++) {
		printf("N: %d, Start: %d, End: %d\n", records[i].n_nodes
											, records[i].start
											, records[i].end);
	}
};

int main() {

//	test_1();
//  test_2();
	visualize_performance();
    
//	delete_all();

return 0;    
}
