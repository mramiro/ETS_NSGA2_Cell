/*
 * pr.c
 *
 *  Created on: 11/11/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <randoms.h>
#include "defs.h"
#include "pr.h"

int hamming_distance(Sol *a, Sol *b)
{
	size_t i;
	int dist = 0;
	for (i = 0; i < n; i++)
	{
		if (a->conf[machine][i] != b->conf[machine][i])
			dist++;
		else if (a->conf[vlevel][i] != b->conf[vlevel][i])
			dist++;
	}
	return dist;
}

int path_relinking_e(Sol *s_out, Sol *s_init, Sol *s_guide, Bounds *bounds, int max_sols)
{
	int i, task, changes, maxchanges, sctr;
	int tm, tv, ord[n];
	Sol *s_temp;

	random_perm((int*)ord, 0, n);

	s_temp = clone_sol(s_init);
	maxchanges = n*pr_dist_ratio;
	changes = 0;
	sctr = 0;

	i = n-1;
	while (changes < maxchanges && i >= 0)
	{
//		task = s_temp->ord[i];
		task = ord[i];

		tm = s_temp->conf[machine][task];
		tv = s_temp->conf[vlevel][task];
		// Verificar que el elemento realmente difiera
		if (tm != s_guide->conf[machine][task] || tv != s_guide->conf[vlevel][task])
		{
			s_temp->conf[machine][task] = s_guide->conf[machine][task];
			s_temp->conf[vlevel][task] = s_guide->conf[vlevel][task];

			eval_sol(s_temp);
			if (s_temp->makespan < bounds->MINMAKESPAN || s_temp->energy < bounds->MINENERGY)
			{ // Se ha encontrado una nueva solución en el frente
				update_bounds(bounds, s_temp);
				//HAY QUE GUARDARLA!!
				copy_sol(s_out, s_temp);
				s_out++;
				sctr++;
//				changes++;
				if (sctr == max_sols)
				{
					free_sol(s_temp);
					return(sctr);
				}
			}
			changes++;
		}
		i--;
	}
	copy_sol(s_out, s_temp);
	sctr++;
	free_sol(s_temp);
	return (sctr);
}

void path_relinking_i(Sol *s_out, Sol *s_init, Sol *s_guide, Bounds *bounds)
{
	int i, task, changes, maxchanges;
	int tm, tv;
	Sol *s_temp;

	s_temp = clone_sol(s_init);
	maxchanges = n*pr_dist_ratio;
	changes = 0;
	i = n-1;

	while (changes < maxchanges && i > 0)
	{
		task = s_temp->ord[i];

		tm = s_temp->conf[machine][task];
		tv = s_temp->conf[vlevel][task];
		// Verificar que el elemento realmente difiera
		if (tm != s_guide->conf[machine][task] || tv != s_guide->conf[vlevel][task])
		{
			s_temp->conf[machine][task] = s_guide->conf[machine][task];
			s_temp->conf[vlevel][task] = s_guide->conf[vlevel][task];

			eval_sol(s_temp);
			if (s_temp->makespan < bounds->MINMAKESPAN || s_temp->energy < bounds->MINENERGY)
			{ // Se ha encontrado una nueva solución en el frente
				update_bounds(bounds, s_temp);
				changes++;
			}
			else
			{ // Regresar la solución a como estaba
				s_temp->conf[machine][task] = tm;
				s_temp->conf[vlevel][task] = tv;
			}
		}
		i--;
	}
	copy_sol(s_out, s_temp);
	eval_sol(s_out);
	free_sol(s_temp);
}


int new_pr(Sol *s_out, Sol *s_init, Sol *s_guide)
{
	int dist, i;
	Sol *new_sols, *s_cur;

	dist = hamming_distance(s_init, s_guide);
	new_sols = new_vector_sol(dist);
	if (dist > 0)
	{
		s_cur = new_sols;
		s_cur = clone_sol(s_init);
		for (i = 0; i < n; i++)
		{
			if (s_cur->conf[machine][i] == s_guide->conf[machine][i])
			{
				if (s_cur->conf[vlevel][i] != s_guide->conf[vlevel][i])
				{
					s_cur->conf[vlevel][i] = s_guide->conf[vlevel][i];
					s_cur++;
					s_cur = clone_sol(s_cur-1);
				}
			}
			else
			{
				s_cur->conf[machine][i] = s_guide->conf[machine][i];
				s_cur->conf[vlevel][i] = s_guide->conf[vlevel][i];
				s_cur++;
				s_cur = clone_sol(s_cur-1);
			}
		}
	}
	s_out = new_sols;
	return(dist);
}
