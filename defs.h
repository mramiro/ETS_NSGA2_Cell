/*
 * defs.h
 *
 *  Created on: 07/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef DEFS_H_
#define DEFS_H_

#include <memalloc.h>
#include <arrutils.h>

enum EnumDAG{
	t_i = 0,
	t_j,
	c_ij
};

enum EnumConfArray{
	machine = 0,
	vlevel
};

#define DEFAULT_INSTANCE "sample_3_8.txt"
#define DEFAULT_VSPAIRS "speed_pairs.txt"

int n, m, n_edges, n_vlevels;

double **p_matrix;
double **dag_edges;
double **vspairs;

extern void terminate(const char *error);

// Configuration
const static int population_size = 200;
const static int ncells = 2;
const static int max_generations = 300;
const static int use_stagnation = 1;
const static int stagn_max_gen = 30;
const static int custom_cells = 0;
const static int bilateral_comm = 1;
const static int post_processing = 1;
const static double pr_dist_ratio = 0.10;

#endif /* DEFS_H_ */
