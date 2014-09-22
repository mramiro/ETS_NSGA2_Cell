/*
 * solution.h
 *
 *  Created on: 09/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <stdio.h>

typedef struct Sol{
	int *ord;
	int **conf;
	double makespan, energy, crowdist;
	int id, rank;
	struct Sol *next;
	struct Sol *prev;
} Sol;

typedef struct SolList{
    Sol *start;
    Sol *end;
    int len;
}SolList;

Sol* new_sol(int empty);

void copy_sol(Sol *dest, Sol *src);

Sol *clone_sol(Sol *s);

void free_sol(Sol * s);

void swap_sol(Sol *a, Sol *b);

void eval_sol(Sol* s);

void print_sol(FILE *stream, Sol* s);

void print_sol_values(FILE *stream, Sol *s);

Sol *new_vector_sol(int dim);

SolList *new_list();

/* inicializa la lista LIST */
void init_list(SolList *list);

/* Imprime en STREAM el contenido de la lista LIST */
void print_list(FILE *stream, SolList * list);

/* Elimina todas las soluciones de la lista LIST y la reinicializa. */
void clear_list(SolList * list);

/* Añade a la lista LIST la solución S.
 * Regresa el tamaño de la lista después del cambio, o -1 en caso de error */
int list_append(SolList *list, Sol *s);

/* Añade a la lista LIST una copia de la solución S.
 * Regresa el tamaño de la lista después del cambio, o -1 en caso de error */
int list_add(SolList *list, Sol *s);

/* Remueve el elemento S de la lista LIST y libera la memoria usada por el mismo.
 * Regresa el tamaño de la lista después del cambio, o -1 en caso de error */
int list_delete(SolList *list, Sol *s);

int equal_sols(Sol *a, Sol *b);

#endif /* SOLUTION_H_ */
