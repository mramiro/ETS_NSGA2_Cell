/*
 * eval.h
 *
 *  Created on: 17/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef EVAL_H_
#define EVAL_H_

double calc_makespan(int *order, int **conf, int len);

double calc_makespan_heft(int *order, int *assignation, int len);

double calc_energy(int **conf, double makespan);

int is_feasible(int *order, int len);

int **gen_random_conf();

void free_precedence_matrix();

#endif /* EVAL_H_ */
