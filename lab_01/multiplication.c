#include <stdio.h>
#include "multiplication.h"

int zxz(short *arr, short len_arr, short *first_mantissa, short len, short second_mant);
void sum_of_mult(short *result_of_mult, short *arr_of_mult,
                 short len_of_arr_of_mult, int j);
void rounding(short *result_of_mult, int i);

int multi(struct integer_num first_num, struct float_num second_num,
          struct float_num *third_num)
{
    int res = OK;
    short result_of_mult[2*MAX_LEN + 1] = {0}, arr_of_mult[MAX_LEN][MAX_LEN + 1] = {0};
    short len_of_arr_of_mult[MAX_LEN];
    for (int i = second_num.mant_len - 1; i >= 0; i--)
    {
       len_of_arr_of_mult[i] = zxz(arr_of_mult[i], len_of_arr_of_mult[i],
                                   first_num.mantissa, first_num.mant_len,
                                   second_num.mantissa[i]);
    }
    int j = 0;
    int res_len = first_num.mant_len - 1;
    for (int i = second_num.mant_len - 1; i >= 0; i--)
    {
        sum_of_mult(result_of_mult, arr_of_mult[i],
                    len_of_arr_of_mult[i], j);
        j++;
        res_len++;
    }
    third_num->order = second_num.order - second_num.mant_len;
    if (result_of_mult[res_len] != 0)
    {
        res_len++;
    }
    third_num->order += res_len;
    int k = 0;
    if (res_len >= MAX_LEN)
    {
        k = res_len - MAX_LEN;
        //third_num->order += k;
        if (result_of_mult[k - 1] > 4)
            rounding(result_of_mult, k);
    }
    while (result_of_mult[k] == 0)
        k++;
    j = 0;
    for (int i = res_len - 1; i >= k; i--)
    {
        third_num->mantissa[j] = result_of_mult[i];
        j++;
    }
    third_num->mant_len = res_len - k;
    if (third_num->order > 99999 || third_num->order < -99999)
    {
        res = ERR;
        printf("Order overflow!");
    }
    if (first_num.sign && second_num.sign)
        third_num->sign = 0;
    else if (first_num.sign || second_num.sign)
        third_num->sign = 1;
    return res;
}

int zxz(short *arr, short len_arr, short *first_mantissa, short len, short second_mant)
{
    int c = 0, j = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        c = first_mantissa[i] * second_mant + arr[j];
        if (c < 10)
        {
            arr[j] = c;
        }
        else
        {
            arr[j] = c % 10;
            arr[j + 1] = c / 10;
        }
        j++;
    }
    if (arr[len] == 0)
        len_arr = len;
    else
    {
        len_arr = len + 1;
    }
    return len_arr;
}

void sum_of_mult(short *result_of_mult, short *arr_of_mult,
                 short len_of_arr_of_mult, int j)
{
    for (int i = 0; i < len_of_arr_of_mult; i++)
    {
        result_of_mult[j + i] += arr_of_mult[i];
        if (result_of_mult[j + i] >= 10)
        {
            result_of_mult[j + i + 1] += result_of_mult[j + i] / 10;
            result_of_mult[j + i] %= 10;
        }
    }
}

void rounding(short *result_of_mult, int i)
{
    if (result_of_mult[i] + 1 == 10)
    {
        result_of_mult[i] = 0;
        rounding(result_of_mult, i + 1);
    }
    else
        result_of_mult[i]++;
}
