#include <stdio.h>
#include <stdlib.h>
#include <graphviz\gvc.h>
#include "interface.h"
#include "in_out.h"

void long_out(int len, char c);

void input_graph(struct matrix_graph *graph)
{
    if (graph->adjacence_matrix != NULL)
        free(graph->adjacence_matrix);

    printf("Input count of vertex: ");
    int rc = scanf("%d", &graph->size);
    if (!rc || graph->size <= 0)
    {
        printf("Wrong size!\n");
        return;
    }

    graph->adjacence_matrix = calloc(graph->size * graph->size, sizeof(int));
    int v1 = 1, v2 = 1, weigth = 1;
    printf("Input ways in format 'v1 v2 weigth'. Weigth must be more than 0.\n"
           "For end of input '0 0 0'.\n");
    while (v1 != 0 || v2 != 0 || weigth != 0)
    {
        rc = scanf("%d %d %d", &v1, &v2, &weigth);
        if (rc == 3 && v1 <= graph->size && v1 > 0 && v2 > 0 && v2 <= graph->size && weigth > 0)
        {
            v1--;
            v2--;
            *(graph->adjacence_matrix + v1 * graph->size + v2) = weigth;
            *(graph->adjacence_matrix + v2 * graph->size + v1) = weigth;
        }
        else if ((rc != 3 || v1 <= 0 || v2 <= 0 || v1 > graph->size || v2 > graph->size || weigth <= 0) && !(rc == 3 && v1 == 0 && v2 == 0 && weigth == 0))
            wrong_hint();
    }
    printf("End of input.\n");
}

void output_graph(struct matrix_graph graph)
{
    FILE *f = fopen("out_graph.gv", "w");

    fprintf(f, "graph Out_Graph {\n");
    int count;
    for (int i = 0; i < graph.size; i++)
    {
        count = 0;
        for (int j = i; j < graph.size; j++)
        {
            if (i != j && *(graph.adjacence_matrix + i * graph.size + j) != 0)
            {
                count++;
                fprintf(f, "%d -- %d [label=\"%d\"];\n", i + 1, j + 1, *(graph.adjacence_matrix + i * graph.size + j));
            }
        }
        if (count == 0)
            fprintf(f, "%d;\n", i + 1);
    }
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng out_graph.gv -o graph.png");
    system("graph.png");
}

void out_res(struct matrix_graph *res)
{
    printf("Min ways from any to any vertex\n        ");
    for (int  i = 0; i < res->size; i++)
        printf("|%4d   ", i + 1);
    printf("\n");
    long_out((res->size + 1) * 8, '-');
    printf("\n");
    for (int i = 0; i < res->size; i++)
    {
        printf("%4d    ", i + 1);
        for (int j = 0; j < res->size; j++)
        {
            printf("|%4d   ", *(res->adjacence_matrix + i * res->size + j));
        }
        printf("\n");
    }
}

void long_out(int len, char c)
{
    for (int i = 0; i < len ; i++)
        printf("%c", c);
}
