#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int num_procs;
long num_steps;
double step;
long interval_size;

int GetNumProcessors() {
    int p = (int)sysconf(_SC_NPROCESSORS_ONLN);
    if (p < 2)  p = 2;
    return p;
}

double SomaParcial(int start, int end) {
    int i;
    double x, sum = 0.0;
    for(i=start; i < end; i++){
        x = (i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    return sum;
}

void* execute(void* arg) {
    int i, start, end;
    double* sum = (double*)malloc(sizeof(double));
    
    i = (int)arg

    start = ;
    end = ;
    *sum = SomaParcial(start, end);
    
    return sum;
}

int main(int argc, char* argv[]) {
    int i; 
    double pi, sum = 0.0;

    if (argc < 2) {
        printf("ERRO: o numero de passos precisa ser passado.\n");
        return 0;
    }
    num_steps = atol(argv[1]);
    step = 1.0/(double)num_steps;
    num_procs = GetNumProcessors();
    interval_size = 0; /*WAT*/

    pthread_t* tids = (pthread_t*)malloc(sizeof(pthread_t) * (num_procs-1));
    /* criar N-1 thread, start nelas, rodar 1 SomaParcial aqui, terminar dar merge somar tudo e bam*/
    for (i=0; i<num_procs-1; i++) {
        if ( pthread_create( &tids[i], NULL, execute, (void*)i) ) {
            printf("error creating thread.\n");
            return 0;
        }
    }

    pi = step*sum;
    printf("PI = %.20g\n", pi);
    return 0;
}
