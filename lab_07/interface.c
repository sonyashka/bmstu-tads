#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

void user_hint()
{
    printf("Choose what you want to do:\n"
           "1 - Input graph\n"
           "2 - Choose default (wiki) graph\n"
           "3 - Show graph\n"
           "4 - Find shortest ways\n"
           "0 - Exit\n"
           "Choose what you want: ");
}

void end_hint()
{
    printf("Its end of working the progrma!\n");
}

void wrong_hint()
{
    printf("Wrong input! Try again.\n");
}

void default_graph(struct matrix_graph *graph)
{
    graph->size = 6;
    graph->adjacence_matrix = calloc(graph->size * graph->size, sizeof(int));
    *(graph->adjacence_matrix + 0 * graph->size + 1) = 7;
    *(graph->adjacence_matrix + 0 * graph->size + 2) = 9;
    *(graph->adjacence_matrix + 0 * graph->size + 5) = 14;
    *(graph->adjacence_matrix + 1 * graph->size + 2) = 10;
    *(graph->adjacence_matrix + 1 * graph->size + 3) = 15;
    *(graph->adjacence_matrix + 2 * graph->size + 3) = 11;
    *(graph->adjacence_matrix + 2 * graph->size + 5) = 2;
    *(graph->adjacence_matrix + 3 * graph->size + 4) = 6;
    *(graph->adjacence_matrix + 4 * graph->size + 5) = 9;
    for (int i = 0; i < graph->size; i++)
        for (int j = i; j < graph->size; j++)
            if (*(graph->adjacence_matrix + i * graph->size + j) != 0)
                *(graph->adjacence_matrix + j * graph->size + i) = *(graph->adjacence_matrix + i * graph->size + j);
    printf("Success!\n");
}
