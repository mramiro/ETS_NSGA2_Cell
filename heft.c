/*
 * heft.c
 *
 *  Created on: 09/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <float.h>
#include <stdlib.h>

#include "defs.h"
#include "eval.h"
#include "blevel.h"

#include "heft.h"

void heft(int *order, int *assignation)
{
	int i, j, cur_task, best_machine = -1;
	double min_makespan,cur_makespan;

	for (i = 0; i < n; i++)
	{
		cur_task = order[i];
		min_makespan = DBL_MAX;
		for (j = 0; j < m; j++)
		{
			assignation[cur_task] = j;
			cur_makespan = calc_makespan_heft(order, assignation, i+1);
			if (cur_makespan < min_makespan)
			{
				min_makespan = cur_makespan;
				best_machine = j;
			}
		}
		assignation[cur_task] = best_machine;
	}
}
