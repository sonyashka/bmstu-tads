#include <stdio.h>
#include <stdlib.h>

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

struct matrix_graph
{
    int size;
    int *adjacence_matrix;
};

void user_hint();
void end_hint();
void wrong_hint();
void default_graph(struct matrix_graph *graph);

#endif
