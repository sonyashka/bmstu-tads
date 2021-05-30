#include <stdio.h>
#include <stdlib.h>

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define MAX_LEN 10

struct binary_tree
{
    char value[MAX_LEN + 1];
    struct binary_tree *left;
    struct binary_tree *right;
};

struct avl_tree
{
    char value[MAX_LEN + 1];
    int balance;
    struct avl_tree *left;
    struct avl_tree *right;
};

struct hash_slot
{
    char value[MAX_LEN + 1];
    struct hash_slot *next;
};

struct tree
{
    int count;
    struct binary_tree *binary;
    struct avl_tree *avl;
    int hash_size;
    int *rand;
    struct hash_slot **hash_table;
};

void user_hint();
void wrong_hint();
void end_hint();
unsigned long long tick(void);

#endif
