#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "stack_function.h"

unsigned long add_list(struct stack_slot **stack_list, char brace, struct mem_slot **mem);
void add_mem(struct mem_slot **mem, struct stack_slot *slot);
unsigned long delete_list(struct stack_slot **stack_list, struct mem_slot **mem);
void delete_mem(struct mem_slot **mem, struct stack_slot *slot);

void show_mem(struct mem_slot *mem);

int check_mass(struct stack_all *stack, unsigned long *time);
int check_list(struct stack_all *stack, unsigned long *time);

unsigned long add_list_v2(struct stack_slot **stack_list, char brace);
unsigned long delete_list_v2(struct stack_slot **stack_list);

void add_to_stack(struct stack_all *stack, struct mem_slot **mem)
{
    if (stack->stack_len >= MAX_LEN)
    {
        printf("Stack is full! Clear or delete it.\n");
    }
    else
    {
        char *brace = "(){}[]";
        char buf_str[MAX_STR_LEN];
        gets(buf_str);
        printf("Input symbols, which need to add (no more 15 symbols): ");
        fgets(buf_str, MAX_STR_LEN + 1, stdin);
        if (buf_str[0] == '\n')
            printf("Empty string!\n");
        else if (buf_str[strlen(buf_str) - 1] != '\n')
        {
            printf("Too mucnh symbols!\n");
            gets(buf_str);
        }
        else
        {
            long time1 = 0, time2 = 0, t_buf = 0, len_dif = 0;
            for (int i = 0; i < (int)strlen(buf_str) - 1; i++)
            {
                for (int j = 0; j < (int)strlen(brace); j++)
                {
                    if (buf_str[i] == brace[j] && stack->stack_len < MAX_LEN)
                    {
                        t_buf = tick();
                        stack->stack_mass[stack->stack_len] = brace[j];
                        time1 += tick() - t_buf;
                        time2 += add_list(&stack->stack_list, brace[j], mem);
                        stack->stack_len++;
                        len_dif++;
                    }
                    else if (buf_str[i] == brace[j])
                        printf("Stack filled up while adding! Some elements was added.\n"
                               "Choose 'show stack' to more info.\n"
                               "Clear or delete some and try again.\n");
                }
            }
            if (stack->stack_list == NULL)
                printf("GG\n");
            else
            {
                printf("Time of mass add: %ld tacts\n", time1 / len_dif);
                printf("Time of list add: %ld tacts\n", time2 / len_dif);
            }
        }
    }
}

unsigned long add_list(struct stack_slot **stack_list, char brace, struct mem_slot **mem)
{
    unsigned long t_buf = tick();
    struct stack_slot *slot = malloc(sizeof(struct stack_slot));
    slot->value = brace;
    if (stack_list != NULL)
        slot->next = *stack_list;
    else
    {
        slot->next = NULL;
    }
    *stack_list = slot;
    t_buf = tick() - t_buf;
    add_mem(mem, slot);
    return t_buf;
}

void add_mem(struct mem_slot **mem, struct stack_slot *slot)
{
    int f = 0;
    struct mem_slot *a = *mem;
    while (a && !f)
    {
        if (slot == a->stack_slot)
        {
            f = 1;
            a->busy = 1;
        }
        else
            a = a->next;
    }
    if (!f)
    {
        struct mem_slot *buf = malloc(sizeof(struct mem_slot));
        buf->stack_slot = slot;
        buf->busy = 1;
        buf->next = *mem;
        *mem = buf;
    }
}

void delete_from_stack(struct stack_all *stack, struct mem_slot **mem)
{
    if (stack->stack_len == 0)
    {
        printf("Stack is empty! Nothing to delete.\n");
    }
    else
    {
        long time1 = 0, time2 = 0, t_buf = 0;
        t_buf = tick();
        stack->stack_mass[stack->stack_len - 1] = '\0';
        time1 = tick() - t_buf;
        time2 = delete_list(&stack->stack_list, mem);
        stack->stack_len--;
        printf("Time of mass delete: %ld tacts\n", time1);
        printf("Time of list delete: %ld tacts\n", time2);
    }
}

unsigned long delete_list(struct stack_slot **stack_list, struct mem_slot **mem)
{
    unsigned long t_buf = tick();
    struct stack_slot *buf = *stack_list;
    *stack_list = (*stack_list)->next;
    delete_mem(mem, buf);
    free(buf);
    t_buf = tick() - t_buf;
    return t_buf;
}

void delete_mem(struct mem_slot **mem, struct stack_slot *slot)
{
    int f = 0;
    struct mem_slot *a = *mem;
    while (a && !f)
    {
        if (slot == a->stack_slot)
        {
            f = 1;
            a->busy = 0;
        }
        else
            a = a->next;
    }
}

