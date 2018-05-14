#ifndef PERCOLATION_H_
#define PERCOLATION_H_



#include <stdio.h>
#include <stdbool.h>

// libsodium - generate random numbers
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


// Define this macro to enable measuring of lengths of percolated clusters. In
// some researches it can be useful, for example when we want to calculate
// relation between the percolated cluster' size and the probability of filling
// the cell
// #define LENS_OF_CLUSTERS_WITH_PERC

// The order of these entries corresponds to results_strings[] array so we can
// use it as an index for this array while printing the result
typedef enum {
    NO_PERCOLATION = 0,
    UPWARDS,
    LEFT_TO_RIGHT,
    BOTH
} PercolationType;

// Struct representing the grid. See percolation.c for more information about
// members of this structure and their roles in the algorithm
typedef struct {
    int x_dim;
    int y_dim;
    double probability_of_1;
    int **grid;

    int num_of_ones;
    #ifndef SERIES
        int num_of_clusters_with_perc;
    #endif

    int *ids;
    int **coords;

    #ifndef SERIES
        int *clusters_with_perc;
    #endif
    #ifdef LENS_OF_CLUSTERS_WITH_PERC
        int *lens_of_clusters_with_perc;
    #endif
} Grid;


extern char *colored_cells[];
extern char *results_strings[];


void generate_grid(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1);
void delete_grid(Grid *g_ptr);
#ifndef SERIES
    void print_grid(Grid *g_ptr);
#endif
void find_clusters(Grid *g_ptr);
PercolationType is_percolation(Grid *g_ptr);

PercolationType single_exprmnt(Grid *g_ptr, int y_dim, int x_dim,
                               double probability_of_1);



#endif /* PERCOLATION_H_ */
