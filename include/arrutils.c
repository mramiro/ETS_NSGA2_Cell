/*
 * arrutils.c
 *
 *  Created on: 26/09/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include "arrutils.h"

void print_vector_int(FILE *stream, int* ptr, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stream, "%d ", ptr[i]);
	}
	fprintf(stream, "\n");
}

void print_vector_float(FILE *stream, float* ptr, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stream, "%f ", ptr[i]);
	}
	fprintf(stream, "\n");
}

void print_vector_double(FILE *stream, double* ptr, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stream, "%lf ", ptr[i]);
	}
	fprintf(stream, "\n");
}

void print_matrix_int(FILE *stream, int** ptr, int y, int x)
{
	int i;
	for (i = 0; i < y; i++)
	{
		print_vector_int(stream, ptr[i], x);
	}
}

void print_matrix_float(FILE *stream, float** ptr, int y, int x)
{
	int i;
	for (i = 0; i < y; i++)
	{
		print_vector_float(stream, ptr[i], x);
	}
}

void print_matrix_double(FILE *stream, double** ptr, int y, int x)
{
	int i;
	for (i = 0; i < y; i++)
	{
		print_vector_double(stream, ptr[i], x);
	}
}

void set_vector_int(int* ptr, int len, int val)
{
	int i;
	for (i = 0; i < len; i++)
	{
		ptr[i] = val;
	}
}

void set_vector_float(float* ptr, int len, float val)
{
	int i;
	for (i = 0; i < len; i++)
	{
		ptr[i] = val;
	}
}

void set_vector_double(double* ptr, int len, double val)
{
	int i;
	for (i = 0; i < len; i++)
	{
		ptr[i] = val;
	}
}

void set_matrix_int(int** ptr, int y, int x, int val)
{
	int i;
	for (i = 0; i < y; i++)
	{
		set_vector_int(ptr[i], x, val);
	}
}

void set_matrix_float(float** ptr, int y, int x, float val)
{
	int i;
	for (i = 0; i < y; i++)
	{
		set_vector_float(ptr[i], x, val);
	}
}

void set_matrix_double(double** ptr, int y, int x, double val)
{
	int i;
	for (i = 0; i < y; i++)
	{
		set_vector_double(ptr[i], x, val);
	}
}
