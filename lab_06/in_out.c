#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interface.h"
#include "in_out.h"

unsigned long place_leaf(struct binary_tree *leaf, struct tree *tree, int *compare);
struct avl_tree *search_inc(struct avl_tree *leaf, struct avl_tree *node, int *compare);
struct avl_tree *balance(struct avl_tree *node);
struct avl_tree *rotright(struct avl_tree *node);
struct avl_tree *rotleft(struct avl_tree *node);
int factor(struct avl_tree *node);
void fixheight(struct avl_tree *node);
int height(struct avl_tree *node);

void binary_cycle_out(struct binary_tree *leaf, FILE *f);
void avl_cycle_out(struct avl_tree *leaf, FILE *f);

unsigned long hash_func(char *leaf, struct tree *tree, int *compare);

void read_tree(FILE *file, struct tree *tree)
{
    printf("Input size of hash-table: ");
    int rc = 0;
    rc = scanf("%d", &tree->hash_size);
    if (rc && tree->hash_size > 0)
    {
        printf("Hash size is acceptable.\n");
        tree->rand = malloc(tree->hash_size * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < tree->hash_size; i++)
            tree->rand[i] = rand() % tree->hash_size;
        tree->hash_table = calloc(tree->hash_size, sizeof(struct hash_slot *));
    }
    else
    {
        printf("Hash size is not acceptable.\n");
        return;
    }
    struct binary_tree *buf1 = NULL;
    struct avl_tree *buf2 = NULL;
    char leaf[MAX_LEN + 2];
    int compare = 0;
    while (!feof(file))
    {
        leaf[0] = '\0';
        fgets(leaf, MAX_LEN + 2, file);
        if (strlen(leaf) != 0 && leaf[strlen(leaf) - 1] != '\n')
        {
            printf("Too many symbols in element. Rewrite file.\n");
            return;
        }
        else
            leaf[strlen(leaf) - 1] = '\0';

        if (strlen(leaf) != 0)
        {
            buf1 = malloc(sizeof(struct binary_tree));
            strcpy(buf1->value, leaf);
            buf1->left = buf1->right = NULL;

            buf2 = malloc(sizeof(struct avl_tree));
            strcpy(buf2->value, leaf);
            buf2->balance = 1;
            buf2->left = buf2->right = NULL;

            if (tree->count == 0)
            {
                tree->binary = buf1;
                tree->avl = buf2;
            }
            else
            {
                place_leaf(buf1, tree, &compare);
                tree->avl = search_inc(buf2, tree->avl, &compare);
            }

            if (tree->hash_size > 0)
                hash_func(leaf, tree, &compare);
            tree->count++;
        }
    }
}

unsigned long place_leaf(struct binary_tree *leaf, struct tree *tree, int *compare)
{
    unsigned long t = tick();
    struct binary_tree *cur = tree->binary;
    int find = 0;
    while (!find)
    {
        if (strcmp(leaf->value, cur->value) < 0)
        {
            if (cur->left == NULL)
            {
                cur->left = leaf;
                find = 1;
            }
            else
                cur = cur->left;
        }
        else
        {
            if (cur->right == NULL)
            {
                cur->right = leaf;
                find = 1;
            }
            else
                cur = cur->right;
        }
        (*compare)++;
    }
    t = tick() - t;
    return t;
}

struct avl_tree *search_inc(struct avl_tree *leaf, struct avl_tree *node, int *compare)
{
    if (node == NULL)
        return(leaf);
    else
    {
        if (strcmp(leaf->value, node->value) < 0)
            node->left = search_inc(leaf, node->left, compare);
        else
            node->right = search_inc(leaf, node->right, compare);
        (*compare)++;
    }
    return balance(node);
}

struct avl_tree *balance(struct avl_tree *node)
{
    fixheight(node);
    if (factor(node) == 2)
    {
        if (factor(node->right) < 0)
            node->right = rotright(node->right);
        return rotleft(node);
    }
    if (factor(node) == -2)
    {
        if (factor(node->left) > 0)
            node->left = rotleft(node->left);
        return rotright(node);
    }
    return node;
}

struct avl_tree *rotright(struct avl_tree *node)
{
    struct avl_tree *buf = node->left;
    node->left = buf->right;
    buf->right = node;
    fixheight(node);
    fixheight(buf);
    return buf;
}

struct avl_tree *rotleft(struct avl_tree *node)
{
    struct avl_tree *buf = node->right;
    node->right = buf->left;
    buf->left = node;
    fixheight(node);
    fixheight(buf);
    return buf;
}

int factor(struct avl_tree *node)
{
    return height(node->right) - height(node->left);
}

void fixheight(struct avl_tree *node)
{
    int left = height(node->left);
    int right = height(node->right);
    node->balance = (left > right ? left : right) + 1;
    //node->balance = (left > right ? left : right);
}

int height(struct avl_tree *node)
{
    return node ? node->balance : 0;
}

