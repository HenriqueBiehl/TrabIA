#include <iostream> 
#include <stdlib.h> 
#include <chrono>
#include <sys/resource.h>
#include "z3++.h"   //SAT solver utilizado Z3
#include "lib_set.hpp"
#include "lib_game_of_life_operations.hpp"

using namespace std; 
using namespace z3;
using namespace std::chrono;

void print_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "Memória usada (RSS): " << usage.ru_maxrss << " KB" << std::endl;
}

/*
    Operação para ler um valor 0 ou 1. 
    Retorna o valor lido.
*/
short read_value(){
    short v; 

    cin >> v; 

    //Repete a operação caso o valor lido não seja 0 ou 1
    while(v != 0 && v != 1){
        cout << "Valores devem ser entre 1 e 0" << endl; 
        cin >> v; 
    }

    return v; 
}

/*
    Incializa os valores da tabela table com dimensões n x m
*/
void set_table(short *table, short n, short m){
    short v; 

    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            v = read_value();  // Le o valor a ser adicionado na posição i*m + j
            table[i*m + j] = v;
        }
    }
}

/*
    Imprime os valores da tabela table com dimensões n x m
*/
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

int main(int argc, char **argv){
    short n;
    short m; 
    short *table; 
    unsigned int time; 

    if(argc == 2)
        time = atoi(argv[1]); //Opção de inserir um tempo máximo em segundos para testes personalizados
    else
        time = 300; //Tempo maximo padrão é 300 segundos (5 minutos)

    z3::config config; 
    z3::context c(config);

    auto start_time = steady_clock::now();

    // Tempo de execução máximo (5 minutos)
    auto max_duration = seconds(time);
    
    cout << "Max execution time: " << max_duration.count() << " segundos" << endl;
    struct set_t *s = create_empty_set(); 

    cin >> n; 
    cin >> m; 

    std::vector <z3::expr>  variables; 

    //Incializa as variaveis da formula SAT. Valores vão de 0 a n*m, 
    //onde cada uma representa uma célula do tabuleiro
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

    int min_true_vars = variables.size();
    expr_vector not_more_than(c);

    z3::solver f(c);

    create_formula(table, s, n, m, f, c);

    params p(c);
    //Adiciona timeout para resolução do problema (para ficar dentro da faixa de tempo de max duration)
    p.set("timeout",static_cast<unsigned>(duration_cast<milliseconds>(max_duration).count()));
    f.set(p);
    int true_vars; 
    int iterations = 0;

    while(f.check() == z3::sat){ 
        iterations++;
        z3::model model = f.get_model(); 
        auto current_time = steady_clock::now();
        auto elapsed = duration_cast<seconds>(current_time - start_time); // Calcular o tempo decorrido em segundos

        // Atualizar o tempo restante
        auto time_remaining = max_duration - elapsed;

        if (time_remaining <= seconds(0)) {
            std::cout << "Tempo limite total atingido.\n";
            break;
        }

        true_vars = 0; 
        for(const auto& var: variables){
            if(model.eval(var).is_true()) 
                true_vars++;
        }

        min_true_vars = true_vars;

        //Adiciona os resultados das váriveis x_i*m+j no tabuleiro como 0 ou 1
        for(short i=0; i < n; i++){
            for(short j=0; j < m; j++){
                z3::expr value = model.eval(variables[i*m +j]);
                table[i*m +j] = value.bool_value() == Z3_L_TRUE ? 1 : 0;        
            }
        }

        //Expressão inteira inicializada como valor 0 
        expr true_var_sum = c.int_val(0); 
        for(const auto& var: variables){
            //Soma as váriaveis verdadeiras em var. Usa do ite para converter para 1 váriveis verdadeiras e 0 para falsas.
            true_var_sum = true_var_sum + ite(var, c.int_val(1), c.int_val(0));
        }
        //Adiciona a restrição de true_var_sum ser menor que min_true_vars -1 para a proxima iteração
        not_more_than.push_back(true_var_sum <= c.int_val(min_true_vars - 1));

        //Adiciona expressão ao solver f
        f.add(mk_and(not_more_than));

        //Timeout é modificado para o tempo restante (max_duration do inicio menos o tempo gastado (elapsed))
        p.set("timeout",static_cast<unsigned>(duration_cast<milliseconds>(time_remaining).count()));
        f.set(p);
    }
    auto end = steady_clock::now();
    auto duration = end - start_time;

    print_memory_usage();
    cout.precision(3);
    cout << "Duração total: " << duration_cast<milliseconds>(duration).count()/1e3 << " segundos " << endl;

    //Calula quantas váriaveis verdadeiras há no tabuleiro
    true_vars = 0; 
    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            if(table[i*m +j])
                true_vars++;     
        }
    }
    
    cout << endl << "Melhor Solução Encontrada com " << true_vars << " células vivas em " << iterations << " iterações:" <<endl;
    
    print_table(table, n, m);

    free(table);
    destroy_set(s);

    variables.clear();
    f.reset();    
    return 0;
}