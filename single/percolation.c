#include "percolation.h"



char *colored_cells[] = {
	ANSI_COLOR_RED "▉▉" ANSI_COLOR_RESET,\
	ANSI_COLOR_GREEN "▉▉" ANSI_COLOR_RESET,\
	ANSI_COLOR_CYAN "▉▉" ANSI_COLOR_RESET,\
	ANSI_COLOR_YELLOW "▉▉" ANSI_COLOR_RESET,\
	ANSI_COLOR_BLUE "▉▉" ANSI_COLOR_RESET,\
	ANSI_COLOR_MAGENTA "▉▉" ANSI_COLOR_RESET
};

char *results_strings[] = {
	"There is no percolation",\
	"Upwards percolation",\
	"Percolation from left to right",\
	"Both kinds of percolation"
};



void generate_grid(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1) {
	int y, x, i=0;

	g_ptr->y_dim = y_dim;
	g_ptr->x_dim = x_dim;
	g_ptr->probability_of_1 = probability_of_1;
	g_ptr->num_of_clusters_with_perc = 0;

	g_ptr->grid = calloc(y_dim+2, sizeof(int*));

	for (y=0; y<=y_dim+1; y++) {
		g_ptr->grid[y] = calloc(x_dim+2, sizeof(int));

		if ((y!=0) && (y!=y_dim+1))
			for (x=1; x<=x_dim; x++)
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



void delete_grid(Grid *g_ptr) {
	int y, x, i=0;

	for (y=0; y<=g_ptr->y_dim+1; y++) {
		free(g_ptr->grid[y]);

		if ((y!=0) && (y!=g_ptr->y_dim+1))
			for (x=1; x<=g_ptr->x_dim; x++)
				if (g_ptr->grid[y][x] == 1) {
					free(g_ptr->coords[i]);
					i++;
				}
	}

	free(g_ptr->grid);
	free(g_ptr->ids);
	free(g_ptr->coords);
	free(g_ptr->clusters_with_perc);
	free(g_ptr->lens_of_clusters_with_perc);
}



void print_grid(Grid *g_ptr) {
	int y, x, i=0, k;

	for (y=0; y<=g_ptr->y_dim+1; y++) {
		for (x=0; x<=g_ptr->x_dim+1; x++) {
			if (y==0 || y==g_ptr->y_dim+1) {
				if (x==0)
					printf(" ─");
				else if (x==g_ptr->x_dim+1)
					printf("─ ");
				else
					printf("──");
				continue;
			}
			if (x==0) {
				printf("│ ");
				continue;
			}
			if (x==g_ptr->x_dim+1) {
				printf(" │");
				continue;
			}
			if (g_ptr->grid[y][x]==1) {
				for (k=0; k<=g_ptr->num_of_clusters_with_perc; k++) {
					if (k == g_ptr->num_of_clusters_with_perc) {
						printf("▉▉");
						break;
					}
					if (g_ptr->ids[i]==g_ptr->clusters_with_perc[k]) {
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



void find_clusters(Grid *g_ptr) {
	int i, k, y, x, id_of_first_neighbor, id_of_second_neighbor, np_cnt, wrong_id, correct_id;

	do {
		np_cnt = 0;

		for (i=0; i<g_ptr->num_of_ones; i++) {
			y = g_ptr->coords[i][0];
			x = g_ptr->coords[i][1];

			if (g_ptr->grid[y-1][x]==1 && g_ptr->grid[y][x-1]==0) {
				for (k=0; k<g_ptr->num_of_ones; k++)
					if ((g_ptr->coords[k][0]==(y-1)) && (g_ptr->coords[k][1]==x))
						g_ptr->ids[i] = g_ptr->ids[k];
			}
			else if (g_ptr->grid[y][x-1]==1 && g_ptr->grid[y-1][x]==0) {
				for (k=0; k<g_ptr->num_of_ones; k++)
					if ((g_ptr->coords[k][0]==y) && (g_ptr->coords[k][1]==(x-1)))
						g_ptr->ids[i] = g_ptr->ids[k];
			}

			else if (g_ptr->grid[y-1][x]==1 && g_ptr->grid[y][x-1]==1) {
				for (k=0; k<g_ptr->num_of_ones; k++) {
					if ((g_ptr->coords[k][0]==(y-1)) && (g_ptr->coords[k][1]==x))
						id_of_first_neighbor = g_ptr->ids[k];
					if ((g_ptr->coords[k][0]==y) && (g_ptr->coords[k][1]==(x-1)))
						id_of_second_neighbor = g_ptr->ids[k];
				}
				g_ptr->ids[i] = MIN(id_of_first_neighbor, id_of_second_neighbor);

				if (id_of_first_neighbor!=id_of_second_neighbor) {
					np_cnt++;
					wrong_id = MAX(id_of_first_neighbor, id_of_second_neighbor);
					correct_id = MIN(id_of_first_neighbor, id_of_second_neighbor);
					for (k=0; k<g_ptr->num_of_ones; k++)
						if (g_ptr->ids[k]==wrong_id)
							g_ptr->ids[k] = correct_id;
				}
			}
		}

	} while (np_cnt);
}



PercolationType is_percolation(Grid *g_ptr) {
	bool y_eq_1,\
		 y_eq_GRID_Y_DIM,\
		 x_eq_1,\
		 x_eq_GRID_X_DIM;
	bool upwards = false,\
		 lefttoright = false;
	int i, j=0, k, y, x, len_of_cluster, len_of_uniq;

	int *uniq_clusters = malloc(sizeof(int));
	bool not_uniq;
	uniq_clusters[0] = g_ptr->ids[0];
	len_of_uniq = 1;
	for (i=1; i<g_ptr->num_of_ones; i++) {
		not_uniq = false;

		for (k=0; k<len_of_uniq; k++)
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

	g_ptr->clusters_with_perc = malloc(sizeof(int));
	g_ptr->lens_of_clusters_with_perc = malloc(sizeof(int));

	for (i=0; i<len_of_uniq; i++) {
		y_eq_1 = false;
		y_eq_GRID_Y_DIM = false;
		x_eq_1 = false;
		x_eq_GRID_X_DIM = false;

		len_of_cluster = 0;

		for (k=0; k<g_ptr->num_of_ones; k++) {
			if (g_ptr->ids[k]==uniq_clusters[i]) {
				y = g_ptr->coords[k][0];
				x = g_ptr->coords[k][1];

				if (y==1)
					y_eq_1 = true;
				if (y==g_ptr->y_dim)
					y_eq_GRID_Y_DIM = true;
				if (x==1)
					x_eq_1 = true;
				if (x==g_ptr->x_dim)
					x_eq_GRID_X_DIM = true;

				len_of_cluster++;
			}
		}

		if (y_eq_1 && y_eq_GRID_Y_DIM) {
			upwards = true;
			j++;
			g_ptr->clusters_with_perc = realloc(g_ptr->clusters_with_perc, j*sizeof(int));
			g_ptr->clusters_with_perc[j-1] = uniq_clusters[i];
			g_ptr->lens_of_clusters_with_perc = realloc(g_ptr->lens_of_clusters_with_perc, j*sizeof(int));
			g_ptr->lens_of_clusters_with_perc[j-1] = len_of_cluster;
		}
		if (x_eq_1 && x_eq_GRID_X_DIM) {
			lefttoright = true;
			if (!(y_eq_1 && y_eq_GRID_Y_DIM)) {
				j++;
				g_ptr->clusters_with_perc = realloc(g_ptr->clusters_with_perc, j*sizeof(int));
				g_ptr->clusters_with_perc[j-1] = uniq_clusters[i];
				g_ptr->lens_of_clusters_with_perc = realloc(g_ptr->lens_of_clusters_with_perc, j*sizeof(int));
				g_ptr->lens_of_clusters_with_perc[j-1] = len_of_cluster;
			}
		}
	}
	g_ptr->num_of_clusters_with_perc = j;

	if (upwards && !lefttoright)
		return UPWARDS;
	else if (lefttoright && !upwards)
		return LEFT_TO_RIGHT;
	else if (upwards && lefttoright)
		return BOTH;
	else
		return NO_PERCOLATION;
}



PercolationType single_exprmnt(Grid *g_ptr, int y_dim, int x_dim, double probability_of_1) {
	generate_grid(g_ptr, y_dim, x_dim, probability_of_1);
	find_clusters(g_ptr);

	return is_percolation(g_ptr);
}
