#include "percolation.h"

#define GRID_Y_DIM 20
#define GRID_X_DIM 20

#define PROBABILITY_OF_1 0.6



int main(void) {
	if (sodium_init()==-1)
		exit(EXIT_FAILURE);

	Grid g;
	Grid *g_ptr = &g;
	PercolationType result;

	// result = single_exprmnt(g_ptr, GRID_Y_DIM, GRID_X_DIM, PROBABILITY_OF_1);

	do {
		result = single_exprmnt(g_ptr, GRID_Y_DIM, GRID_X_DIM, PROBABILITY_OF_1);

		if ( (g_ptr->num_of_clusters_with_perc>=2) ) {
			break;
		}
	} while (true);

	printf("%s\n", results_strings[result]);
	print_grid(g_ptr);

	delete_grid(g_ptr);

	return 0;
}
