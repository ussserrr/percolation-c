#ifndef PERCOLATION_H_
#define PERCOLATION_H_



#include <stdio.h>
#include <stdbool.h>
#include <sodium.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define NUM_OF_COLORS      6

#define MIN(x,y)           ((x)<(y)?(x):(y))
#define MAX(x,y)           ((x)>(y)?(x):(y))


typedef enum {
	NO_PERCOLATION = 0,
	UPWARDS,
	LEFT_TO_RIGHT,
	BOTH
} PercolationType;

typedef struct {
	int x_dim;
	int y_dim;
	double probability_of_1;
	int **grid;

	int num_of_ones;
	int num_of_clusters_with_perc;

	int *ids;  // ids[num_of_ones];
	int **coords;  // coords[num_of_ones][2];

	int *clusters_with_perc;
	int *lens_of_clusters_with_perc;
	// PercolationType percolation;
} Grid;


extern char *colored_cells[];
extern char *results_strings[];


extern void generate_grid(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1);
extern void delete_grid(Grid *g_ptr);
extern void print_grid(Grid *g_ptr);
extern void find_clusters(Grid *g_ptr);
extern PercolationType is_percolation(Grid *g_ptr);
extern PercolationType single_exprmnt(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1);



#endif /* PERCOLATION_H_ */
