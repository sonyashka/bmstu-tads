#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "in_out.h"

int main(void)
{
    printf("Welcome to the program!\n");
    int rc = 1, choice = 1, f = 0;
    struct tree tree;
    tree.count = 0;
    tree.avl = NULL;
    tree.binary = NULL;
    tree.hash_size = 0;
    tree.hash_table = NULL;
    FILE *file = fopen("tree.txt", "a+");
    while (rc && choice > 0 && choice < 6)
    {
        user_hint();
        rc = scanf("%d", &choice);
        if (rc)
        {
            switch (choice)
            {
                case 1:
                    if (!f)
                    {
                        read_tree(file, &tree);
                        if (tree.hash_size != 0)
                            f = 1;
                        printf("Success!\n");
                    }
                    else
                        printf("File has already been read!\n");
                    break;
                case 2:
                    add_leaf(file, &tree);
                    break;
                case 3:
                    if (f)
                        binary_out(tree);
                    else
                        printf("Read file firstly.\n");
                    break;
                case 4:
                    if (f)
                        avl_out(tree);
                    else
                        printf("Read file firstly.\n");
                    break;
                case 5:
                    if (tree.hash_size > 0)
                    {
                        hash_out(tree);
                    }
                    else if (f)
                        printf("Uncorrect size of hash-table.\n");
                    else
                        printf("File not opened.\n");
                    break;
            }
        }
    }
    fclose(file);
    end_hint();
    return 0;
}
