#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"

void user_hint()
{
    printf("Choose what you want to do:\n"
           "1 - Simulate servicing 1000 requests(by list)\n"
           "2 - Simulate servicing 1000 requests(by array)\n"
           "3 - Show memory list\n"
           "4 - Efficiency comparing\n"
           "0 - Exit\n"
           "Choose what you want to do: ");
}

void end_hint()
{
    printf("Its end of working the program!\n");
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d));
    return d;
}

double get_time(int t1, int t2)
{
    double r = (double)rand() / (double)RAND_MAX;
    r = (t2 - t1) * r + t1;
    return r;
}

int chance(void)
{
    int mas[10] = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0};
    int ind = rand() % 10;
    return mas[ind];
}
