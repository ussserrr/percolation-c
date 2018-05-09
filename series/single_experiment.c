#include "percolation.h"

#define GRID_Y_DIM 10
#define GRID_X_DIM 10

#define PROBABILITY_OF_1 0.6



int main(void) {
	if (sodium_init()==-1)
		exit(EXIT_FAILURE);

	int y, x, i=0, k;
	int y_dim=GRID_Y_DIM, x_dim=GRID_X_DIM;
	double probab = PROBABILITY_OF_1;

	int grid[y_dim+2][x_dim+2];
	for (y=0; y<y_dim+2; y++)
		for (x=0; x<x_dim+2; x++)
			grid[y][x] = 0;

	int num_of_ones = 0;
	for (y=1; y<y_dim+1; y++)
		for (x=1; x<x_dim+1; x++)
			if ((double)randombytes_random()/0xFFFFFFFF <= probab) {
				grid[y][x] = 1;
				num_of_ones++;
			}

	int ids[num_of_ones];
	int coords[num_of_ones][2];
	for (y=1; y<y_dim+1; y++)
		for (x=1; x<x_dim+1; x++)
			if (grid[y][x] == 1) {
				ids[i] = i;
				coords[i][0] = y;
				coords[i][1] = x;
				i++;
			}

	for (y=0; y<y_dim+2; y++) {
		for (x=0; x<x_dim+2; x++) {
			if (grid[y][x] == 1)
				printf("▉▉");
			else
				printf("  ");
		}
		printf("\n");
	}

	int id_of_first_neighbor, id_of_second_neighbor, np_cnt, wrong_id, correct_id;
	do {
		np_cnt = 0;

		for (i=0; i<num_of_ones; i++) {
			y = coords[i][0];
			x = coords[i][1];

			if (grid[y-1][x]==1 && grid[y][x-1]==0) {
				for (k=0; k<num_of_ones; k++)
					if ((coords[k][0]==(y-1)) && (coords[k][1]==x))
						ids[i] = ids[k];
			}
			else if (grid[y][x-1]==1 && grid[y-1][x]==0) {
				for (k=0; k<num_of_ones; k++)
					if ((coords[k][0]==y) && (coords[k][1]==(x-1)))
						ids[i] = ids[k];
			}

			else if (grid[y-1][x]==1 && grid[y][x-1]==1) {
				for (k=0; k<num_of_ones; k++) {
					if ((coords[k][0]==(y-1)) && (coords[k][1]==x))
						id_of_first_neighbor = ids[k];
					if ((coords[k][0]==y) && (coords[k][1]==(x-1)))
						id_of_second_neighbor = ids[k];
				}
				ids[i] = (id_of_first_neighbor<id_of_second_neighbor) ? id_of_first_neighbor : id_of_second_neighbor;

				if (id_of_first_neighbor!=id_of_second_neighbor) {
					np_cnt++;
					wrong_id = (id_of_first_neighbor>id_of_second_neighbor) ? id_of_first_neighbor : id_of_second_neighbor;
					correct_id = (id_of_first_neighbor<id_of_second_neighbor) ? id_of_first_neighbor : id_of_second_neighbor;
					for (k=0; k<num_of_ones; k++)
						if (ids[k]==wrong_id)
							ids[k] = correct_id;
				}
			}
		}

	} while (np_cnt);

	bool y_eq_1,\
		 y_eq_GRID_Y_DIM,\
		 x_eq_1,\
		 x_eq_GRID_X_DIM;
	bool upwards = false,\
		 lefttoright = false;
	int len_of_clusters_with_perc=0, len_of_cluster;

	int *uniq_clusters = malloc(sizeof(int));
	bool not_uniq;
	uniq_clusters[0] = ids[0];
	int len_of_uniq = 1;
	for (i=1; i<num_of_ones; i++) {
		not_uniq = false;

		for (k=0; k<len_of_uniq; k++)
			if (ids[i] == uniq_clusters[k]) {
				not_uniq = true;
				break;
			}

		if (!not_uniq) {
			uniq_clusters = realloc(uniq_clusters, (len_of_uniq+1)*sizeof(int));
			uniq_clusters[len_of_uniq] = ids[i];
			len_of_uniq++;
		}
	}

	for (i=0; i<len_of_uniq; i++) {
		y_eq_1 = false;
		y_eq_GRID_Y_DIM = false;
		x_eq_1 = false;
		x_eq_GRID_X_DIM = false;

		len_of_cluster = 0;

		for (k=0; k<num_of_ones; k++) {
			if (ids[k]==uniq_clusters[i]) {
				y = coords[k][0];
				x = coords[k][1];

				if (y==1)
					y_eq_1 = true;
				if (y==y_dim)
					y_eq_GRID_Y_DIM = true;
				if (x==1)
					x_eq_1 = true;
				if (x==x_dim)
					x_eq_GRID_X_DIM = true;

				len_of_cluster++;
			}
		}

		if (y_eq_1 && y_eq_GRID_Y_DIM) {
			upwards = true;
			len_of_clusters_with_perc += len_of_cluster;
		}
		if (x_eq_1 && x_eq_GRID_X_DIM) {
			lefttoright = true;
			if (!(y_eq_1 && y_eq_GRID_Y_DIM))
				len_of_clusters_with_perc += len_of_cluster;
		}
	}

	if (upwards && !lefttoright)
		printf("%s, %d\n", results_strings[1], len_of_clusters_with_perc);
	else if (lefttoright && !upwards)
		printf("%s, %d\n", results_strings[2], len_of_clusters_with_perc);
	else if (upwards && lefttoright)
		printf("%s, %d\n", results_strings[3], len_of_clusters_with_perc);
	else
		printf("%s, %d\n", results_strings[0], len_of_clusters_with_perc);


	return 0;
}
