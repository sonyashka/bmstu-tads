#include <stdio.h>
#include <time.h>
#include "interface.h"

#ifndef _STACK_FUNCTION_H_
#define _STACK_FUNCTION_H_

void show_stack(struct stack_all stack, struct mem_slot *mem);
void add_to_stack(struct stack_all *stack, struct mem_slot **mem);
void delete_from_stack(struct stack_all *stack, struct mem_slot **mem);
void check_brace(struct stack_all *stack, struct mem_slot **mem);

#endif
