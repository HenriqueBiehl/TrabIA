#include <cstring>
#include "lib_set.hpp"

struct set_t *create_empty_set(){
    struct set_t  *s;

    s = (struct set_t *) malloc(sizeof(struct set_t)); 

    if(!s)
        return NULL; 

    s->cardinality = 0; 

    memset(s->elem, -1, sizeof(short)*MAX_ELEMENTS);

    return s;
}

struct set_t *destroy_set(struct set_t *s){
    
    free(s); 

    return NULL;
}

int insert_element_set(struct set_t *s, short value){

    if(s->cardinality + 1 > MAX_ELEMENTS){
        std::cout << "ERROR: MAX ELEMENTS (" << MAX_ELEMENTS << ") REACHED" << std::endl; 
        return 0; 
    }

    s->elem[s->cardinality] = value;
    s->cardinality++; 

    return 1;
}

void remove_element_set(struct set_t *s){

    if(s->cardinality == 0)
        return;

    s->elem[s->cardinality-1] = -1; 
    s->cardinality--;
}

void print_set(struct set_t *s){

    if(s->cardinality == 0)
        std::cout << "Empty set." << std::endl;    
    
    for(int i=0; i < s->cardinality; ++i){
        std::cout << s->elem[i] << " ";
    }

    std::cout << std::endl;
}

void reset_set(struct set_t *s){

    if(s == NULL)
        return;

    s->cardinality = 0; 

    memset(s->elem, -1, sizeof(short)*MAX_ELEMENTS);
}