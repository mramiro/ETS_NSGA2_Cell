/*
 * io.c
 *
 *  Created on: 18/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"
#include "io.h"

static char *instance_name;

static void file_error(const char *file_name, int halt)
{
	char buffer[512];
	strcpy(buffer, "Error opening file ");
	strcat(buffer, file_name);
	if (halt)
		terminate((const char *)buffer);
	else
		perror((const char *)buffer);
}

static void read_error(const char *file_name, int halt)
{
	char buffer[512];
	strcpy(buffer, "Error reading file ");
	strcat(buffer, file_name);
	if (halt)
		terminate((const char *)buffer);
	else
		perror((const char *)buffer);
}

static int get_instance_size(FILE *pInFile)
{
	char cTemp = 0;
	char szTag[32];
	int nRet;

	n = 0;
	m = 0;
	n_edges = 0;
	memset(&szTag[0], 0, sizeof(szTag));
	while (cTemp != EOF)
	{
		cTemp = fgetc(pInFile);
		if (cTemp == '#')
		{
			nRet = fscanf(pInFile, "%s", &szTag[0]);
			if (nRet)
			if (strcmp(&szTag[0], "tasks") == 0)
			{
				do
				{
					cTemp = fgetc(pInFile);
					if (cTemp == '\n')
						n++;
					if (m == 0)
					{
						m = 1;
						do
						{
							cTemp = fgetc(pInFile);
							if (cTemp == ',')
								m++;
						}
						while (cTemp != '\n');
					}
				} while (cTemp != '#');
				fseek(pInFile, ftell(pInFile)-1, 0);
			}
			if (strcmp(&szTag[0], "edges") == 0)
			{
				do
				{
					cTemp = fgetc(pInFile);
					if (cTemp == '\n')
						n_edges++;
				} while (cTemp != '#');
				n_edges--;
				fseek(pInFile, ftell(pInFile)-1, 0);
			}
			if (strcmp(&szTag[0], "end") == 0)
			{
				break;
			}
		}
		memset(&szTag[0], 0, sizeof(szTag));
	}
	nRet = fseek(pInFile, 0, 0);
	return (nRet);
}

FILE *open_file(const char *file_path, const char *mode, int halt_on_err)
{
	FILE *ret;
	ret = fopen(file_path, mode);
	if (!ret)
	{
		file_error(file_path, halt_on_err);
	}
	return (ret);
}

void close_file(FILE *file)
{
	if (file)
		if (fclose(file))
			perror("Error closing file.");
}

int read_instance(const char * instance_path)
{
	int nRet;
	FILE *pInFile;

	// Guardar el nombre de la instancia
	instance_name = (char*)instance_path;

	pInFile = open_file(instance_path, "r", 1);

	nRet = get_instance_size(pInFile);

	// Reservar memoria para arreglos
	p_matrix = new_matrix_double(n, m);
	dag_edges = new_matrix_double(n_edges, 3);

	int i, j;
	while (fgetc(pInFile) != '#');
	while (fgetc(pInFile) != '#');
	nRet = fscanf(pInFile, "%*s");

	for (i = 0; i < n; i++)
	{
		nRet = fscanf(pInFile, "%*s");
		for (j = 0; j < m; j++)
		{
			nRet = fscanf(pInFile, "%lf%*c", &p_matrix[i][j]);
		}
	}
	nRet = fscanf(pInFile, "%*s");
	for (i = 0; i < n_edges; i++)
	{
		nRet = fscanf(pInFile, "%*s");
		for (j = 0; j < 3; j++)
		{
			nRet = fscanf(pInFile, "%lf%*c", &dag_edges[i][j]);
		}
	}
	close_file(pInFile);

	if (!nRet)
			read_error(instance_path, 1);

	return (nRet);
}

int read_speed_pairs(const char * vspairs_path)
{
	int nRet;
	FILE *pInFile;
	int nPairs = 0;

	// Leer speed pairs
	pInFile = open_file(vspairs_path, "r", 1);


	nRet = fscanf(pInFile, "%d", &n_vlevels);
	nRet = fscanf(pInFile, "%d", &nPairs);

	if (m < nPairs)
		vspairs = new_matrix_double(n_vlevels, nPairs<<1);
	else
		vspairs = new_matrix_double(n_vlevels, m<<1);

	int i, j;
	for (i = 0; i < n_vlevels; i++)
	{
		for (j = 0; j < nPairs<<1; j++)
		{
			nRet = fscanf(pInFile, "%lf", &vspairs[i][j]);
		}
	}
	close_file(pInFile);

	// Si la instancia tiene más maquinas que las disponibles en los speed pairs
	// se hace un round robin para asignar pairs a las máquinas que faltan.
	if (m > nPairs)
	{
		int cur_pair;
		for (i = 0; i < n_vlevels; i++)
		{
			for (j = nPairs<<1; j < m<<1; j++)
			{
				cur_pair = j%(nPairs<<1);
				vspairs[i][j] = vspairs[i][cur_pair];
			}
		}
	}

	if (!nRet)
		read_error(vspairs_path, 1);

	return (nRet);
}

void print_instance_name(FILE *stream)
{
	fprintf(stream, "%s",instance_name);
}

//void print_all(Sol* s)
//{
//	print_matrix_double(p_matrix, n, m);
//	printf("-------------\n");
//	print_matrix_double(dag_edges, n_edges, 3);
//	printf("-------------\n");
//	print_matrix_double(vspairs, n_vlevels, m << 1);
//	printf("-------------\n");
//	print_vector_int(s->ord, n);
//	printf("-------------\n");
//	print_matrix_int(s->conf, 2, n);
//	printf("-------------\n");
//	printf("Makespan: %f\n", s->makespan);
//	printf("Energía: %f\n", s->energy);
//}

double print_time(FILE *stream, clock_t start, clock_t end)
{
	double time = (double)(end-start)/CLOCKS_PER_SEC;
	fprintf(stream, "%lf\n", time);
	return (time);
}
