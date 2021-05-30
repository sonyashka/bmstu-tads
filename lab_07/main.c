#include <stdio.h>
#include <stdlib.h>
#include <graphviz\gvc.h>
#include "interface.h"
#include "in_out.h"
#include "algoritm.h"

int main(void)
{
    int rc = 1, choice = 1;
    printf("Welcome to the program!\n");
    struct matrix_graph matrix_graph;
    matrix_graph.size = 0;
    matrix_graph.adjacence_matrix = NULL;
    while (rc && choice > 0 && choice < 5)
    {
        user_hint();
        rc = scanf("%d", &choice);
        if (rc)
            switch (choice)
            {
                case 1:
                    input_graph(&matrix_graph);
                    break;
                case 2:
                    default_graph(&matrix_graph);
                    break;
                case 3:
                    output_graph(matrix_graph);
                    break;
                case 4:
                    find_min(matrix_graph);
                    break;
            }
    }
    end_hint();
    return 0;
}
