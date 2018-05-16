#include "percolation.h"



// Colored cells indicate percolated cluster (one color - one cluster)
char *colored_cells[] = {
    ANSI_COLOR_RED "▉▉" ANSI_COLOR_RESET,\
    ANSI_COLOR_GREEN "▉▉" ANSI_COLOR_RESET,\
    ANSI_COLOR_CYAN "▉▉" ANSI_COLOR_RESET,\
    ANSI_COLOR_YELLOW "▉▉" ANSI_COLOR_RESET,\
    ANSI_COLOR_BLUE "▉▉" ANSI_COLOR_RESET,\
    ANSI_COLOR_MAGENTA "▉▉" ANSI_COLOR_RESET
};

// The order of these strings corresponds to PercolationType enum so we can use
// it as an index for this array while printing the result
char *results_strings[] = {
    "There is no percolation",\
    "Upwards percolation",\
    "Percolation from left to right",\
    "Both kinds of percolation"
};



/*
 *  Initialize Grid structure: fill the grid at given size according to
 *  the given probability. You can reuse the same grid and its pointer for
 *  series of experiments but always call delete_grid() after each single
 *  experiment to prevent memory leaks
 */
void generate_grid(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1) {

    g_ptr->y_dim = y_dim;
    g_ptr->x_dim = x_dim;
    // We don't actually use this parameter anywhere after this function but
    // store it for possible future usage
    g_ptr->probability_of_1 = probability_of_1;

    #ifndef SERIES
        // used for print_grid() function
        g_ptr->num_of_clusters_with_perc = 0;
    #endif

    // 1-D dynamic array of labels (IDs) of each occupied cell. At the beginning,
    // all labels are different and are simply counted like 0,1,2,3,...
    g_ptr->ids = malloc(sizeof(int));
    // 2-D dynamic array that storing (y,x) coordinates of occupied cells
    g_ptr->coords = malloc(2*sizeof(int));
    // We operate with these two arrays in the parallel way so both ID and
    // coordinates are always match to each other, e.g.:
    //
    //   id   y x
    //    0  [2,0]
    //    1  [2,3]
    //    2  [4,5]
    //      ...
    //

    // 2-D array storing the grid itself. We add borders of zeros to handle
    // operations at edges (i.e. where (x-1)-like expressions could be met).
    // For example, randomly-filled grid 2x3 would be represented as
    //
    //   [ [0, 0, 0, 0, 0],
    //     [0, 1, 1, 0, 0],
    //     [0, 0, 1, 1, 0],
    //     [0, 0, 0, 0, 0] ]
    //
    g_ptr->grid = calloc(y_dim+2, sizeof(int*));

    #ifndef SERIES
        // 1-D array with labels (IDs) of clusters that have percolation. We
        // initialize it here (despite the fact we use it in is_percolation()
        // function) to safely freeing the memory (delete_grid() function) at any
        // time (not necessarly call find_clusters() and is_percolation() functions)
        g_ptr->clusters_with_perc = malloc(sizeof(int));
    #endif

    #ifdef LENS_OF_CLUSTERS_WITH_PERC
        g_ptr->lens_of_clusters_with_perc = malloc(sizeof(int));
    #endif


    int i = 0;
    for (int y=0; y<=y_dim+1; y++) {
        g_ptr->grid[y] = calloc(x_dim+2, sizeof(int));

        if ((y!=0) && (y!=y_dim+1))
            for (int x=1; x<=x_dim; x++)

                if ((double)randombytes_random()/0xFFFFFFFF < probability_of_1) {
                    g_ptr->grid[y][x] = 1;

                    g_ptr->ids = realloc(g_ptr->ids, (i+1)*sizeof(int));
                    g_ptr->ids[i] = i;

                    g_ptr->coords = realloc(g_ptr->coords, (i+1)*2*sizeof(int));
                    g_ptr->coords[i] = malloc(2*sizeof(int));
                    g_ptr->coords[i][0] = y;
                    g_ptr->coords[i][1] = x;

                    i++;
                }
    }

    g_ptr->num_of_ones = i;
}



/*
 *  Free all dynamically allocated memory (belonged to Grid structure; all local
 *  pointers are free'd at the functions were they've been declared). We free
 *  them in the same way as allocate, but in the reverse order.
 */
void delete_grid(Grid *g_ptr) {

    int i = 0;
    for (int y=0; y<=g_ptr->y_dim+1; y++) {
        free(g_ptr->grid[y]);

        if ((y!=0) && (y!=g_ptr->y_dim+1))
            for (int x=1; x<=g_ptr->x_dim; x++)

                if (g_ptr->grid[y][x] == 1) {
                    free(g_ptr->coords[i]);
                    i++;
                }
    }

    free(g_ptr->grid);
    free(g_ptr->ids);
    free(g_ptr->coords);
    #ifndef SERIES
        free(g_ptr->clusters_with_perc);
    #endif
    #ifdef LENS_OF_CLUSTERS_WITH_PERC
        free(g_ptr->lens_of_clusters_with_perc);
    #endif
}



