#include "percolation.h"


#define N_GRID_DIMS 3  // number of grid sizes

#define N_EXPER 100  // number of experiments for each probability value

#define N_PROBAB 99  // exclude cases of 0.00 and 1.00 as trivial
#define D_PROBAB 0.01  // increment of the probability on each step



int main(void) {

    if (sodium_init() == -1)
        exit(EXIT_FAILURE);

    Grid g;
    Grid *g_ptr = &g;
    PercolationType result;

    // set of grid sizes
    int grid_dim[N_GRID_DIMS] = {10, 15, 20};

    // iterate through grid sizes...
    for (int grid_size_i=0; grid_size_i<N_GRID_DIMS; grid_size_i++) {
        // display progress
        printf("Grid dimension: %d\n", grid_dim[grid_size_i]);

        // For each grid size we create a file with a name consisting
        // of the size and the number of experiments
        char fname[20];
        snprintf(fname, 20, "%dx%d, N=%d", grid_dim[grid_size_i],
                 grid_dim[grid_size_i], N_EXPER);
        FILE *fp = fopen(fname, "wt");

        // ...iterate through probabilities...
        double probab = 0.01;
        for (int probab_i=0; probab_i<N_PROBAB; probab_i++) {
            fprintf(stdout, "\rprobability: %.2f", probab);
            fflush(stdout);

            // ...and iterate through the number of experiments
            int num_of_exper_ended_with_percolation = 0;
            for (int exper_i=0; exper_i<N_EXPER; exper_i++) {
                result = single_exprmnt(g_ptr, grid_dim[grid_size_i],
                                        grid_dim[grid_size_i], probab);

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

        printf("\n");
    }

    return 0;
}
