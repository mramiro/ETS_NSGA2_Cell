/*
 * arrutils.h
 *
 *  Created on: 26/09/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef ARRUTILS_H_
#define ARRUTILS_H_

#include <stdio.h>

void print_vector_int(FILE *stream, int* ptr, int len);

void print_vector_float(FILE *stream, float* ptr, int len);

void print_vector_double(FILE *stream, double* ptr, int len);

void print_matrix_int(FILE *stream, int** ptr, int y, int x);

void print_matrix_float(FILE *stream, float** ptr, int y, int x);

void print_matrix_double(FILE *stream, double** ptr, int y, int x);

//----------------

void set_vector_int(int* ptr, int len, int val);

void set_vector_float(float* ptr, int len, float val);

void set_vector_double(double* ptr, int len, double val);

void set_matrix_int(int** ptr, int y, int x, int val);

void set_matrix_float(float** ptr, int y, int x, float val);

void set_matrix_double(double** ptr, int y, int x, double val);

#endif /* ARR_UTILS_H_ */
