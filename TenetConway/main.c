#include <stdio.h> 
#include <stdlib.h> 

int read_value();
void set_table(int *table, int n, int m);
void print_table(int *table, int n, int m);


int read_value(){
    int v; 

    scanf("%d", &v); 

    while(v != 0 && v != 1){
        printf("Valores devem ser entre 1 e 0\n");
        scanf("%d", &v); 
    }

    return v; 
}

void set_table(int *table, int n, int m){
    int v; 

    for(int i=0; i < n; i++){
        for(int j=0; j < m; j++){
            v = read_value(); 
            table[i*m + j] = v;
        }
    }
}

void print_table(int *table, int n, int m){

    printf("\n");
    for(int i=0; i < n; i++){
        for(int j=0; j < m; j++){
            printf("%d ", table[i*m + j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(){
    int n;
    int m; 
    int *table; 

    scanf("%d", &n);
    scanf("%d", &m); 

    table = malloc(sizeof(int)*n*m);

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
