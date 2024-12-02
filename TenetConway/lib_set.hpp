#include <iostream>
#include <stdlib.h>

#define MAX_ELEMENTS 8      //Como a lib_set.hpp será usada para definir conjuntos vizinhos, o máximo de 
                            //vizinhos que uma célula pode ter No conway's game of life é 8

struct set_t{
    short cardinality;          //Cardinalidade do conjunto  
    short elem[MAX_ELEMENTS];   //Vetor com os elementos do conjunto
};

/*
    Cria um conjunto struct set_t vazio, com cardinalidade 0 e os elementos marcados como -1. 
    Retorna o ponteiro para o conjunto criado e NULL em caso de erro.  
*/
struct set_t *create_empty_set();

/*
    Libera memória alocada pelo conjunto struct set_t *s. 
    Retorna NULL;
*/
struct set_t *destroy_set(struct set_t *s);

/*
    Insere o elemento value no conjunto struct set_t *s. 
    Retorna 1 em caso de sucesso e 0 em caso de erro.  
*/
int insert_element_set(struct set_t *s, short value);

/*
    Remove o último elemento do conjunto struct set_t *s;
*/
void remove_element_set(struct set_t *s);

/*
    Recebe o conjunto struct set_t s e strutct set_t a e cria o conjunto diferença entre os 2. 
    Retorna um conjunto struct set_t;
*/
struct set_t create_set_diff(struct set_t *s, struct set_t *a);

/*
    Imprime os elementos do conjunto struct set_t *s;
*/
void print_set(struct set_t *s);

/*
    "Esvazia" o conjunto, deixando com cardinalidade 0 e sem elementos.
*/
void reset_set(struct set_t *s);