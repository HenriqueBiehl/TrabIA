#include <z3++.h>
#include <iostream>

int main() {
    try {
        z3::context ctx;

        // Variáveis booleanas
        z3::expr x = ctx.bool_const("x");
        z3::expr y = ctx.bool_const("y");
        z3::expr z = ctx.bool_const("z");

        // Fórmula lógica: (x OR y) AND (NOT x OR z) AND (NOT y OR NOT z)
        z3::expr formula = (x || y) && (!x || z) && (!y || !z);

        // Resolver problema de SAT
        z3::solver solver(ctx);
        solver.add(formula);

        // Checar satisfatibilidade
        if (solver.check() == z3::sat) {
            std::cout << "Satisfatível! Modelo:\n" << solver.get_model() << "\n";
        } else {
            std::cout << "Insatisfatível.\n";
        }

    } catch (z3::exception &ex) {
        std::cerr << "Erro: " << ex << "\n";
    }

    return 0;
}
