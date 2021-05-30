#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

int main(void)
{
    int res = 2, rc = 1;
    printf("Welcome to the program! ");
    while (res > 0 && res < 5 && rc)
    {
        user_hint();
        rc = scanf("%d", &res);
        if (rc)
            switch (res)
            {
                case 1:
                    classic_mult();
                    break;
                case 2:
                    random_mult();
                    break;
                case 3:
                    file_mult();
                    break;
                case 4:
                    efficiency_table();
                    break;
                default:
                end_hint();
             }
        else
            end_hint();
    }
    return 0;
}
