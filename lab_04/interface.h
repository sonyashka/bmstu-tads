#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define MAX_LEN 200
#define MAX_STR_LEN 100

struct stack_slot
{
    char value;
    struct stack_slot *next;
};

struct stack_all
{
    int stack_len;
    char stack_mass[MAX_LEN];
    struct stack_slot *stack_list;
};

struct mem_slot
{
    struct stack_slot *stack_slot;
    int busy;
    struct mem_slot *next;
};

void user_hint();
void end_hint();
unsigned long long tick(void);

#endif
