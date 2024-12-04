#include "lib_game_of_life_operations.hpp"

/*
    Cria a expressão de disjunção dos elementos do conjunto s. 
    Recebe um z3::context& c e um conjunto struct set_t s.
    Retorna uma expressão de disjunção z3::expr dos elementos x_i  do conjunto s.
*/
z3::expr set_disjunction(z3::context& c,  struct set_t *s){
    z3::expr_vector var(c); 

    for(int i =0; i < s->cardinality; ++i){
        //Váriavel da expressão é nomeada x_{s->elem[i]} 
        var.push_back(c.bool_const(("x_" + std::to_string(s->elem[i])).c_str()));
    }
    
    return z3::mk_or(var);
}

/*
    Cria a expressão de disjunção dos elementos negados do conjunto s. 
    Recebe um z3::context& c e um conjunto struct set_t s.
    Retorna uma expressão de disjunção z3::expr dos elementos x_i  do conjunto s.
*/
z3::expr set_disjunction_not_elem(z3::context& c,  struct set_t *s){
    z3::expr_vector var(c); 

    for(int i =0; i < s->cardinality; ++i){
        //Váriavel da expressão é nomeada x_{s->elem[i]} 
        var.push_back(!c.bool_const(("x_" + std::to_string(s->elem[i])).c_str()));
    }

    return z3::mk_or(var);
}

