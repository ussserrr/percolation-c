#include "percolation.h"

#ifdef MULTITHREADING
    #include <pthread.h>
#endif

// #define BENCHMARK
#ifdef BENCHMARK
    #include <time.h>
#endif


#define N_GRID_DIMS 3  // number of grid sizes

#define N_EXPER 1000  // number of experiments for each probability value

#define N_PROBAB 99  // exclude cases of 0.00 and 1.00 as trivial
#define D_PROBAB 0.01  // increment of the probability on each step



void *experiment_thread(void *args_ptr) {

    int grid_dim = *(int *)args_ptr;

    Grid g;
    Grid *g_ptr = &g;
    PercolationType result;

    // display progress
    printf("Grid dimension: %d\n", grid_dim);

    // For each grid size we create a file with a name consisting
    // of the size and the number of experiments
    char fname[20];
    snprintf(fname, 20, "%dx%d, N=%d", grid_dim,
             grid_dim, N_EXPER);
    FILE *fp = fopen(fname, "wt");

    // ...iterate through probabilities...
    double probab = 0.01;
    for (int probab_i=0; probab_i<N_PROBAB; probab_i++) {
        #ifndef MULTITHREADING
            fprintf(stdout, "\rprobability: %.2f", probab);
            fflush(stdout);
        #endif

        // ...and iterate through the number of experiments
        int num_of_exper_ended_with_percolation = 0;
        for (int exper_i=0; exper_i<N_EXPER; exper_i++) {
            result = single_exprmnt(g_ptr, grid_dim,
                                    grid_dim, probab);

            if (result != NO_PERCOLATION)
                num_of_exper_ended_with_percolation++;

            delete_grid(g_ptr);
        }

        // Write 2 columns: probability and the number of experiments
        // ended with percolation
        fprintf(fp, "%1.2f\t%1.2f\n", probab,
                (double)num_of_exper_ended_with_percolation/N_EXPER);

        probab += D_PROBAB;
    }

    fclose(fp);


    #ifdef MULTITHREADING
        printf("Done for grid dimension: %d\n", grid_dim);
        pthread_exit(NULL);
    #else
        printf("\n");
        return NULL;
    #endif
}



int main(void) {

    if (sodium_init() == -1)
        exit(EXIT_FAILURE);

    // set of grid sizes
    int grid_dim[N_GRID_DIMS] = {10, 15, 20};


    #ifdef BENCHMARK
        struct timespec begin, end;
        double elapsed;

        clock_gettime(CLOCK_MONOTONIC, &begin);
    #endif

    #ifdef MULTITHREADING
        pthread_t thread[N_GRID_DIMS];
        int thread_id;

        // iterate through grid sizes...
        for (int grid_size_i=0; grid_size_i<N_GRID_DIMS; grid_size_i++) {
            thread_id = pthread_create(&thread[grid_size_i],
                                       NULL,
                                       experiment_thread,
                                       (void *)&grid_dim[grid_size_i]);
            if (thread_id != 0) {
                fprintf(stderr, "error: pthread_create, rc: %d\n", thread_id);
                exit(EXIT_FAILURE);
            }
        }

        // block until all threads complete
        for (int grid_size_i=0; grid_size_i<N_GRID_DIMS; grid_size_i++) {
            pthread_join(thread[grid_size_i], NULL);
        }


    #else
        for (int grid_size_i=0; grid_size_i<N_GRID_DIMS; grid_size_i++) {
            experiment_thread((void *)&grid_dim[grid_size_i]);
        }
    #endif

    #ifdef BENCHMARK
        clock_gettime(CLOCK_MONOTONIC, &end);

        elapsed = end.tv_sec - begin.tv_sec;
        elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
        printf("Elapsed: %g s\n", elapsed);
    #endif


    return 0;
}