#ifndef SERIES
/*
 *  Display grid in the terminal as pseudo-graphics. If find_clusters() and
 *  is_percolation() were called before, this function also colorizes the output
 *  to highlight clusters that has percolation (up to 6, but never met more
 *  than two at a time)
 */
void print_grid(Grid *g_ptr) {

    int i = 0;
    for (int y=0; y<=g_ptr->y_dim+1; y++) {
        for (int x=0; x<=g_ptr->x_dim+1; x++) {

            // horizontal borders
            if ((y==0) || (y==g_ptr->y_dim+1)) {
                if (x == 0)
                    printf(" ─");
                else if (x == g_ptr->x_dim+1)
                    printf("─ ");
                else
                    printf("──");
                continue;
            }

            // vertical borders
            if (x == 0) {
                printf("│ ");
                continue;
            }
            if (x == g_ptr->x_dim+1) {
                printf(" │");
                continue;
            }

            // put grid' cells
            if (g_ptr->grid[y][x] == 1) {
                // paint each cluster in its color (or simply put block if
                // there is no one)
                for (int k=0; k<=g_ptr->num_of_clusters_with_perc; k++) {
                    if (k == g_ptr->num_of_clusters_with_perc) {
                        printf("▉▉");
                        break;
                    }
                    if (g_ptr->ids[i] == g_ptr->clusters_with_perc[k]) {
                        printf("%s", colored_cells[k]);
                        break;
                    }
                }
                i++;
            }
            else
                printf("  ");
        }

        printf("\n");
    }
}
#endif



/*
 *  Routine that reassign cells' labels accordingly to their belonging to
 *  the same (or not) cluster
 */
void find_clusters(Grid *g_ptr) {

    int y, x,\
        id_first_neighbor=0, id_second_neighbor=0,\
        np_cnt,\
        wrong_id, correct_id;

    // reassign IDs until there are no more left so called wrong labels - 'np'
    do {
        np_cnt = 0;

        for (int i=0; i<g_ptr->num_of_ones; i++) {
            // extract coordinates of the filled cell to temp local variables
            y = g_ptr->coords[i][0];
            x = g_ptr->coords[i][1];

            // look only for two neighbors
            if (g_ptr->grid[y-1][x]==1 && g_ptr->grid[y][x-1]==0) {
                for (int k=0; k<g_ptr->num_of_ones; k++)
                    if ((g_ptr->coords[k][0]==(y-1)) && (g_ptr->coords[k][1]==x))
                        g_ptr->ids[i] = g_ptr->ids[k];
            }
            else if (g_ptr->grid[y][x-1]==1 && g_ptr->grid[y-1][x]==0) {
                for (int k=0; k<g_ptr->num_of_ones; k++)
                    if ((g_ptr->coords[k][0]==y) && (g_ptr->coords[k][1]==(x-1)))
                        g_ptr->ids[i] = g_ptr->ids[k];
            }

            // If both neighbors are occupied, choose the smaller label to be
            // assigned to the current cell (firstly we need to find neighbors'
            // IDs by known coordinates). Also, if IDs are unequal we again
            // reassign cells to the smaller one
            else if (g_ptr->grid[y-1][x]==1 && g_ptr->grid[y][x-1]==1) {
                for (int k=0; k<g_ptr->num_of_ones; k++) {
                    if ((g_ptr->coords[k][0]==(y-1)) && (g_ptr->coords[k][1]==x))
                        id_first_neighbor = g_ptr->ids[k];
                    if ((g_ptr->coords[k][0]==y) && (g_ptr->coords[k][1]==(x-1)))
                        id_second_neighbor = g_ptr->ids[k];
                }
                g_ptr->ids[i] = MIN(id_first_neighbor, id_second_neighbor);

                if (id_first_neighbor != id_second_neighbor) {
                    np_cnt++;
                    wrong_id = MAX(id_first_neighbor, id_second_neighbor);
                    correct_id = MIN(id_first_neighbor, id_second_neighbor);
                    for (int k=0; k<g_ptr->num_of_ones; k++)
                        if (g_ptr->ids[k] == wrong_id)
                            g_ptr->ids[k] = correct_id;
                }
            }
        }

    } while (np_cnt);
}



/*
 *  Define whether there is a percolation in the given grid and what its type.
 *  Also, the function forms the clusters_with_perc, lens_of_clusters_with_perc
 *  and num_of_clusters_with_perc variables if corresponding macro was set. All
 *  of the above is true only if find_clusters() function had been called earlier.
 */
