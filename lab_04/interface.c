#include <stdio.h>
#include "interface.h"

void user_hint()
{
    printf("Choose what you want:\n"
           "1 - Show stack state\n"
           "2 - Add to stack\n"
           "3 - Delete from stack\n"
           "4 - Check brace placement(with clear stack)\n"
           "0 - Exit\n"
           "Choose what you want: ");
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
