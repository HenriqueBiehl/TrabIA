#include <z3++.h>
#include <iostream>

using namespace std;
using namespace z3;

int main() {
    // Inicializar o contexto Z3
    context c;

    // Definir variáveis para representar o estado das células (0: morta, 1: viva)
    expr x = c.bool_const("x"); // célula central

    // Criar um array de variáveis booleanas para os vizinhos
    expr n[8] = {
        c.bool_const("n0"),
        c.bool_const("n1"),
        c.bool_const("n2"),
        c.bool_const("n3"),
        c.bool_const("n4"),
        c.bool_const("n5"),
        c.bool_const("n6"),
        c.bool_const("n7")
    };

    // Criar a fórmula para a regra de loneliness
    // Loneliness: Uma célula com menos de 2 vizinhos vivos (ou seja, pelo menos 7 vizinhos mortos)
    expr_vector lonely_clauses(c);
    for (int i = 0; i <= 1; i++) {
        // Gerar todas as combinações de i vizinhos vivos
        for (unsigned j = 0; j < (1 << 8); j++) { // 2^8 combinações de vizinhos
            int count = 0;
            expr_vector clause(c);
            for (int k = 0; k < 8; k++) {
                if (j & (1 << k)) {
                    clause.push_back(n[k]);
                    count++;
                } else {
                    clause.push_back(!n[k]);
                }
            }
            if (count == i) {
                lonely_clauses.push_back(mk_or(clause));
            }
        }
    }

    // Fórmula final para loneliness
    expr loneliness = mk_and(lonely_clauses);

    // Mostrar a fórmula
    std::cout << "Fórmula de Loneliness:" << std::endl;
    std::cout << loneliness << std::endl;

    return 0;
}