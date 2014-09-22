/*
 * io.h
 *
 *  Created on: 18/10/2012
 *      Author: Miguel A. Ramiro <mike.longbow@gmail.com>
 */

#ifndef IO_H_
#define IO_H_

#include <stdio.h>
#include <time.h>

const char *output_file;

FILE *open_file(const char *file_path, const char *mode, int halt_on_err);

void close_file(FILE *file);

int read_instance(const char * instance_path);

int read_speed_pairs(const char * vspairs_path);

void print_instance_name(FILE *stream);

double print_time(FILE *stream, clock_t start, clock_t end);

#endif /* IO_H_ */
