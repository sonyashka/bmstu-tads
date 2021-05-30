#include <stdio.h>
#include <stdlib.h>

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

void vector_mult();
void classic_mult();
void random_mult();
void file_mult();
void sparse_multiplication(int *V, int *JV, int n, int *A, int *IA, int *JA, int *non_zero_res, int *res);
void efficiency_table();

void long_out(char a,int count);
void user_hint();
void wrong_data();
void end_hint();

#endif
