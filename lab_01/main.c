#include <stdio.h>
#include "in_out.h"
#include "multiplication.h"

int main()
{
    struct integer_num first_num;
    struct float_num second_num;
    int res = input(&first_num, &second_num);
    if (!res)
    {
        struct float_num third_num;
        res = multi(first_num, second_num, &third_num);
        if (!res)
            output(third_num);
    }
    return res;
}