/*
    Adiciona a expressão de loneliness ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Loneliness(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){

    //Se o conjunto struct set_t s tem cardinalidade menor que card, retorna
    //Não é possivel realizar a operação de loneliness com o conjunto s. 
    if(s->cardinality < card)
        return;

    //Realiza a operação quando a cardinalidade subset for igual a de card
    if(subset->cardinality == card){
        //Realiza disjunção entre os elementos do subset dado o contexto c e adiciona a formula f como conjunção.
        f.add(set_disjunction(c, subset));
        return;
    }

    //Itera sobre os elementos de s de modo a adicionar seus elemento ao subset. 
    //Realiza backtracking para criar todas as possibilidades
    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Loneliness(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

/*
    Adiciona a expressão de Overcrowding ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Overcrowding(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    //Se o conjunto struct set_t s tem cardinalidade menor que card, retorna
    //Não é possivel realizar a operação de loneliness com o conjunto s. 
    if(s->cardinality < card)
        return;
    
    //Realiza a operação quando a cardinalidade subset for igual a de card
    if(subset->cardinality == card){
        //Realiza disjunção entre os elementos negados do subset dado o contexto c e adiciona a formula f (como conjunção).
        f.add(set_disjunction_not_elem(c, subset));
        return;
    }

    //Itera sobre os elementos de s de modo a adicionar seus elemento ao subset. 
    //Realiza backtracking para criar todas as possibilidades
    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Overcrowding(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

/*
    Adiciona a expressão de Stagnation ao solver f de um z3::context&c. 
    Recebe uma célula cell, o conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Stagnation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    //Se o conjunto struct set_t s tem cardinalidade menor que card, retorna
    //Não é possivel realizar a operação de loneliness com o conjunto s.     
    if(s->cardinality < card)
        return;

    //Realiza a operação quando a cardinalidade subset for igual a de card
    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset); //Cria o conjunto diferença entre s e o subset. 

        //Cria a expressão x com x_cell (para representar a célula)
        z3::expr x = c.bool_const(("x_" + std::to_string(cell)).c_str());
        x = x || set_disjunction(c, &diff);     //Adiciona a disjunção entre x e a disjunção dos elementos de diff 
        x = x || set_disjunction_not_elem(c, subset);   //Adiciona a disjunção entre x e a disjunção dos elementos negados do subset
        f.add(x);   //Adiciona x a formula f
        return;
    }

    //Itera sobre os elementos de s de modo a adicionar seus elemento ao subset. 
    //Realiza backtracking para criar todas as possibilidades
    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Stagnation(cell, s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

/*
    Adiciona a expressão de Preservation ao solver f de um z3::context&c. 
    Recebe uma célula cell, o conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Preservation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    //Realiza a operação quando a cardinalidade subset for igual a de card
    if(s->cardinality < card)
        return;

    //Realiza a operação quando a cardinalidade subset for igual a de card
    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset); //Cria o conjunto diferença entre s e o subset.
        
        //Cria a expressão x com !x_cell (para representar a célula negada)
        z3::expr x = !c.bool_const(("x_" + std::to_string(cell)).c_str()); 
        x = x || set_disjunction(c, &diff);     //Adiciona a disjunção entre x e a disjunção dos elementos de diff 
        x = x || set_disjunction_not_elem(c, subset);   //Adiciona a disjunção entre x e a disjunção dos elementos negados do subset
        f.add(x);   //Adiciona x a formula f
        return;
    }
    
    //Itera sobre os elementos de s de modo a adicionar seus elemento ao subset. 
    //Realiza backtracking para criar todas as possibilidades
    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Preservation(cell, s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

/*
    Adiciona a expressão de Life ao solver f de um z3::context&c. 
    Recebe um conjunto struct set_t de vizinhos s, um conjunto struct set_t subset que representa um subconjunto de s
    O índice de inicio start para iterar  sobre o conjunto via backtracking. Cardinalidade máxima do subconjunto subset card. 
*/
void Life(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){

     //Realiza a operação quando a cardinalidade subset for igual a de card   
    if(s->cardinality < card)
        return;
    
    //Realiza a operação quando a cardinalidade subset for igual a de card   
    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset); //Cria o conjunto diferença entre s e o subset.
        
        //Adiciona a disjunção entre a disjunção dos elementos de diff e a disjunção dos elementos negados do subset
        z3::expr x = set_disjunction(c, &diff) || set_disjunction_not_elem(c, subset);
        f.add(x);   //Adiciona x a formula f
        return;
    }
    
    //Itera sobre os elementos de s de modo a adicionar seus elemento ao subset. 
    //Realiza backtracking para criar todas as possibilidades
    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Life(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

/*
    Gera o conjunto de vizinhos de um elemento x_ij (representado como i*m + j na matriz vetor).
    Recebe o struct set_t *s onde será montado o conjunto, as coordenadas i,j e o tamanho n x m. 
*/
void generate_neighbour_set(struct set_t *s, short i, short j, int n, int m){

    //Avalia se a posição i,j possui uma linha acima (se i-1 for negativo, significa que não há)
    if((i-1) >= 0){

        //Avalia se a posição i-1,j possui um vizinho a esquerda
        if((j-1) >= 0)
            insert_element_set(s, (i-1)*m + j-1);
        
        //Adiciona o element logo acima de i,j
        insert_element_set(s, (i-1)*m + j);
        
        //Avalia se a posição i-1,j possui um vizinho a direita
        if((j+1) < m)
            insert_element_set(s, (i-1)*m + j+1);

    }

    //Avalia se a posição i,j possui um vizinho a esquerda
    if((j-1) >= 0)
        insert_element_set(s, i*m + j-1);

    //Avalia se a posição i,j possui um vizinho a direita
    if((j + 1) < m)
        insert_element_set(s, i*m + j+1);

    //Avalia se a posição i,j possui uma linha abaixo (se i+1 for igual que n, significa que não há)
    if((i+1) < n){
            
        //Avalia se a posição i+1,j possui um vizinho a esquerda
        if((j-1) >= 0)
            insert_element_set(s, (i+1)*m + j-1);
        
        //Adiciona o element logo acima de i,j
        insert_element_set(s, (i+1)*m + j);
        
        //Avalia se a posição i+1,j possui um vizinho a direita
        if((j+1) < m)
            insert_element_set(s, (i+1)*m + j+1);
    }

}

/*
    Cria a formula para o solver f dado um contexto c. 
    Recebe a tabela com 0's e 1's, um conjunto vazio struct set_t s,
    onde serão criados os conjuntos vizinhos e as dimensões n e m da tabela. 
*/
void create_formula(short *table, struct set_t *s, int n, int m, z3::solver& f, z3::context& c){
    struct set_t *subset = create_empty_set(); 

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){   

            //Cria o conjunto de vizinhos para a célula i*m+j 
            generate_neighbour_set(s, i, j, n, m); 

            //Avalia se a celula i*m+j está viva
            if(table[i*m + j]){

                //Realiza operações para células vivas
                Loneliness(s, subset, 0, 7, f, c);
                Overcrowding(s, subset, 0, 4, f, c);
                Stagnation(i*m+j, s, subset, 0, 2, f,c);

            }
            else {

                //Caso contrário, realiza operações para célula morta
                Preservation(i*m+j, s, subset, 0, 2, f,c);
                Life(s, subset, 0, 3, f, c);
            }
            reset_set(s);
            reset_set(subset);
        }
    }

    destroy_set(subset);
}