void show_stack(struct stack_all stack, struct mem_slot *mem)
{
    if (stack.stack_len == 0)
    {
        printf("Stack is empty!\n");
    }
    else
    {
        printf("Stack fullness is %d of %d\n", stack.stack_len, MAX_LEN);
        printf("-> ");
        for (int i = stack.stack_len - 1; i >= 0; i--)
            printf("'%c' / ", stack.stack_mass[i]);
        printf("\n");
        show_mem(mem);
    }
}

void show_mem(struct mem_slot *mem)
{
    printf("Usage of memory:\n");
    struct mem_slot *buf = mem;
    while (buf)
    {
        printf("%p is ", buf->stack_slot);
        if (buf->busy == 1)
            printf("use.\n");
        else
            printf("not used.\n");
        buf = buf->next;
    }
}

void check_brace(struct stack_all *stack, struct mem_slot **mem)
{
    int res_mass = 0, res_list = 0;
    unsigned long time1 = 0, time2 = 0;
    res_mass = check_mass(stack, &time1);
    res_list = check_list(stack, &time2);
    struct mem_slot *buf = *mem;
    while (buf)
    {
        buf->busy = 0;
        buf = buf->next;
    }
    if (res_mass == res_list)
    {
        if (res_list == 0)
            printf("Brackets are placed correctly.\n");
        else
            printf("Brackets are placed UNcorrectly.\n");
        printf("Stack is clear!\n");
        printf("Time of mass checking: %lu tacts\n", time1);
        printf("Time of list checking: %lu tacts\n", time2);
        printf("Cleared memory by mass: %d\n", (int)(sizeof(char) * stack->stack_len));
        printf("Cleared memory by list: %d\n", (int)(sizeof(struct stack_slot) * stack->stack_len));
        stack->stack_len = 0;
    }
    else
        printf("Something goes wrong!\n");
}

int check_mass(struct stack_all *stack, unsigned long *time)
{
    int res = 0, buf_len = 0;
    char buf[MAX_LEN];
    *buf = '\0';
    unsigned long t_buf = tick();
    for (int i = stack->stack_len - 1; i >= 0 && !res; i--)
    {
        switch (stack->stack_mass[i])
        {
            case ')':
                *(buf + buf_len) = ')';
                *(buf + buf_len + 1) = '\0';
                buf_len++;
                break;
            case '}':
                *(buf + buf_len) = '}';
                *(buf + buf_len + 1) = '\0';
                buf_len++;
                break;
            case ']':
                *(buf + buf_len) = ']';
                *(buf + buf_len + 1) = '\0';
                buf_len++;
                break;
            case '(':
                if (*(buf + buf_len - 1) != ')')
                    res = 1;
                else
                {
                    *(buf + buf_len - 1) = '\0';
                    buf_len--;
                }
                break;
            case '{':
                if (*(buf + buf_len - 1) != '}')
                    res = 1;
                else
                {
                    *(buf + buf_len - 1) = '\0';
                    buf_len--;
                }
                break;
            case '[':
                if (*(buf + buf_len - 1) != ']')
                    res = 1;
                else
                {
                    *(buf + buf_len - 1) = '\0';
                    buf_len--;
                }
                break;
        }
    }
    *time = tick() - t_buf;
    return res;
}

int check_list(struct stack_all *stack, unsigned long *time)
{
    int res = 0;
    unsigned long t_buf = tick();
    struct stack_slot *buf = NULL;
    while (stack->stack_list != NULL && !res)
    {
        switch (stack->stack_list->value)
        {
            case ')':
                add_list_v2(&buf, ')');
                break;
            case '}':
                add_list_v2(&buf, '}');
                break;
            case ']':
                add_list_v2(&buf, ']');
                break;
            case '(':
                if (buf != NULL && buf->value == ')')
                    delete_list_v2(&buf);
                else
                    res = 1;
                break;
            case '{':
                if (buf != NULL && buf->value == '}')
                    delete_list_v2(&buf);
                else
                    res = 1;
                break;
            case '[':
                if (buf != NULL && buf->value == ']')
                    delete_list_v2(&buf);
                else
                    res = 1;
                break;
        }
        delete_list_v2(&stack->stack_list);
    }
    while (stack->stack_list != NULL)
        delete_list_v2(&stack->stack_list);

    *time = tick() - t_buf;

    return res;
}

unsigned long add_list_v2(struct stack_slot **stack_list, char brace)
{
    unsigned long t_buf = 0;
    struct stack_slot *slot = malloc(sizeof(struct stack_slot));
    t_buf = tick();
    slot->value = brace;
    if (stack_list != NULL)
        slot->next = *stack_list;
    else
    {
        slot->next = NULL;
    }
    *stack_list = slot;
    t_buf = tick() - t_buf;
    return t_buf;
}

unsigned long delete_list_v2(struct stack_slot **stack_list)
{
    unsigned long t_buf = tick();
    struct stack_slot *buf = *stack_list;
    *stack_list = (*stack_list)->next;
    t_buf = tick() - t_buf;
    free(buf);
    return t_buf;
}
