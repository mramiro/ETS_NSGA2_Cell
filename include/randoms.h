/*
 * randoms.h
 *
 *  Created on: 14/12/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef RANDOMS_H_
#define RANDOMS_H_

void init_random();

// Generate a random int in the range (lowerb, upperb]
int random_int(int lowerb, int upperb);

// Generate a new random double in the range (0, 1)
double random_double();

// Generate a random permutation of the int range (FROM, TO]
void random_perm(int *arr, int from, int to);

#endif /* RANDOMS_H_ */
