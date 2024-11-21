#include "lib_set.h"

struct set_t *create_empty_set(){
    struct set_t *s; 

    s = malloc(sizeof(struct set_t));

    if(!s)
        return NULL;

    s->cardinality = 0;
    s->first = NULL;
    s->last = NULL;

    return s;
}

struct set_t *destroy_set(struct set_t *s){
    struct setElement_t *e, *aux; 

    e = s->first;

    while(e != NULL){
        aux = e; 
        e = e->prox;
        free(aux);
    }

    free(s);

    return NULL;
}

struct setElement_t *create_set_element(short index){
    struct setElement_t *e; 

    e = malloc(sizeof(struct setElement_t));

    if(!e)
        return NULL;

    e->cardinality = e;
    e->prev = NULL;
    e->prox = NULL;

    return e;
}

int insert_element_set(struct set_t *s, short value){
    struct setElement_t *elem; 

    if(s->first == NULL){
        elem = create_set_element(value);
        s->first = elem;
        s->last = s->first;
        s->cardinality++;
        return 1
    }
    else if(value > s->last->index){

        if(value == s->last->index)
            return 0;

        elem = create_set_element(value); 

        elem->prev = s->last;
        s->last->prox = elem;
        s->last = elem;
        s->cardinality++;

        return 1;
    }
    else if{
        struct setElement_t *aux, *previous; 

        previous = NULL;
        aux = s->first;
        for(short i=1; i < s->cardinality; ++i){
            
            if(aux->index == value){
                return 0;
            }
            else if(aux->index > value){
                
                elem = create_set_element(value);
                
                elem->prev = previous;
                elem->prox = aux;

                aux->prev = elem; 

                if(previous != NULL)
                    previous->prox = elem;
                else
                    s->first = elem;

                s->cardinality++;

                return 1;
            }

            neighbour = aux;
            aux = aux->prox;
        }
    }

    return 0;
}

void print_set(struct set_t *s){
    struct setElement_t *e;

    e = s->first;

    if(!e)
        printf("Empty set\n");


    while(e != NULL){
        printf("%hd ", e->index);
        e = e->prox;
    }
    printf("\n");

}
