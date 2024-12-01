#include <stdio.h> 
#include <stdlib.h> 
#include "lib_set.h"

short read_value();
void set_table(short *table, short n, short m);
void print_table(short *table, short n, short m);


short read_value(){
    short v; 

    scanf("%hd", &v); 

    while(v != 0 && v != 1){
        printf("Valores devem ser entre 1 e 0\n");
        scanf("%hd", &v); 
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

    printf("\n");
    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            printf("%hd ", table[i*m + j]);
        }
        printf("\n");
    }
    printf("\n");
}

struct set_t *neighbour_set(short index, short i, short j, short m){




    
}

short *return_best_table(short *table, short  n, short m){
    //short *best_result; 
    //int cells_alive = 0;
    short x, index;
    
    short *aux_table = malloc(sizeof(short)*n*m);


    for(short i=0; i < n; i++){
        for(short j=0; j < m; j++){
            printf("%hd ", table[i*m + j]);
            index = i*m + j;
            x =  table[index] == 0;
            // n = neighbour_set(index, i, j, n, m);

            if(x == 0){
                //Preservation && Life
            }
            else{
                //Loneliness  && Stagnation && Overcrowding
            }
        }
        printf("\n");
    }

}



int main(){
    short n;
    short m; 
    short *table; 

    scanf("%hd", &n);
    scanf("%hd", &m); 

    table = malloc(sizeof(short)*n*m);

    if(!table){
        puts("Falha ao alocar tabela\n");
        exit(1);
    }

    set_table(table, n, m);
    print_table(table, n, m);

    free(table);

    return 0;
}

/*
4 6
0 0 0 0 0 0
0 0 1 1 0 0
0 0 0 1 1 0
0 0 0 0 0 0
*/
