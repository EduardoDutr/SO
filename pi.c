#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void gauss_legendre(int num_iteracoes) {
    long double a = 1.0;
    long double b = 1.0 / sqrtl(2.0);
    long double t = 0.25;
    long double p = 1.0;
    long double a_next, b_next, t_next, pi;

    for (int i = 0; i < num_iteracoes; i++) {
        a_next = (a + b) / 2;
        b_next = sqrtl(a * b);
        t_next = t - p * (a - a_next) * (a - a_next);
        a = a_next;
        b = b_next;
        t = t_next;
        p = 2 * p;
    }

    pi = (a + b) * (a + b) / (4 * t);
    printf("Pi com %d iteracoes: %.100Lf\n", num_iteracoes, pi);
}

int main() {
    int num_casas;
    printf("Digite o numero de casas decimais desejadas (ate 100): ");
    scanf("%d", &num_casas);

    if (num_casas < 1 || num_casas > 100) {
        printf("Numero de casas decimais invalido.\n");
        return 1;
    }

    //int num_iteracoes = ceil(log2l((double)num_casas * log10l(10.0)));
    gauss_legendre(10000);

    return 0;
}
