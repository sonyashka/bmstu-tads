#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

#ifndef _IN_OUT_H_
#define _IN_OUT_H_

void read_tree(FILE *file, struct tree *tree);
void binary_out(struct tree tree);
void avl_out(struct tree tree);
void hash_out(struct tree tree);
void add_leaf(FILE *file, struct tree *tree);

#endif
