#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "stack_function.h"

int main()
{
    printf("Welcome to the program!\n");
    int choice= 1, rc = 1;
    struct stack_all stack;
    stack.stack_len = 0;
    stack.stack_list = NULL;
    struct mem_slot *mem = NULL;
    while (rc && (choice > 0 || choice > 4))
    {
        user_hint();
        rc = scanf("%d", &choice);
        if (rc)
            switch (choice)
            {
                case 0:
                    end_hint();
                    break;
                case 1:
                    show_stack(stack, mem);
                    break;
                case 2:
                    add_to_stack(&stack, &mem);
                    break;
                case 3:
                    delete_from_stack(&stack, &mem);
                    break;
                case 4:
                    check_brace(&stack, &mem);
                    break;
                default:
                    printf("Wrong value! Try again!\n");
            }
        else
            end_hint();
    }
    return 0;
}
