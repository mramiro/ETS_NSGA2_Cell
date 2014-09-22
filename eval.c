/*
 * eval.c
 *
 *  Created on: 17/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <randoms.h>

#include "defs.h"
#include "eval.h"

static int **tasks_precedence = 0;

//static double calc_cprime_value(Sol* s, int task_i, int task_j)
//{
//	// Si las tareas i y j se ejecutan en diferente máquina, entonces C' = C
//	if (s->conf[machine][task_i] != s->conf[machine][task_j])
//	{
//		int i;
//		for (i = 0; i < n_edges; i++)
//		{
//			if (dag_edges[i][t_i] == task_i && dag_edges[i][t_j] == task_j)
//			{
//				return (dag_edges[i][c_ij]);
//			}
//		}
//	}
//	// De otra manera, C' = 0
//	return 0;
//}

static int **build_precedence_matrix()
{
	if (!tasks_precedence)
	{ // Construir la estructura de precedencias
		tasks_precedence = (int**)calloc(n, sizeof(int*));
		int i, j, counter;
		int tasks_buffer[n-1];
		for (i = 0; i < n; i++)
		{
			counter = 0;
			for (j = 0; j < n_edges; j++)
			{
				if (dag_edges[j][t_j] == i)
				{
					tasks_buffer[counter] = (int)dag_edges[j][t_i];
					counter++;
				}
			}
			tasks_precedence[i] = new_vector_int(counter+1);
			for (j = 0; j < counter; j++)
			{
				tasks_precedence[i][j] = tasks_buffer[j];
			}
			tasks_precedence[i][counter] = -1;

		}
	}
	return (tasks_precedence);
}

//static double get_pprime(Sol* s, int task)
//{
//	// El costo relativo de ejecutar task en cur_machine, corriendo a un nivel cur_vlevel
//	int cur_machine = s->conf[machine][task];
//	int cur_vlevel = s->conf[vlevel][task];
//	return p_matrix[task][cur_machine]/vspairs[cur_vlevel][(cur_machine<<1)+1];
//}
//
//void calc_pprime_vector(Sol* s, double pprime[])
//{
//	int i;
//	for (i = 0; i < n; i++)
//	{
//		pprime[i] = get_pprime(s, i);
//	}
//}

double calc_makespan(int *order, int **conf, int len)
{
	int i, j, k, cur_task, cur_machine;
	double cur_cost, max_cost, cprime;
	double cur_exec_time[m];
	double start_times[n];
	double end_times[n];
	int *prec_tasks;
	double pprime = 0;
	double makespan = 0;
	// Inicializar arreglos
	set_vector_double(&cur_exec_time[0], m, 0);
	if (!tasks_precedence)
		build_precedence_matrix();
	// Recorrer tareas del ordenamiento
	for (i = 0; i < len; i++)
	{
		cur_task = order[i];
		cur_machine = conf[machine][cur_task];
		// Obtener tareas precedentes
		prec_tasks = tasks_precedence[cur_task];
		// Obtener el mayor costo acumulado de las tareas precedentes
		max_cost = 0;
		for (j = 0; prec_tasks[j] != -1; j++)
		{
			// Si la tarea no tiene precedentes (hay un -1 con j=0) nunca se entra aquí.
			cprime = 0;
			if (cur_machine != conf[machine][prec_tasks[j]])
			{
				for (k = 0; k < n_edges; k++)
				{
					if (dag_edges[k][t_i] == prec_tasks[j] && dag_edges[k][t_j] == cur_task)
						cprime = dag_edges[k][c_ij];
				}
			}
			cur_cost = end_times[prec_tasks[j]] + cprime;
			if (cur_cost > max_cost)
			{
				max_cost = cur_cost;
			}
		}
		// Verificar quien aporta un mayor costo: el tiempo actual de ejecución
		// de la máquina, o el mayor costo de las tareas precedentes
		if (max_cost > cur_exec_time[cur_machine])
		{
			start_times[cur_task] = max_cost;
		}
		else
		{
			// La tarea no tiene precedentes, o ninguno de los tiempos de sus precedentes
			// son mayores al tiempo actual en la máquina.
			start_times[cur_task] = cur_exec_time[cur_machine];
		}
		pprime = p_matrix[cur_task][cur_machine]/vspairs[conf[vlevel][cur_task]][(cur_machine<<1)+1]; // Wow!
		// Guardar el fin de la tarea
		end_times[cur_task] = start_times[cur_task] + pprime; //get_pprime(s, cur_task);//pprime[cur_task];
		// Actualizar el tiempo de ejecución de la máquina
		cur_exec_time[cur_machine] = end_times[cur_task];

		// El makespan es el mayor tiempo de terminación registrado
		if (end_times[cur_task] > makespan)
			makespan = end_times[cur_task];
	}
	return makespan;
}

double calc_makespan_heft(int *order, int *assignation, int len)
{
	int i, j, k, cur_task, cur_machine;
	double cur_cost, max_cost, cprime;
	double cur_exec_time[m];
	double start_times[n];
	double end_times[n];
	int *prec_tasks;

	cur_task = 0;
	set_vector_double(&cur_exec_time[0], m, 0);
	if (!tasks_precedence)
		build_precedence_matrix();
	for (i = 0; i < len; i++)
	{
		cur_task = order[i];
		cur_machine = assignation[cur_task];
		prec_tasks = tasks_precedence[cur_task];
		max_cost = 0;
		for (j = 0; prec_tasks[j] != -1; j++)
		{
			cprime = 0;
			if (cur_machine != assignation[prec_tasks[j]])
			{
				for (k = 0; k < n_edges; k++)
				{
					if (dag_edges[k][t_i] == prec_tasks[j] && dag_edges[k][t_j] == cur_task)
						cprime = dag_edges[k][c_ij];
				}
			}
			cur_cost = end_times[prec_tasks[j]] + cprime;
			if (cur_cost > max_cost)
			{
				max_cost = cur_cost;
			}
		}
		if (max_cost > cur_exec_time[cur_machine])
		{
			start_times[cur_task] = max_cost;
		}
		else
		{
			start_times[cur_task] = cur_exec_time[cur_machine];
		}
		end_times[cur_task] = start_times[cur_task] + p_matrix[cur_task][cur_machine];
		cur_exec_time[cur_machine] = end_times[cur_task];
	}
	return end_times[cur_task];
}


double calc_energy(int **conf, double makespan)
{
	int i, j;
	double idle_time, energy, v, rel_time;
	int cur_vlevel, cur_machine;
	if (makespan <= 0)
		return -1;
	// Calcular la energía consumida por la ejecución de tareas
	energy = 0;
	v = 0;
	for (i = 0; i < n; i++)
	{
		cur_machine = conf[machine][i];
		cur_vlevel = conf[vlevel][i];
		v = vspairs[cur_vlevel][cur_machine<<1];
		rel_time = p_matrix[i][cur_machine]/ vspairs[cur_vlevel][(cur_machine<<1)+1];
		energy += rel_time * v * v;
	}
	// Recorrer las máquinas para calcular el tiempo muerto (idle)
	for (i = 0; i < m; i++)
	{
		cur_machine = i;
		// Encontrar el menor voltaje disponible en m_i
//		for (j = 0; j < n_vlevels; j++)
//		{
//			if (vspairs[j][cur_machine<<1] == 0)
//				break;
//			v = vspairs[j][cur_machine<<1];
//		}
		for (j = 0; j < n_vlevels && vspairs[j][cur_machine<<1] != 0; j++); j--;
		v = vspairs[j][cur_machine<<1];

		idle_time = makespan;
		// El idle de una máquina es el tiempo en que no se ejecuta tarea alguna en esta
		for (j = 0; j < n; j++)
		{
			// Encontrar cada tarea ejecutada en m_i y restar su costo.
			if (cur_machine == conf[machine][j])
			{
				idle_time -= p_matrix[j][cur_machine]/vspairs[conf[vlevel][j]][(cur_machine<<1)+1];
			}
		}
		energy += idle_time * v*v;
	}
	return energy;
}

int is_feasible(int *order, int len)
{
	int exec_tasks[n], *prec_tasks;
	static int i, j;
	set_vector_int(&exec_tasks[0], n, 0);
	if (!tasks_precedence)
		build_precedence_matrix();
	for (i = 0; i < len; i++)
	{
		prec_tasks = tasks_precedence[order[i]]; //TODO Revisar si conviene checar que las tareas no se repitan
		for (j = 0; prec_tasks[j] != -1; j++)
		{
			if (exec_tasks[prec_tasks[j]] == 0)
				return 0;
		}
		exec_tasks[order[i]] = 1;
	}
	return 1;
}

int **gen_random_conf()
{
	int **ret, i, j, mach;
	ret = new_matrix_int(2,n);
	for(i=0;i<n;i++)
	{
        mach = random_int(0,m);
		ret[machine][i]=mach;

		// Encontrar mínimo nivel de voltaje en la máquina
		for (j = 0; j < n_vlevels && vspairs[j][mach<<1] != 0; j++);

		ret[vlevel][i]=random_int(0,j);
	}
	return ret;
}

void free_precedence_matrix()
{
	if (tasks_precedence != 0)
		free_matrix((void**)tasks_precedence,n);
}
