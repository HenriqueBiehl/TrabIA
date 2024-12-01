#include <stdio.h>
#include <stdlib.h>


struct set_t{
    short cardinality;
    struct setElement_t *first; 
    struct setElement_t *last;
}

struct setElement_t{
    short index; 
    struct setElement_t *prev; 
    struct setElement_t *prox; 
}

struct set_t *create_empty_set();

struct set_t *destroy_set(struct set_t *s);

struct setElement_t *create_set_element(short index);

int insert_element_set(struct set_t *s, short value);

void print_set(struct set_t *s);