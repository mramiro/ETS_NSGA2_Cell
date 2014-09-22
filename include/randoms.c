/*
 * randoms.c
 *
 *  Created on: 14/12/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <time.h>

#include "randoms.h"

void init_random()
{
	srand(time(0));
}

int random_int(int lowerb, int upperb)
{
	return (((double) rand() / (RAND_MAX)) * (upperb-lowerb) + lowerb);
	//return (rand() % (upperb-lowerb) + lowerb);
}

double random_double()
{
	return ((double)rand()/(double)RAND_MAX);
}

void random_perm(int *arr, int from, int to)
{
	int i, random, temp;
	for (i = from; i < to; i++)
	{
		arr[i-from] = i;
	}
	for (i = 0; i < to-from; i++)
	{
		random = random_int(0,to-from);
		temp = arr[i];
		arr[i] = arr[random];
		arr[random] = temp;
	}
}
