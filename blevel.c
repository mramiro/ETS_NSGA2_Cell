/*
 * blevel.c
 *
 *  Created on: 07/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include "defs.h"
#include "blevel.h"

/* Calcula recursivamente el b-level de una tarea dada */
static double calc_blevel(int task, double sum, double p_means[])
{
	int i;
	double cur_cost, max_cost;
	max_cost = sum;
	for (i = 0 ; i < n_edges; i++)
	{
		// Calcular para todos los arcos salientes de task
		if (dag_edges[i][0] == task)
		{
			// pEdges[i][1] -> nodo j, pEdges[i][2] -> peso del arco
			cur_cost = calc_blevel(dag_edges[i][1], sum + dag_edges[i][2] + p_means[(int)dag_edges[i][1]], p_means);
			// Sólo nos interesa el mayor costo acumulado (el camino más largo)
			if (cur_cost > max_cost)
			{
				max_cost = cur_cost;
			}
		}
	}
	return max_cost;
}

/* Calcula los b-levels para todas las tareas */
static double *calc_blevels()
{
	int i,j;
	double *blevel, means[n], mean_p;
	blevel=new_vector_double(n);
	// Calcular los costos de ejecución promedio para cada tarea
	for (i = 0; i < n; i++)
	{
		mean_p = 0;
		for (j =  0; j < m; j++)
		{
			mean_p += p_matrix[i][j];
		}
		means[i] = mean_p/m;
	}
	for(i = 0; i < n; i++)
	{
		// Cálculo recursivo
		blevel[i] = calc_blevel(i, means[i], means);
	}
	return blevel;
}

int* gen_order_blevel()
{
	int i, j, temp = 0;
	int *order;
	double *blevels;
	double max_blevel;

	order = new_vector_int(n);
	// Generar un orden inicial usando los b-levels
	blevels = calc_blevels();
	for (i = 0; i < n; i++)
	{
		max_blevel = 0;
		for (j = 0; j < n; j++)
		{
			if (max_blevel < blevels[j])
			{
				max_blevel = blevels[j];
				temp = j;
			}
		}
		order[i] = temp;
		blevels[temp] = 0;
	}
	free_vector(blevels);

	return (order);
}
