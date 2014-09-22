/*
 * cell.c
 *
 *  Created on: 27/02/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <float.h>
#include <randoms.h>
#include <epsilon.h>

#include "defs.h"
#include "quicksort.h"
#include "eval.h"
#include "nsga2.h"
#include "pr.h"

#include "cell.h"

NSGA2Cell *new_cell()
{
	NSGA2Cell *ret = (NSGA2Cell*)malloc(sizeof(NSGA2Cell));
	if (!ret)
		terminate("Error allocating NSGA2Cell structure.");
	return (ret);
}

void init_cell(NSGA2Cell *cell, Sol *population, int popsize,
		stagnation_t stagn, mutation_t mut, int max_gen)
{
	cell->population = population;
	cell->popsize = popsize;
	cell->psize = popsize >> 1;
	cell->generation = 1;
	cell->firstfrontlen = 0;
	cell->is_stagnated = 0;
	cell->stagn_ctr = 0;
	cell->mut = mut;
	cell->stagn = stagn;
	cell->max_gen = max_gen;

	switch (cell->stagn)
	{
	case fixed_stagn:
		break;
	case means_stagn:
		cell->prev_means = new_vector_double(2);
		cell->prev_means[0] = DBL_MAX;
		cell->prev_means[1] = DBL_MAX;
		break;
	case epsilon_stagn:
		break;
	}

	cell->bounds.MAXMAKESPAN = cell->bounds.MAXENERGY = 0;
	cell->bounds.MINMAKESPAN = cell->bounds.MINENERGY = DBL_MAX;
	evaluation(cell->population, cell->popsize, &cell->bounds);
}

static void detect_stagnation(NSGA2Cell *cell)
{
	int i;
	double **cur_pfront, eps, means[2], sums[2];;
	switch (cell->stagn)
	{
	case fixed_stagn:
		if (cell->generation >= cell->max_gen)
			cell->is_stagnated = 1;
		return;
	case means_stagn:
		sums[0] = 0;
		sums[1] = 0;
		for (i = 0; i < cell->psize; i++)
		{
			sums[0] += cell->population[i].makespan;
			sums[1] += cell->population[i].energy;
		}
		means[0] = sums[0] / cell->psize;
		means[1] = sums[1] / cell->psize;

	//	if (!(means[0] < cell->prev_means[0] && means[1] < cell->prev_means[1]))
		if (cell->prev_means[0] < means[0] && cell->prev_means[1] < means[1])
			cell->is_stagnated = 1;
		cell->prev_means[0] = means[0];
		cell->prev_means[1] = means[1];
		return;
	case epsilon_stagn:
		cur_pfront = new_matrix_double(cell->firstfrontlen, 2);
		for (i = 0; i < cell->firstfrontlen; i++)
		{
			cur_pfront[i][0] = cell->population[i].makespan;
			cur_pfront[i][1] = cell->population[i].energy;
		}
		eps = 0;
		if (cell->generation > 1)
		{
			eps = additive_epsilon(cell->pfront, cell->pfrontlen, cur_pfront, cell->firstfrontlen, 2);
			free_matrix((void**)cell->pfront, cell->pfrontlen);
		}
		cell->pfront = cur_pfront;
		cell->pfrontlen = cell->firstfrontlen;

		if (eps < 0)
			cell->stagn_ctr = 0;
		else
			cell->stagn_ctr++;

		if (cell->stagn_ctr == stagn_max_gen)
			cell->is_stagnated = 1;
		return;
	}
}

void process_cell(NSGA2Cell *cell)
{
	int i, j, newpopulation, popsize, psize;
	Sol *cur_front, *population;
	SolList *fronts;
	Bounds *bounds;

	population = cell->population;
	popsize = cell->popsize;
	psize = cell->psize;
	bounds = &cell->bounds;

	Sol population_temp[psize];
	int selected_parents[psize];

	fronts = fast_nondominated_sort(population, popsize); //devuelve lista de frentes encontrados
	// Actualizar el indicador de donde termina el primer frente.
	cell->firstfrontlen = fronts[0].len > psize ? psize : fronts[0].len;
	newpopulation = 0;
	for (i = 0;; i++)
	{
//		if ((newpopulation + fronts[i].len) <= psize)
		if ((newpopulation + fronts[i].len) < psize)
		{
			cur_front = crowding_distance_assignament(fronts[i], bounds); //devuelve arreglo de soluciones con distancia calculada
			for (j = 0; j < fronts[i].len; j++)
			{
				swap_sol(&population_temp[newpopulation], &cur_front[j]);
				newpopulation++;
			}
			free(cur_front);
		}
		else
		{
			break;
		}
	}

	if (newpopulation < psize)
	{
		cur_front = crowding_distance_assignament(fronts[i], bounds);
		sort_distance(cur_front, 0, fronts[i].len); //sort
		for (j = fronts[i].len - 1; j >= 0; j--) //descending order
		{
			swap_sol(&population_temp[newpopulation], &cur_front[j]);
			newpopulation++;
			if (newpopulation == psize)
			{
				break;
			}
		}
		free(cur_front);
	}
	//aqui es mejor hacer una copia para poder reciclar la memoria de las listas
	for (i = 0; i < psize; i++) //Poblacion Nueva Pt+1
	{
		copy_sol(population+i, population_temp+i);
		population[i].id = 0;
	}
	for (i = 0; i < psize; i++) //numero maximo de frentes
	{
		if (fronts[i].len > 0)
			clear_list(fronts+i);
	}
	free(fronts);

	// Detección de estancamiento
	detect_stagnation(cell);
	if (cell->generation == cell->max_gen)
		cell->is_stagnated = 1;

	if (cell->is_stagnated)
		return;

	// Generar hijos
	selection(population, selected_parents, psize);
	crossover(population, population + psize, selected_parents, psize);
	mutation(population + psize, psize, cell->mut);
	evaluation(population, popsize, bounds);

	cell->generation++;
}

int is_alive(NSGA2Cell *cell)
{
	return (!cell->is_stagnated);
}

void print_cell_pop(NSGA2Cell * cell)
{
	int i;
	for (i = 0; i < cell->psize; i++)
	{
		eval_sol(&cell->population[i]);
		print_sol_values(stdout, &cell->population[i]);
	}
//	printf("First Front Length: %d\n", cell->firstfrontlen);
}

void comm_cells(NSGA2Cell* this_cell, NSGA2Cell *cell1, NSGA2Cell *cell2)
{
	int i;
	Sol *sol_a, *sol_b;

	for (i = 0; i < this_cell->psize / 2; i++)
	{
		// Seleccionar una solución de los frentes de Pareto de cada celda.
		sol_a = &this_cell->population[random_int(0, this_cell->firstfrontlen)];
		do
			sol_b = &cell1->population[random_int(0, cell1->firstfrontlen)];
		while (hamming_distance(sol_a, sol_b) < n*pr_dist_ratio);
		// Path Relinking de sol_a -> sol_b
//		path_relinking(&this_cell->population[this_cell->psize+i], sol_a, sol_b, &this_cell->bounds);

		// Ahora hacia la otra celda
		sol_a = &this_cell->population[random_int(0, this_cell->firstfrontlen)];
		do
			sol_b = &cell2->population[random_int(0, cell2->firstfrontlen)];
		while (hamming_distance(sol_a, sol_b) < n*pr_dist_ratio);
//		path_relinking(&this_cell->population[this_cell->psize+(this_cell->psize>>1)+i], sol_a, sol_b, &this_cell->bounds);
	}
}

int comm_2cells(NSGA2Cell *cell_a, NSGA2Cell *cell_b)
{
	int remaining_slots, used_slots;
	Sol *sol_a, *sol_b, *start_pt;

	start_pt = cell_a->population + cell_a->psize;
	remaining_slots = cell_a->psize;
	used_slots = 0;
	do
	{
		sol_a = cell_a->population + random_int(0, cell_a->firstfrontlen);
		do
			sol_b = cell_b->population + random_int(0, cell_b->firstfrontlen);
		while(hamming_distance(sol_a, sol_b) < n*pr_dist_ratio);

		used_slots = path_relinking_e(start_pt, sol_a, sol_b, &cell_a->bounds, remaining_slots);
		start_pt += used_slots;
		remaining_slots -= used_slots;

		if (bilateral_comm && remaining_slots > 0)
		{
			used_slots = path_relinking_e(start_pt, sol_b, sol_a, &cell_a->bounds, remaining_slots);
			start_pt += used_slots;
			remaining_slots -= used_slots;
		}

	}
	while (remaining_slots > 0);//  && ictr < cell_a->popsize);
	return (cell_a->psize - remaining_slots);
}
