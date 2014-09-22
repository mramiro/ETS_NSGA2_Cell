/*
 * nsga2.c
 *
 *  Created on: 16/12/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <float.h>
#include <randoms.h>

#include "defs.h"
#include "io.h"
#include "quicksort.h"
#include "eval.h"
#include "blevel.h"
#include "solution.h"

#include "nsga2.h"

SolList *fast_nondominated_sort(Sol *solutions, int nsolutions)
{
	int i, j;
	int dominations[nsolutions][nsolutions]; // Matriz de dominancias: matriz[i][j]=1 => i domina a j
	int n_masters[nsolutions]; // Cantidad de soluciones que dominan a la solución i
	Sol *Ptr;
	SolList *fronts;
	int maxfronts = nsolutions >> 1;

	fronts = calloc(maxfronts, sizeof(SolList));
	for (i = 0; i < maxfronts; i++)
		init_list(&fronts[i]);

	for (i = 0; i < nsolutions; i++)
		for (j = 0; j < nsolutions; j++)
			dominations[i][j] = 0;

	for (i = 0; i < nsolutions; i++)
	{
		n_masters[i] = 0;
	}
	for (i = 0; i < nsolutions; i++) // BLOQUE 1: Revisión de dominancias
	{
		for (j = 0; j < nsolutions; j++)
		{
			if (j != i) // Las comparaciones son entre soluciones diferentes
			{
				if ((solutions[i].makespan <= solutions[j].makespan
						&& solutions[i].energy < solutions[j].energy)
						|| (solutions[i].makespan < solutions[j].makespan
								&& solutions[i].energy <= solutions[j].energy)) // i domina a j
				{
					dominations[i][j] = 1; //MARCA LA SOLUCION J COMO DOMINADA POR I
				}
				else //de otro modo aumenta el numero de soluciones que la domina
				{
					if ((solutions[j].makespan <= solutions[i].makespan
							&& solutions[j].energy < solutions[i].energy)
							|| (solutions[j].makespan < solutions[i].makespan
									&& solutions[j].energy <= solutions[i].energy)) // j domina a i
						n_masters[i]++;
				}
			}
		}

		if (n_masters[i] == 0) // Si nadie me domina, pertenezco al primer frente :D
		{
			list_add(&fronts[0], &solutions[i]);
			fronts[0].end->rank = 0;
			fronts[0].end->id = i;
			fronts[0].end->crowdist = 0;
		}
	}

	i = 0; // BLOQUE 2: Construcción de Frentes, desde F1 hasta donde alcance
	Ptr = fronts[i].start;

	while (Ptr != NULL && i < maxfronts - 1)
	{
//		while (Ptr != NULL && i + 1 < maxfronts - 1) // Para todas las soluciones del frente actual
		while (Ptr != NULL && i + 1 < maxfronts) // Para todas las soluciones del frente actual
		{
			for (j = 0; j < nsolutions; j++) //
			{
				if ((dominations[Ptr->id][j]) == 1) // por cada solución que i domina
				{
					//printf("soluciones que dominan a %d %d",j,domino[j]);
					n_masters[j] -= 1; // reduce el contador de las que dominan a j
					if (n_masters[j] == 0) // agrega al nuevo frente
					{
						list_add(&fronts[i + 1], &solutions[j]);
						fronts[i + 1].end->rank = i + 1;
						fronts[i + 1].end->id = j;
						fronts[i + 1].end->crowdist = 0; //poniendo distancia en 0
					}
				}
			}
			Ptr = Ptr->next;
		}

		i++; //Avanzar al siguiente frente
		Ptr = fronts[i].start;
	}
	return fronts;
}

Sol *crowding_distance_assignament(SolList front, Bounds *b)
{
	int i;
	Sol *sols;
	sols = new_vector_sol(front.len);
	Sol *Ptr;

	//Llenar el arreglo con las soluciones en el frente
	Ptr = front.start;
	for (i = 0; i < front.len; i++)
	{
		sols[i] = *Ptr;
		Ptr = Ptr->next;
	}

	// Cálculo de la crowding distance ; )
	sort_makespan(sols, 0, front.len); //QuickSort ordena arreglo !
	sols[0].crowdist += b->MAXMAKESPAN; //boundrys infinite !!
	sols[front.len - 1].crowdist += b->MAXMAKESPAN; // boundrys infinite !!
	for (i = 1; i < front.len - 1; i++)
	{
		if (b->MAXMAKESPAN != b->MINMAKESPAN)
			sols[i].crowdist += (sols[i + 1].makespan - sols[i - 1].makespan)
					/ (b->MAXMAKESPAN - b->MINMAKESPAN);
		else
			sols[i].crowdist += (sols[i + 1].makespan - sols[i - 1].makespan);
	}
	sort_energy(sols, 0, front.len); //QuickSort ordena soluciones!
	sols[0].crowdist += b->MAXENERGY; //boundrys infinite !!
	sols[front.len - 1].crowdist += b->MAXENERGY; //boundrys infinite !!
	for (i = 1; i < front.len - 1; i++)
	{
		if (sols[i].crowdist < DBL_MAX) //si es infinito ya no lo modifiques
		{
			if (b->MAXENERGY != b->MINENERGY)
				sols[i].crowdist += (sols[i + 1].energy - sols[i - 1].energy)
						/ (b->MAXENERGY - b->MINENERGY);
			else
				sols[i].crowdist += (sols[i + 1].energy - sols[i - 1].energy);
		}
	}
	return sols;
}

void update_bounds(Bounds *b, Sol *s)
{
	if (b->MAXMAKESPAN < s->makespan)
		b->MAXMAKESPAN = s->makespan;
	if (b->MAXENERGY < s->energy)
		b->MAXENERGY = s->energy;
	if (b->MINMAKESPAN > s->makespan)
		b->MINMAKESPAN = s->makespan;
	if (b->MINENERGY > s->energy)
		b->MINENERGY = s->energy;
}

void selection(Sol *parents_pool, int *selected_parents, int nsols)
{
	tournament_selection(parents_pool, selected_parents, nsols);
}

void crossover(Sol *parents_pool, Sol *sons, int *selected_parents, int nsols)
{
	int i;
	if (nsols%2 == 1)
		nsols--;
	for (i = 0; i < nsols; i = i + 2)
	{
		point_crossover(parents_pool+selected_parents[i],
				parents_pool+selected_parents[i+1], sons+i, sons+i+1);
	}
}

void mutation(Sol *sols, int nsols, mutation_t mt)
{
	int i;
	for (i = 0; i < nsols; i++)
	{
		switch (mt)
		{
		case uniform:
			uniform_mutation(sols+i);
			break;
		case biased:
			biased_mutation(sols+i, 0);
			break;
		case biased_i:
			biased_mutation(sols+i, 1);
			break;
		default:
			break;
		}
	}
}

void evaluation(Sol *sols, int nsols, Bounds *b)
{
	int i;
	for (i = 0; i < nsols; i++)
	{
		eval_sol(sols + i);
		update_bounds(b, sols + i);
	}
}

void print_pop_values(FILE *stream, Sol *sols, int nsols)
{
	int i;
	for (i = 0; i < nsols; i++)
	{
		eval_sol(sols + i);
		print_sol_values(stream, sols + i);
	}
	fprintf(stream, "\n");
}

int nsga2()
{
	int psize; // Tamaño del conjunto P (padres)
	int popsize; // Tamaño de la población total (padres + hijos)
	int *selected_parents;
	Sol *population;

	int i, j, newpopulation, generation, firstfrontlen;
	Sol *cur_front;
	Sol *population_temp;
	SolList *fronts;
	Bounds bounds;
	int *best_order;

	popsize = population_size;
	psize = popsize / 2;
	bounds.MAXMAKESPAN = bounds.MAXENERGY = 0;
	bounds.MINMAKESPAN = bounds.MINENERGY = DBL_MAX;
	population = new_vector_sol(popsize);
	population_temp = new_vector_sol(psize);
	selected_parents = new_vector_int(psize);

	best_order = gen_order_blevel(); // Se usa el orden generado por B-Levels en todas las soluciones

	generation = 0;
	init_random();
	for (i = 0; i < popsize; i++) //genera una poblacion aleatoria factible (padres e hijos random)!!!
	{
		population[i].ord = best_order;
		population[i].conf = gen_random_conf();
	}
	evaluation(population, popsize, &bounds);
	do
	{
		fronts = fast_nondominated_sort(population, popsize); //devuelve lista de frentes encontrados
		// Actualizar el indicador de donde termina el primer frente.
		firstfrontlen = fronts[0].len > psize ? psize : fronts[0].len;
		newpopulation = 0;
		for (i = 0;; i++)
		{
			if ((newpopulation + fronts[i].len) < psize)
			{
				cur_front = crowding_distance_assignament(fronts[i], &bounds); //devuelve arreglo de soluciones con distancia calculada
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
			cur_front = crowding_distance_assignament(fronts[i], &bounds);
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
		for (i = 0; i < psize; i++)
		{
			copy_sol(population+i, population_temp+i);
			population[i].id = 0;
		}

		for (i = 0; i < psize; i++) //numero maximo de frentes
		{
			if (fronts[i].len > 0)
				clear_list(&fronts[i]);
		}
		free(fronts);

		// Generar hijos
		selection(population, selected_parents, psize);
		crossover(population, population + psize, selected_parents, psize);
		mutation(population + psize, psize, uniform);
		evaluation(population, popsize, &bounds);
		generation++;
//		printf(" %d generacion\n", generation);
	}
	//while (continue_flag);
	while(generation < max_generations);

	// Imprimir resultados
	FILE *pout = stdout;
	if (output_file)
	{
		pout = open_file(output_file, "a", 0);
		if (!pout) pout = stdout;
	}
	print_pop_values(pout, population, psize);
	if (pout != stdout)	close_file(pout);

	// Imprimir datos para análisis
	printf("Tamaño del primer frente: ");
	printf("%d\n", firstfrontlen);

	// Liberar memoria
	for (i = 0; i < popsize; i++)
		free_matrix((void**)population[i].conf, 2);
	free(population);
	free(population_temp);
	free(selected_parents);
	free(best_order);

	return (firstfrontlen);
}
