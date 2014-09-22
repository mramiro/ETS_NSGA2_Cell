/*
 * pr.h
 *
 *  Created on: 11/11/2013
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef PR_H_
#define PR_H_

#include "nsga2.h"
#include "solution.h"

int hamming_distance(Sol *a, Sol *b);

void path_relinking_i(Sol *s_out, Sol *s_init, Sol *s_guide, Bounds *bounds);

int path_relinking_e(Sol *s_out, Sol *s_init, Sol *s_guide, Bounds *bounds, int max_sols);

int new_pr(Sol *s_out, Sol *s_init, Sol *s_guide);

#endif /* PR_H_ */
