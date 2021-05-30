#include <stdio.h>
#include <string.h>
#include "in_out.h"

void int_hint();
void float_hint();
int int_num_parser(char *str,int len, struct integer_num *num);
int float_num_parser(char *str, int len, struct float_num *num);
int order_parser(char *str, int len, int i, struct float_num *num);
void shift(short *arr, short len);

void out_hint();

int input(struct integer_num *first_num, struct float_num *second_num)
{
    printf("Welcome to multiplication of integer and float numbers!\n");
    int res = OK;
    int res1 = input_int(first_num);
    if (res1)
    {
        res = ERR;
        printf("Incorrect integer number!");
    }
    else
    {
        res1 = input_float(second_num);
        if (res1)
        {
            res = ERR;
            printf("Incorrect float number!");
        }
    }
    return res;
}

int input_int(struct integer_num *first_num)
{
    char int_str[MAX_LEN + 4];
    int_hint();
    fgets(int_str, MAX_LEN + 4, stdin);
    int res = OK, len = strlen(int_str);
    if ((len == MAX_LEN + 3 && (int_str[0] == '+' || int_str[0] == '-'))
            || (len == MAX_LEN + 2 && (int_str[0] >= '0' && int_str[0] <= '9')))
    {
        printf("Number so long. ");
        res = ERR;
    }
    else if (int_str[0] == '\n')
    {
        printf("Empty number. ");
        res = ERR;
    }
    else
    {
        int_str[len - 1] = '\0';
        res = int_num_parser(int_str, len, first_num);
    }
    return res;
}

int int_num_parser(char *str, int len, struct integer_num *num)
{
    int res = OK, i = 0;
    if (str[0] == '-')
    {
        num->sign = 1;
        i = 1;
    }
    else if (str[0] == '+')
    {
        num->sign = 0;
        i = 1;
    }
    else
        num->sign = 0;
    while (str[i] == '0' && i != len - 2)
    {
        i++;
        printf("*");
    }
    for (int j = i; j < len - 1 && res == OK; j++)
    {
        if (str[j] >= '0' && str[j] <= '9')
        {
            num->mantissa[j - i] = str[j] - 48;
        }
        else
            res = ERR;
    }
    if (!res)
        num->mant_len = len - 1 - i;
    return res;
}

int input_float(struct float_num *second_num)
{
    char float_str[MAX_LEN + 13];
    float_hint();
    fgets(float_str, MAX_LEN + 13, stdin);
    int res = OK, len = strlen(float_str);
    if (float_str[0] == '\n')
    {
        printf("Empty number. ");
        res = ERR;
    }
    else
    {
        float_str[len - 1] = '\0';
        second_num->sign = 0;
        res = float_num_parser(float_str, len, second_num);
    }
    return res;
    return OK;
}

int float_num_parser(char *str, int len, struct float_num *num)
{
    int res = OK, i = 0;
    if (str[0] == '-')
    {
        i = 1;
        num->sign = 1;
    }
    else if (str[0] == '+')
    {
        i = 1;
        num->sign = 0;
    }
    else
        num->sign = 0;
    int flag = 0, flag_index = -1;
    num->mant_len = 0;
    for (int j = i; j < len - 1 &&
         str[j] != ' ' && str[j] != 'E' && res == OK; j++)
    {
        if (str[j] == '.' && !flag)
        {
            flag = 1;
            if (num->mant_len > 0)
                flag_index = num->mant_len - 1;
            else
                flag_index = 0;
        }
        else if (str[j] >= '0' && str[j] <= '9')
        {
            num->mantissa[num->mant_len] = str[j] - 48;
            num->mant_len++;
        }
        else
            res = ERR;
    }
    if (num->mant_len > MAX_LEN)
    {
        res = ERR;
        printf("So long mantissa. ");
    }
    else
    {
        i += num->mant_len + flag;
        num->order = 0;
        if (str[i] != '\0' && str[i] == ' ')
            i++;
        if (str[i] != '\0' && str[i] == 'E')
        {
            i++;
            res = order_parser(str, len, i, num);
        }
        else if (str[i] != '\0')
            res = ERR;
        if (!res && flag_index >= 0)
        {
            num->order += flag_index + 1;
            flag_index = 0;
        }
        else if (!res && flag_index == -1)
            num->order += num->mant_len;
        while (!res && num->mant_len > 1 && num->mantissa[0] == 0)
        {
            shift(num->mantissa, num->mant_len);
            num->order--;
            num->mant_len--;
        }
        if (str[0] == '.')
            num->order--;
    }
    return res;
}

void shift(short *arr, short len)
{
    for (int i = 0; i < len - 1; i++)
        arr[i] = arr[i + 1];
}

int order_parser(char *str, int len, int i, struct float_num *num)
{
    int res = OK, sign = 0;
    if (str[i] == ' ')
        i++;
    if (str[i] == '-')
    {
        sign = 1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    for (int j = i; j < len - 1; j++)
    {
        num->order *= 10;
        if (str[j] >= '0' && str[j] <= '9')
            num->order += str[j] - 48;
        else
            res = ERR;
    }
    if (num->order > 99999)
    {
        res = ERR;
        printf("So long order.");
    }
    else if (sign)
        num->order *= -1;
    return res;
}

void int_hint()
{
    printf("Input integer number in format:\n");
    printf("sign|number(no more 30 symbols)\n"
           "-without spaces between sign and number:\n");
    printf("0    5    10   15   20   25   30\n");
    printf("|    |    |    |    |    |    |\n");
}

void float_hint()
{
    printf("Input float number in format:\n");
    printf("sign mantissa|mantissa(no more 30 symbols)|"
           "sign order|order(no more 5 symbols)\n"
           "-without spaces between sign and number\n"
           "-order represented as 'E'(english)\n"
           "-no more 1 space between number, exponent and exponent order:\n");
    printf("0    5    10   15   20   25   30\n");
    printf("|    |    |    |    |    |    |\n");
}

void output(struct float_num num)
{
    out_hint();
    if (num.mant_len > 0)
    {
        if (num.sign)
            printf("-");
        printf("0.");
        for (int i = 0; i < num.mant_len; i++)
            printf("%d", num.mantissa[i]);
        printf(" E %d", num.order);
    }
    else
        printf("0");
}

void out_hint()
{
    printf("Result of multiplication:\n");
    printf("sign mantissa|mantissa(no more 30 symbols)|"
           "sign order|order(no more 5 symbols)\n");
    printf("  0    5    10   15   20   25   30\n");
    printf("  |    |    |    |    |    |    |\n");
}
