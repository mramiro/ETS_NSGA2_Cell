/*
 * main.c
 *
 *  Created on: 02/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <time.h>

#include "defs.h"
#include "io.h"
#include "eval.h"
#include "nsga2.h"
#include "eval.h"
#include "cellular.h"

void free_mem()
{
	free_matrix((void**)p_matrix, n);
	free_matrix((void**)dag_edges, n_edges);
	free_matrix((void**)vspairs, n_vlevels);
	free_precedence_matrix();
}

int main (int argc, const char *argv[])
{
	int ret;
	const char * instance_path = DEFAULT_INSTANCE;
	const char * vspairs_path = DEFAULT_VSPAIRS;
	if (argc > 1)
	{
		instance_path = argv[1];
		vspairs_path = argv[2];
	}
	output_file = NULL;
	if (argc > 3)
		output_file = argv[3];

	read_instance(instance_path);
	read_speed_pairs(vspairs_path);

	/* ------ AQUÍ VA EL MÉTODO DE SOLUCIÓN ------ */

	clock_t ini_time, end_time;
	ini_time = clock();

	ret = cell_nsga2();
//	ret = nsga2();

	end_time = clock();
	printf("Tiempo total: ");
	print_time(stdout, ini_time, end_time);
	printf("\n");

	/* ------------------------------------------- */

	free_mem();

	return (ret);
}
