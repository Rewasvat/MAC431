#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    unsigned long num_steps;
    int i; 
    register double x, step, sum = 0.0;
    double pi;
    if (argc != 2) {
        fprintf (stderr, "Passe o parâmetro de número de passos pela linha de comando\n");
        exit (EXIT_FAILURE);
    }
    num_steps = strtoul (argv[1], NULL, 0);
    step = 1.0/(double) num_steps;
#pragma omp parallel for reduction(+:sum) private(x)
    for (i = 0; i < num_steps; ++i) {
        x = (i + 0.5) * step;
        sum += 4.0/(1.0 + x*x);
    }
    pi = sum / num_steps;
    printf ("PI = %.20g\n", pi);
    return 0;
}
