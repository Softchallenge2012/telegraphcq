#include<stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_RELATIONS 10
#define MAX_PREDICATES 10

class relation;

class join_edge {
    public :
        relation *rel1;
        relation *rel2;
        char predicate[100];
        int index;

        void init_from_stdin() {
            scanf("%s", predicate);
            // printf("%s\n", predicate);
            rel1 = rel2 = NULL;
        }

        relation *other(relation *r) {
            return rel1 == r ? rel2 : rel1;
        }
        void print();
};

class relation {
    public :
        char name[80];
        join_edge *edges_incident[MAX_RELATIONS - 1];
        int num_edges_incident;

        void init_from_stdin(join_edge **edges) {
            scanf("%s", name);
            // printf("%s\n", name);
            scanf("%d", &num_edges_incident);
            // printf("%d\n", num_edges_incident);
            for(int i = 0; i < num_edges_incident; i++) {
                int index;
                scanf("%d", &index);
                (edges[index]->rel1 ? edges[index]->rel2 : edges[index]->rel1) = this;
                edges_incident[i] = edges[index];
            }
        }
};

void join_edge::print() {
    printf("JOIN_EDGE between %s and %s : %s\n", rel1->name, rel2->name, predicate);
}

int num_edges;
join_edge* edges[MAX_RELATIONS-1];
relation* relations[MAX_RELATIONS];

int already_used[MAX_RELATIONS - 1];

void init_already_used() {
    for(int i = 0; i < num_edges; i++)
        already_used[i] = 0;
}

int num_rest_predicates;
char rest_predicates[MAX_PREDICATES][100];

int permutation[MAX_RELATIONS - 1];

int perm_index = 0;

void
init_permutation() {
    for(int i = 0; i < num_edges; i++) {
        permutation[i] = i;
        already_used[i] = 0;
    }
    perm_index = num_edges - 1;
}

void
swap(int *p1, int *p2) {
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// starting with the start element, resort
void
permutation_resort(int start) {
    // find the smallest; swap; recurse
    if(start == num_edges) {
        return;
    } else {
        int smallest = 100;
        int smallest_index = -1;
        for(int i = start; i < num_edges; i++) {
            if(smallest > permutation[i]) {
                smallest = permutation[i];
                smallest_index = i;
            }
        }

        swap(&permutation[start], &permutation[smallest_index]);

        permutation_resort(start+1);
    }
}

int
check_if_reverse_sorted(int start) {
    for(int i = start; i < num_edges-1; i++) {
        if(permutation[i] < permutation[i+1]) {
            return 0;
        }
    }
    return 1;
}

void 
print_permutation() {
    for(int i = 0; i < num_edges; i++) {
        printf("%d ", permutation[i]);
    }
    printf("\n");
}

int 
next_permutation() {
    // beginnign from the end, find the first place where i+1'th element is larger than i'th element
    // switch
    // resort the rest
    int found = 100;
    for(int i = num_edges - 1; i >= 1; i--) {
        if(permutation[i] > permutation[i-1]) {
            found = i - 1;
            break;
        }
    }
    if(found == 100) 
        return 0;

    // find the next largest element after permutation[found] in the sequence after
    int next_largest_than_found_index = found+1; // found + 1 is definitely larger that found
    for(int i = found + 2; i < num_edges; i++) { 
        if((permutation[i] > permutation[found]) && (permutation[i] < permutation[next_largest_than_found_index]))
            next_largest_than_found_index = i;
    }

    assert(next_largest_than_found_index != found);
    
    // swap the two
    swap(&permutation[next_largest_than_found_index], &permutation[found]);

    // resort rest of the sequence
    permutation_resort(found+1);
    return 1;
}


int
find_smallest_unused_start_relation(relation *r) {
    int smallest = 100;
    for(int i = 0; i < r->num_edges_incident; i++) {
        if(!already_used[r->edges_incident[i]->index]) {
            // check this edge itself
            if(smallest > permutation[r->edges_incident[i]->index]) {
                smallest = permutation[r->edges_incident[i]->index];
            }

            // temporarily set this to avoid recursion
            already_used[r->edges_incident[i]->index] = 1;

            // recurse on the other relation
            int s = find_smallest_unused_start_relation(r->edges_incident[i]->other(r));
            if(smallest > s) {
                smallest = s;
            }

            already_used[r->edges_incident[i]->index] = 0;
        }
    }
    return smallest;
}


void
print_query_for_current_permutation(int break_point) {
    join_edge *break_edge = 0;
    for(int i = 0; i < num_edges; i++) {
        if(permutation[i] == break_point) {
            break_edge = edges[i];
            already_used[i] = 1;
            break;
        }
    }

    // break_edge->print();

    // once found this, recurse on the two sides.
    // find the smallest unused on both sides and recurse on that number
    int smallest1 = find_smallest_unused_start_relation(break_edge->rel1);
    int smallest2 = find_smallest_unused_start_relation(break_edge->rel2);

    // printf("%d %d\n", smallest1, smallest2);

    if(smallest1 != 100) {
        printf(" (");
        print_query_for_current_permutation(smallest1);
        printf(") ");
    } else {
        printf(" %s ", break_edge->rel1->name);
    }
    printf(" join ");
    if(smallest2 != 100) {
        printf(" (");
        print_query_for_current_permutation(smallest2);
        printf(")");
    } else {
        printf(" %s ", break_edge->rel2->name);
    }
    printf(" on %s", break_edge->predicate);
}

void
check_permutation_program() {
    init_permutation();
    do {
        print_permutation();
    } while (next_permutation());
}


int
main() {
    scanf("%d\n", &num_edges);
    scanf("%d\n", &num_rest_predicates);

    // printf("%d\n", num_edges);
    // printf("%d\n", num_rest_predicates);
    for(int i = 0; i < num_edges; i++) {
        edges[i] = new join_edge;
        edges[i]->init_from_stdin();
        edges[i]->index = i;
    }

    for(int i = 0; i < num_edges + 1; i++) {
        relations[i] = new relation;
        relations[i]->init_from_stdin(edges);
    }

    for(int i = 0; i < num_rest_predicates; i++) {
        scanf("%s", rest_predicates[i]);
    }


    init_permutation();
    do {
        printf("explain analyze select * from");
        init_already_used();
        print_query_for_current_permutation(0); 

        if(num_rest_predicates) {
            printf(" where ");
            for(int i = 0; i < num_rest_predicates-1; i++) {
                printf("%s and ", rest_predicates[i]);
            }
            printf("%s", rest_predicates[num_rest_predicates-1]);
        }

        printf(";\n");
    } while (next_permutation());
}
