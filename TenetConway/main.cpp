#include <iostream> 
#include <stdlib.h> 
#include "lib_set.hpp"
#include "z3++.h"

using namespace std; 

short read_value();
void set_table(short *table, short n, short m);
void print_table(short *table, short n, short m);


short read_value(){
    short v; 

    cin >> v; 

    while(v != 0 && v != 1){

        cout << "Valores devem ser entre 1 e 0" << endl; 
        cin >> v; 
    }

    return v; 
}

void set_table(short *table, short n, short m){
    short v; 

    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            v = read_value(); 
            table[i*m + j] = v;
        }
    }
}

void print_table(short *table, short n, short m){

    cout << endl;
    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            cout << table[i*m + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

z3::expr set_disjunction(z3::context& c,  struct set_t *s){
    z3::expr_vector var(c); 

    for(int i =0; i < s->cardinality; ++i){
        var.push_back(c.bool_const(("x_" + to_string(s->elem[i])).c_str()));
    }
    
    return z3::mk_or(var);
}

z3::expr set_disjunction_not_elem(z3::context& c,  struct set_t *s){
    z3::expr_vector var(c); 

    for(int i =0; i < s->cardinality; ++i){
        var.push_back(!c.bool_const(("x_" + to_string(s->elem[i])).c_str()));
    }

    return z3::mk_or(var);
}

void Loneliness(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){

    if(s->cardinality < card)
        return;

    if(subset->cardinality == card){
        f.add(set_disjunction(c, subset));
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Loneliness(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

void Overcrowding(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    if(s->cardinality < card)
        return;

    if(subset->cardinality == card){
        f.add(set_disjunction_not_elem(c, subset));
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Overcrowding(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

void Stagnation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    if(s->cardinality < card)
        return;

    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset);

        z3::expr x = c.bool_const(("x_" + to_string(cell)).c_str());
        x = x || set_disjunction(c, &diff);
        x = x || set_disjunction_not_elem(c, subset);
        f.add(x);
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Stagnation(cell, s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

void Preservation(short cell, struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    if(s->cardinality < card)
        return;

    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset); 

        z3::expr x = !c.bool_const(("x_" + to_string(cell)).c_str());
        x = x || set_disjunction(c, &diff);
        x = x || set_disjunction_not_elem(c, subset);
        f.add(x);
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Preservation(cell, s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}

void Life(struct set_t *s, struct set_t *subset, short start, short card, z3::solver& f, z3::context& c){
    
    if(s->cardinality < card)
        return;

    if(subset->cardinality == card){
        struct set_t diff = create_set_diff(s, subset); 

        z3::expr x = set_disjunction(c, &diff) || set_disjunction_not_elem(c, subset);
        f.add(x);
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        Life(s, subset, i+1, card, f, c); 
        remove_element_set(subset);
    }
}
void generate_neighbour_set(struct set_t *s, short i, short j, int n, int m){

    if((i-1) >= 0){

        if((j-1) >= 0)
            insert_element_set(s, (i-1)*m + j-1);

        insert_element_set(s, (i-1)*m + j);

        if((j+1) < m)
            insert_element_set(s, (i-1)*m + j+1);

    }

    if((j-1) >= 0)
        insert_element_set(s, i*m + j-1);


    if((j + 1) < m)
        insert_element_set(s, i*m + j+1);

    if((i+1) < n){
        if((j-1) >= 0)
            insert_element_set(s, (i+1)*m + j-1);

        insert_element_set(s, (i+1)*m + j);

        if((j+1) < m)
            insert_element_set(s, (i+1)*m + j+1);
    }

}

void create_formula(short *table, struct set_t *s, int n, int m, z3::solver& f, z3::context& c){
    struct set_t *subset = create_empty_set(); 

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){   
            generate_neighbour_set(s, i, j, n, m); 

            if(table[i*m + j]){
                Loneliness(s, subset, 0, 7, f, c);
                Overcrowding(s, subset, 0, 4, f, c);
                Stagnation(i*m+j, s, subset, 0, 2, f,c);

            }
            else {
                Preservation(i*m+j, s, subset, 0, 2, f,c);
                Life(s, subset, 0, 3, f, c);
            }
            reset_set(s);
            reset_set(subset);
        }
    }

    destroy_set(subset);
}


int main(){
    short n;
    short m; 
    short *table; 
    z3::context c;

    struct set_t *s = create_empty_set(); 

    cin >> n; 
    cin >> m; 

    std::vector <z3::expr>  variables; 

    for (int i = 0; i < n*m ; ++i){
        string var_name = "x_" + to_string(i); 
        variables.push_back(c.bool_const(var_name.c_str()));
    }

    table = (short *) malloc(sizeof(short)*n*m);

    if(!table){
        cout << "Falha ao alocar tabela!" << endl;
        exit(1);
    }

    set_table(table, n, m);
    print_table(table, n, m);

    z3::solver f(c);


    create_formula(table, s, n, m, f, c);

    if (f.check() == z3::sat) {
        std::cout << "Satisfatível: " << std::endl;

         z3::model model = f.get_model();

        cout << endl;
        for(short i=0; i < n; i++){
            for(short j=0; j < m; j++){
                z3::expr value = model.eval(variables[i*m +j]);
                std::cout << (value.bool_value() == Z3_L_TRUE ? "1" : "0") << " ";        }
            cout << endl;
        }
        cout << endl;

    } else {
        std::cout << "Insatisfatível." << std::endl;
    }

    free(table);
    destroy_set(s);

    return 0;
}