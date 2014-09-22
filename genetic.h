/*
 * genetic.h
 *
 *  Created on: 18/02/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef GENETIC_H_
#define GENETIC_H_

#include "solution.h"

typedef enum {
	uniform = 0,
	biased,
	biased_i
} mutation_t;

void tournament_selection(Sol *pool, int *selected_parents, int poolsize);

void point_crossover(Sol *parent_a, Sol *parent_b, Sol *son_a, Sol *son_b);

void uniform_mutation(Sol *s);

void biased_mutation(Sol *s, int inverse);

#endif /* GENETIC_H_ */