void binary_out(struct tree tree)
{
    FILE *f = fopen("binary_tree.gv", "w");
    fprintf(f, "digraph Binary_Tree {\n");
    binary_cycle_out(tree.binary, f);
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng binary_tree.gv -o bin_graph.png");
    system("bin_graph.png");
}

void binary_cycle_out(struct binary_tree *leaf, FILE *f)
{
    if (leaf->left != NULL)
    {
        fprintf(f, "%s -> %s [color=red];\n", leaf->value, leaf->left->value);
        binary_cycle_out(leaf->left, f);
    }
    else
        fprintf(f, "%s;\n", leaf->value);
    if (leaf->right != NULL)
    {
        fprintf(f, "%s -> %s [color=blue];\n", leaf->value, leaf->right->value);
        binary_cycle_out(leaf->right, f);
    }
}

void avl_out(struct tree tree)
{
    FILE *f = fopen("avl_tree.gv", "w");
    fprintf(f, "digraph AVL_Tree {\n");
    avl_cycle_out(tree.avl, f);
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng avl_tree.gv -o avl_graph.png");
    system("avl_graph.png");
}

void avl_cycle_out(struct avl_tree *leaf, FILE *f)
{
    if (leaf->left != NULL)
    {
        fprintf(f, "%s -> %s [color=red];\n", leaf->value, leaf->left->value);
        avl_cycle_out(leaf->left, f);
    }
    else
        fprintf(f, "%s;\n", leaf->value);
    if (leaf->right != NULL)
    {
        fprintf(f, "%s -> %s [color=blue];\n", leaf->value, leaf->right->value);
        avl_cycle_out(leaf->right, f);
    }
}

void hash_out(struct tree tree)
{
    struct hash_slot *buf = NULL;
    for (int i = 0; i < tree.hash_size; i++)
    {
        printf("Hash-value: %d.", i);
        if (tree.hash_table[i] == NULL)
            printf("This cell is empty.\n");
        else
        {
            printf("\n");
            buf = tree.hash_table[i];
            while (buf)
            {
                printf("\"%s\"\n", buf->value);
                buf = buf->next;
            }
        }
    }
}

unsigned long hash_func(char *leaf, struct tree *tree, int *compare)
{
    unsigned long t = tick();
    int sum = 0;
    for (int i = 0; i < (int)strlen(leaf); i++)
    {
        sum += leaf[i]^tree->rand[i];
    }
    int ind = sum % tree->hash_size;
    struct hash_slot *buf = malloc(sizeof(struct hash_slot));
    strcpy(buf->value, leaf);
    if (tree->hash_table[ind] == NULL)
    {
        buf->next = NULL;
        tree->hash_table[ind] = buf;
    }
    else
    {
        buf->next = tree->hash_table[ind];
        tree->hash_table[ind] = buf;
    }
    (*compare)++;
    t = tick() - t;
    return t;
}

void add_leaf(FILE *file, struct tree *tree)
{
    char leaf[MAX_LEN + 2];
    fgets(leaf, 100, stdin);
    printf("Input word: ");
    fgets(leaf, MAX_LEN + 2, stdin);
    if (strlen(leaf) != 0 && leaf[strlen(leaf) - 1] != '\n')
    {
        printf("Too many symbols in element. Rewrite file.\n");
        fgets(leaf, 100, stdin);
        return;
    }
    else
    {
        leaf[strlen(leaf) - 1] = '\0';
        printf("Success!\n");

        unsigned long t1 = tick();
        struct binary_tree *buf1 = malloc(sizeof(struct binary_tree));
        strcpy(buf1->value, leaf);
        buf1->left = buf1->right = NULL;
        int comp1 = 0;

        if (tree->count == 0)
            tree->binary = buf1;
        else
            place_leaf(buf1, tree, &comp1);
        t1 = tick() - t1;


        unsigned long t2 = tick();
        struct avl_tree *buf2 = malloc(sizeof(struct avl_tree));
        strcpy(buf2->value, leaf);
        buf2->balance = 1;
        buf2->left = buf2->right = NULL;
        int comp2 = 0;
        tree->avl = search_inc(buf2, tree->avl, &comp2);
        t2 = tick() - t2;


        int comp3 = 0;
        unsigned long t3 = hash_func(leaf, tree, &comp3);


        unsigned long t4 = tick();
        fprintf(file, "%s\n", leaf);
        t4 = tick() - t4;

        tree->count++;

        printf("Results of word adding:\n\n");
        printf("Binary search tree\nTime: %lu\nComparing: %d\nMemory: %d\n\n", t1, comp1, (int)sizeof(struct binary_tree));
        printf("AVl tree\nTime: %lu\nComparing: %d\nMemory: %d\n\n", t2, comp2, (int)sizeof(struct avl_tree));
        printf("Hash-table\nTime: %lu\nComparing: %d\nMemory: %d\n\n", t3, comp3, (int)sizeof(struct hash_slot));
        printf("File\nTime: %lu\nComparing: %d\nMemory: %d\n\n", t4, 0, 0);
    }
}
