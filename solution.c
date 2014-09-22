/*
 * solution.c
 *
 *  Created on: 09/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <arrutils.h>

#include "defs.h"
#include "eval.h"

#include "solution.h"

Sol* new_sol(int empty)
{
	Sol* s = (Sol*)malloc(sizeof(Sol));
	if (s)
	{
		if (!empty)
		{
			//s->ord = new_vector_int(n); // TODO: Descomentar cuando se trabaje con diferentes órdenes
			s->conf = new_matrix_int(2,n);
		}
	}
	else
		fprintf(stderr, "Error while allocating Sol structure. Out of memory");
	return s;
}

void copy_sol(Sol *dest, Sol *src)
{
	int i;
	dest->ord = src->ord; // TODO: COMENTAR cuando se trabaje con diferentes órdenes
	for (i = 0; i < n; i++)
	{
		//dest->ord[i] = src->ord[i]; // TODO: Descomentar cuando se trabaje con diferentes órdenes
		dest->conf[machine][i] = src->conf[machine][i];
		dest->conf[vlevel][i] = src->conf[vlevel][i];
	}
	dest->makespan = src->makespan;
	dest->energy = src->energy;
	dest->crowdist = src->crowdist;
	dest->rank = src->rank;
	dest->id = src->id;
}

Sol *clone_sol(Sol *s)
{
	Sol *ret = new_sol(0);
	copy_sol(ret, s);
//	ret->prev = s->prev;
//	ret->next = s->next;
	return (ret);
}

void free_sol(Sol * s)
{
//	free_vector(s->ord); // TODO: Descomentar cuando se trabaje con diferentes órdenes
	free_matrix((void**)s->conf, 2);
	free(s);
}

void swap_sol(Sol *a, Sol *b)
{
	Sol t = *a;
	*a = *b;
	*b = t;
}

void eval_sol(Sol* s)
{
	s->makespan = calc_makespan(s->ord, s->conf, n);
	s->energy = calc_energy(s->conf, s->makespan);
}

void print_sol(FILE *stream, Sol* s)
{
	print_vector_int(stream, s->ord, n);
	print_matrix_int(stream, s->conf, 2, n);
	fprintf(stream, "Makespan: %lf\n", s->makespan);
	fprintf(stream, "Energía: %lf\n", s->energy);
}

void print_sol_values(FILE *stream, Sol *s)
{
	fprintf(stream, "%lf %lf\n", s->makespan, s->energy);
}

Sol *new_vector_sol(int dim)
{
    Sol *aux;
    aux=calloc(dim,sizeof(Sol));
    if(!aux)perror("Out of memory while allocating Sol vector.");

    return aux;
}

/* Funciones de lista de soluciones */

void init_list(SolList *list)
{
    list->start=NULL;
    list->end=NULL;
    list->len=0;
}

void print_list(FILE *stream, SolList * list)
{
    char str[200];
    Sol *s;
    s = list->start;
    while (s != NULL){
        str[0]='\0';
        sprintf(str,"%lf %lf", s->makespan,s->energy);
        fprintf(stream, "%s\n",str);
        s = s->next;
    }
}

void clear_list(SolList * list)
{
    Sol *s;
    s = list->start;
    while(s!=NULL){
        list_delete(list,s);
        s=list->start;
    }
}

int list_append(SolList *list, Sol *s)
{
	if (list->len == 0)
	{
		list->start = s;
		list->end = s;
		s->prev = NULL;
		s->next = NULL;
	}
	else
	{
		s->prev = list->end;
		list->end->next = s;
		list->end = s;
		s->next = NULL;
	}
	list->len++;
	return (list->len);
}

int list_add(SolList *list, Sol *s)
{
	Sol *new = clone_sol(s);
	return (list_append(list, new));
}

int list_delete(SolList *list, Sol *s)
{
    if(list->len==0)
    	return -1;
    if(list->len==1)
    {
        list->start=NULL;
        list->end=NULL;
    }
    else if(list->start==s)
    {
        list->start=s->next;
    }
    else if(list->end==s)
    {
        list->end=s->prev;
        list->end->next=NULL;
    }
    else
    {
		s->prev->next=s->next;
		s->next->prev=s->prev;
    }
    free_sol(s);
    list->len--;;
    return (list->len);
}

int equal_sols(Sol *a, Sol *b)
{
	if (a->energy == b->energy && a->makespan == b->makespan)
		return 0;
	return 1;
}