PercolationType is_percolation(Grid *g_ptr) {
    bool y_eq_1,\
         y_eq_GRID_Y_DIM,\
         x_eq_1,\
         x_eq_GRID_X_DIM;
    bool upwards = false,\
         lefttoright = false;
    #ifndef SERIES
        int j=0;
    #endif
    int y, x,\
        len_of_cluster, len_of_uniq;

    // g_ptr->ids contains labels of all occupied cells so we need to find uniq
    // IDs (i.e. all remaining clusters), for example
    //
    //   g_ptr->ids = {0,0,0,0,1,2,2,0,0,3,3,3,5,5,2}  =>
    //   uniq_clusters = {0,1,2,3,5}
    //
    // We free that dynamically allocated array at the bottom of the function
    int *uniq_clusters = malloc(sizeof(int));
    bool not_uniq;
    uniq_clusters[0] = g_ptr->ids[0];
    len_of_uniq = 1;
    for (int i=1; i<g_ptr->num_of_ones; i++) {
        not_uniq = false;

        for (int k=0; k<len_of_uniq; k++)
            if (g_ptr->ids[i] == uniq_clusters[k]) {
                not_uniq = true;
                break;
            }

        if (!not_uniq) {
            uniq_clusters = realloc(uniq_clusters, (len_of_uniq+1)*sizeof(int));
            uniq_clusters[len_of_uniq] = g_ptr->ids[i];
            len_of_uniq++;
        }
    }

    // Continuously check all clusters whether they contain coordinates at the
    // borders of the grid
    for (int i=0; i<len_of_uniq; i++) {
        y_eq_1 = false;
        y_eq_GRID_Y_DIM = false;
        x_eq_1 = false;
        x_eq_GRID_X_DIM = false;

        len_of_cluster = 0;

        for (int k=0; k<g_ptr->num_of_ones; k++) {
            if (g_ptr->ids[k] == uniq_clusters[i]) {
                y = g_ptr->coords[k][0];
                x = g_ptr->coords[k][1];

                if (y == 1)
                    y_eq_1 = true;
                if (y == g_ptr->y_dim)
                    y_eq_GRID_Y_DIM = true;
                if (x == 1)
                    x_eq_1 = true;
                if (x == g_ptr->x_dim)
                    x_eq_GRID_X_DIM = true;

                len_of_cluster++;
            }
        }

        if (y_eq_1 && y_eq_GRID_Y_DIM) {
            upwards = true;
            #ifndef SERIES
                j++;
                g_ptr->clusters_with_perc = realloc(
                    g_ptr->clusters_with_perc, j*sizeof(int));
                g_ptr->clusters_with_perc[j-1] = uniq_clusters[i];
                #ifdef LENS_OF_CLUSTERS_WITH_PERC
                    g_ptr->lens_of_clusters_with_perc = realloc(
                        g_ptr->lens_of_clusters_with_perc, j*sizeof(int));
                    g_ptr->lens_of_clusters_with_perc[j-1] = len_of_cluster;
                #endif
            #endif
        }
        if (x_eq_1 && x_eq_GRID_X_DIM) {
            lefttoright = true;

            #ifndef SERIES
                if (!(y_eq_1 && y_eq_GRID_Y_DIM)) {
                    j++;
                    g_ptr->clusters_with_perc = realloc(
                        g_ptr->clusters_with_perc, j*sizeof(int));
                    g_ptr->clusters_with_perc[j-1] = uniq_clusters[i];
                    #ifdef LENS_OF_CLUSTERS_WITH_PERC
                        g_ptr->lens_of_clusters_with_perc = realloc(
                            g_ptr->lens_of_clusters_with_perc, j*sizeof(int));
                        g_ptr->lens_of_clusters_with_perc[j-1] = len_of_cluster;
                    #endif
                }
            #endif
        }
    }

    #ifndef SERIES
        g_ptr->num_of_clusters_with_perc = j;
    #endif

    free(uniq_clusters);

    if (upwards && !lefttoright)
        return UPWARDS;
    else if (lefttoright && !upwards)
        return LEFT_TO_RIGHT;
    else if (upwards && lefttoright)
        return BOTH;
    else
        return NO_PERCOLATION;
}



/*
 *  Single whole experiment that takes g_ptr, size and probability as
 *  arguments and returns PercolationType. Of course, you can use processed
 *  Grid structure after this function to retrieve any desired parameters.
 *  PLEASE NOTE, THAT THIS FUNCTION DOESN'T CALL delete_grid(), SO YOU NEED TO
 *  DO IT MANUALLY BY YOURSELF
 */
PercolationType single_exprmnt(Grid *g_ptr, int y_dim, int x_dim,
                               double probability_of_1) {
    generate_grid(g_ptr, y_dim, x_dim, probability_of_1);
    find_clusters(g_ptr);

    return is_percolation(g_ptr);
}
