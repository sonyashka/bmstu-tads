#include <stdio.h>
#include <stdlib.h>
#include <graphviz\gvc.h>
#include "interface.h"

#ifndef _IN_OUT_H_
#define _IN_OUT_H_

void input_graph(struct matrix_graph *graph);
void output_graph(struct matrix_graph graph);
void out_res(struct matrix_graph *res);

#endif
