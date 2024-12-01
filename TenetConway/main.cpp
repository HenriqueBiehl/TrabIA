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

/*z3::expr Loneliness(short cell, struct set_t neighbour_set, std::vector <z3::expr> variables){

}*/

void generate_all_subsets(struct set_t *s, struct set_t *subset, short start, short c){

    if(s->cardinality < c)
        return;

    if(subset->cardinality == c){
        print_set(subset); 
        return;
    }

    for(int i = start; i < s->cardinality; ++i){
        insert_element_set(subset, s->elem[i]);
        generate_all_subsets(s, subset, i+1, c); 
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

void test_neighbour_generation(struct set_t *s, int n, int m){

    generate_neighbour_set(s, 0, 0, n, m);
    cout << "Conjunto vizinho para 0,0: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, 0, (m-1)/2, n , m);
    cout << "Conjunto vizinho para 0,m/2: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, 0, (m-1), n , m);
    cout << "Conjunto vizinho para 0,m-1: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, (n-1)/2, 0, n , m);
    cout << "Conjunto vizinho para n/2,0: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, n-1, 0, n , m);
    cout << "Conjunto vizinho para n/2,m/2: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, (n-1)/2, (m-1)/2, n , m);
    cout << "Conjunto vizinho para n/2,m/2: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, n-1, (m-1)/2, n , m);
    cout << "Conjunto vizinho para n-1 ,m/2: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, (n-1)/2, (m-1), n , m);
    cout << "Conjunto vizinho para n/2,m-1: " << endl; 
    print_set(s); 
    reset_set(s);

    generate_neighbour_set(s, n-1, m-1, n , m);
    cout << "Conjunto vizinho para n-1,m-1: " << endl; 
    print_set(s); 
    reset_set(s);
}

void all_table_neighbours(struct set_t *s, int n, int m){
    struct set_t *subset = create_empty_set(); 

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){   
            cout << "Conjunto de vizinho para [" <<  i <<"][" << j << "]" << endl;
            generate_neighbour_set(s, i, j, n, m); 
            print_set(s); 

            cout <<"    Gerando subsets para [" <<  i <<"][" << j << "]" << endl;

            for(int c=2; c <= 7; ++c){
                cout<<"     para c: " << c << endl;
                generate_all_subsets(s, subset, 0, c);
            }
            cout << endl;

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

    table = (short *) malloc(sizeof(short)*n*m);

    if(!table){
        cout << "Falha ao alocar tabela!" << endl;
        exit(1);
    }

    set_table(table, n, m);
    print_table(table, n, m);

    //test_neighbour_generation(s, n, m);

    //all_table_neighbours(s, n, m);

    cout << "Váriaveis do z3 para tabela: " << endl;

    for (int i = 0; i < n*m ; ++i){
        string var_name = "x_" + to_string(i); 
        variables.push_back(c.bool_const(var_name.c_str()));
    } 

    for (int i = 0; i < n*m; ++i) {
        std::cout << variables[i] << "\n"; // Mostra x_0, x_1, ..., x_(n-1)
    }

    free(table);
    destroy_set(s);

    return 0;
}
