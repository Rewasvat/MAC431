#include <stdio.h>

void main() {
    long num_steps = 1000000;
    int i; double x, pi, step, sum = 0.0;
    step=1.0/(double)num_steps;
    for(i=0;i<num_steps;i++){
        x = (i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    pi = step*sum;
    printf("PI = %g\n", pi);
}