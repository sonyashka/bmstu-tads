#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "queue_func.h"

int main(void)
{
    int choice = 1, rc = 1;
    struct mem_slot *mem = NULL;
    int mem_used = 0;
    srand(time(NULL));
    printf("Welcome to the program!\n");
    while (rc && choice > 0 && choice < 5)
    {
        user_hint();
        rc = scanf("%d", &choice);
        if (rc)
        {
            switch (choice)
            {
                case 1:
                    simulate_service(&mem, &mem_used);
                    break;
                case 2:
                    simulate_service_arr(&mem_used);
                    break;
                case 3:
                    show_mem(&mem);
                    break;
                case 4:
                    queue_compare(&mem);
                    break;
                default:
                    end_hint();
            }
        }
        else
            end_hint();
    }
    return 0;
}
