/*
 * errors.c
 *
 *  Created on: 18/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>

#include "defs.h"

void terminate(const char *error)
{
	perror(error);
	printf("\nExecution halted.\n");
	exit(EXIT_FAILURE);
}
