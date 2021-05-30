#include <stdio.h>

#ifndef _IN_OUT_H_
#define _IN_OUT_H_

#define OK 0
#define ERR -1
#define MAX_LEN 30

struct integer_num
{
    short sign;
    short mant_len;
    short mantissa[MAX_LEN];
};

struct float_num
{
    short sign;
    short mant_len;
    short mantissa[MAX_LEN];
    int order;
};

int input(struct integer_num *first_num, struct float_num *second_num);
int input_int(struct integer_num *first_num);
int input_float(struct float_num *second_num);
void output(struct float_num num);

#endif
