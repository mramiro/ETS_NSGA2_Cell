/*
 * nsga2.h
 *
 *  Created on: 16/12/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef NSGA2_H_
#define NSGA2_H_

#include <stdio.h>
#include "solution.h"
#include "genetic.h"

typedef struct Bounds{
	double MAXMAKESPAN, MAXENERGY, MINMAKESPAN, MINENERGY;
} Bounds;

SolList *fast_nondominated_sort(Sol *solutions, int nsolutions);

Sol *crowding_distance_assignament(SolList front, Bounds *b);

void update_bounds(Bounds *b, Sol *s);

void selection(Sol *parents_pool, int *selected_parents, int nsols);

void crossover(Sol *parents_pool, Sol *sons, int *selected_parents, int nsols);

void mutation(Sol *sols, int nsols, mutation_t mt);

void evaluation(Sol *sols, int nsols, Bounds *b);

void print_pop_values(FILE * stream, Sol *sols, int nsols);

int nsga2();

#endif /* NSGA2_H_ */
