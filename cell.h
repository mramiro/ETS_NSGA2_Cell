/*
 * cell.h
 *
 *  Created on: 27/02/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef CELL_H_
#define CELL_H_

#include "solution.h"
#include "nsga2.h"
#include "genetic.h"

typedef enum stagnation_t{
	fixed_stagn = 0,
	means_stagn,
	epsilon_stagn
} stagnation_t;

typedef enum communication_t{
	null_comm = 0,
	pr_intensive,
	pr_extensive
} communication_t;

typedef struct NSGA2Cell{
	int psize;
	int popsize;
	int generation;
	int max_gen;
	Sol *population;
	Bounds bounds;
	int is_stagnated;
	int firstfrontlen;
	double *prev_means;
	double **pfront;
	int pfrontlen;
	int stagn_ctr;
	stagnation_t stagn;
	mutation_t mut;
} NSGA2Cell;

NSGA2Cell *new_cell();

void init_cell(NSGA2Cell *cell, Sol *population, int popsize,
		stagnation_t stagn, mutation_t mut, int max_gen);

void process_cell(NSGA2Cell *cell);

int is_alive(NSGA2Cell *cell);

void print_cell_pop(NSGA2Cell * cell);

void comm_cells(NSGA2Cell* this_cell, NSGA2Cell *cell1, NSGA2Cell *cell2);

int comm_2cells(NSGA2Cell *cell_a, NSGA2Cell *cell_b);

#endif /* CELL_H_ */
