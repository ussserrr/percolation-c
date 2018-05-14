#include "percolation.h"

// #define BENCHMARK
#ifdef BENCHMARK
	#include <time.h>
#endif


#define GRID_Y_DIM 20
#define GRID_X_DIM 20

#define PROBABILITY_OF_1 0.6



int main(void) {

	if (sodium_init() == -1)
		exit(EXIT_FAILURE);

	Grid g;
	Grid *g_ptr = &g;
	PercolationType result;


	#ifdef BENCHMARK
		clock_t tic = clock();
	#endif

	while (1) {
		result = single_exprmnt(g_ptr, GRID_Y_DIM, GRID_X_DIM, PROBABILITY_OF_1);

		if (g_ptr->num_of_clusters_with_perc > 1)
			break;
		else
			delete_grid(g_ptr);
	}

	#ifdef BENCHMARK
		clock_t toc = clock();
		printf("Elapsed: %f ms\n", (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0);
	#endif


	printf( "%dx%d, P(1)=%g: %s (%d)\n", GRID_X_DIM, GRID_Y_DIM, PROBABILITY_OF_1,
			results_strings[result], g_ptr->num_of_clusters_with_perc );
	print_grid(g_ptr);

	delete_grid(g_ptr);


	return 0;
}
