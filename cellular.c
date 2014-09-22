/*
 * cellular.c
 *
 *  Created on: 27/02/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdio.h>
#include <randoms.h>

#include "defs.h"
#include "io.h"
#include "eval.h"
#include "blevel.h"
#include "cell.h"

#include "cellular.h"

static int cells_are_alive(NSGA2Cell *cells)
{
	int i;
	for (i = 0; i < ncells; i++)
	{
		if (is_alive(&cells[i]))
			return (1);
	}
	return (0);
}

int cell_nsga2()
{
	int i, cellsize, popsize;
	int *best_order;
	NSGA2Cell cells[ncells];
	Sol *population;
	stagnation_t st = (stagnation_t)use_stagnation;

	popsize = population_size;
	population = new_vector_sol(popsize);
	cellsize = popsize/ncells;

	best_order = gen_order_blevel(); // Se usa el órden generado por B-Levels en todas las soluciones

	init_random();
//	srand(100);

	for (i = 0; i < popsize; i++) //genera una poblacion aleatoria factible (padres e hijos random)!!!
	{
		population[i].ord = best_order;
		population[i].conf = gen_random_conf();
	}
	// Inicialización de celdas
	if (custom_cells)
	{
		if (ncells == 2)
		{
			init_cell(&cells[0], &population[0], cellsize,
							st, biased, max_generations);
			init_cell(&cells[1], &population[cellsize], cellsize,
					st, biased_i, max_generations);
		}
		else
		{
			mutation_t m;
			for (i = 0; i < ncells; i++)
			{
				m = (mutation_t)((i % 2) + 1);
				init_cell(&cells[i], &population[i*cellsize], cellsize,
						st, m, max_generations);
			}
		}
	}
	else
	{
		for (i = 0; i < ncells; i++)
		{
			init_cell(&cells[i], &population[i*cellsize], cellsize,
					st, uniform, max_generations);
		}
	}

	// Procesamiento de celdas
	do
	{
		for (i = 0; i < ncells; i++)
		{
			if (!cells[i].is_stagnated)
			{
				process_cell(&cells[i]);
			}
		}
	}
	while(cells_are_alive(&cells[0]));

	clock_t ini, end;
	ini = clock();
	// Comunicación entre celdas
	if (ncells > 1)
	{
		NSGA2Cell *cell_next, *cell_cur;
		for (i = 0; i < ncells; i++)
		{
			cell_cur = &cells[i];
			//cell_prev = i == 0 ? &cells[ncells-1] : &cells[i-1];
			cell_next = i == ncells-1 ? &cells[0] : &cells[i+1];
			comm_2cells(cell_cur, cell_next);
			//comm_cells(cell_cur, cell_prev, cell_next);
		}
	}
	end = clock();

	NSGA2Cell supercell;
	init_cell(&supercell, population, popsize, fixed_stagn, uniform, post_processing);
	do
	{
		process_cell(&supercell);
	}
	while(is_alive(&supercell));

	// ----------------------------------------------
	// Imprimir resultados
	FILE *pout = stdout;
	if (output_file)
	{
		pout = open_file(output_file, "a", 0);
		if (!pout) pout = stdout;
	}
	print_pop_values(pout, supercell.population, supercell.psize);

	if (pout != stdout)	close_file(pout);

	// Imprimir datos para análisis
	printf("Tamaño del frente de Pareto encontrado: ");
	printf("%d\n", supercell.firstfrontlen);
	for (i = 0; i < ncells; i++)
		printf("Última generación de celda %d: %d\n", i, cells[i].generation);
	printf("Última generación de supercelda: %d\n", supercell.generation);
	printf("Tiempo de comunicación: ");
	print_time(stdout, ini, end);

	return (supercell.firstfrontlen);
}
