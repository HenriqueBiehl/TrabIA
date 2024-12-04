#include <iostream> 
#include "z3++.h"
#include "lib_set.hpp"

#pragma once

/*
    Cria a expressão de disjunção dos elementos do conjunto s. 
    Recebe um z3::context& c e um conjunto struct set_t s.
    Retorna uma expressão de disjunção z3::expr dos elementos x_i  do conjunto s.
*/
z3::expr set_disjunction(z3::context& c,  struct set_t *s);

/*
    Cria a expressão de disjunção dos elementos negados do conjunto s. 
    Recebe um z3::context& c e um conjunto struct set_t s.
    Retorna uma expressão de disjunção z3::expr dos elementos x_i  do conjunto s.
*/
z3::expr set_disjunction_not_elem(z3::context& c,  struct set_t *s);

/*
    Adiciona a expressão de loneliness ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Loneliness(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c);

/*
    Adiciona a expressão de Overcrowding ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Overcrowding(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c);

/*
    Adiciona a expressão de Stagnation ao solver f de um z3::context&c. 
    Recebe uma célula cell, o conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Stagnation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c);

/*
    Adiciona a expressão de Preservation ao solver f de um z3::context&c. 
    Recebe uma célula cell, o conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Preservation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c);

/*
    Adiciona a expressão de Life ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Life(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c);

/*
    Gera o conjunto de vizinhos de um elemento x_ij (representado como i*m + j na matriz vetor).
    Recebe o struct set_t *s onde será montado o conjunto, as coordenadas i,j e o tamanho n x m. 
*/
void generate_neighbour_set(struct set_t *s, short i, short j, int n, int m);

/*
    Cria a formula para o solver f dado um contexto c. 
    Recebe a tabela com 0's e 1's, um conjunto vazio struct set_t s,
    onde serão criados os conjuntos vizinhos e as dimensões n e m da tabela. 
*/
void create_formula(short *table, struct set_t *s, int n, int m, z3::solver& f, z3::context& c);