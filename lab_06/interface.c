#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

void user_hint()
{
    printf("Choose what you want to do:\n"
           "1 - Input words from file\n"
           "2 - Add word\n"
           "3 - Output binary tree\n"
           "4 - Output balanced tree\n"
           "5 - Output hash-table\n"
           "0 - Exit\n"
           "Choose what you want to do: ");
}

void wrong_hint()
{
    printf("Wrong value! Try again.\n");
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
