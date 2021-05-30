#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "in_out.h"
#include "algoritm.h"

void matrix_init(struct matrix_graph *matrix);
void find_cur_ver(int i, int k, struct matrix_graph *res, struct matrix_graph matrix, int *way);

void find_min(struct matrix_graph matrix_graph)
{
    struct matrix_graph *res = find_matrix(matrix_graph);
    out_res(res);
    free(res->adjacence_matrix);
    free(res);
}

struct matrix_graph *find_matrix(struct matrix_graph matrix_graph)
{
    struct matrix_graph *res = malloc(sizeof(struct matrix_graph));
    res->size = matrix_graph.size;
    res->adjacence_matrix = malloc(sizeof(struct matrix_graph) * matrix_graph.size * matrix_graph.size);
    matrix_init(res);
    int way[res->size];
    for (int i = 0; i < res->size; i++)
    {
        for (int j = 0; j < res->size; j++)
            way[j] = 0;
        find_cur_ver(i, i, res, matrix_graph, way);
        //printf("ok\n");
        for (int j = 0; j < res->size; j++)
            if (*(res->adjacence_matrix + i * res->size + j) == INT_MAX)
                *(res->adjacence_matrix + i * res->size + j) = 0;
    }
    return res;
}

void matrix_init(struct matrix_graph *matrix)
{
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++)
            if (i == j)
                *(matrix->adjacence_matrix + i * matrix->size + j) = 0;
            else
                *(matrix->adjacence_matrix + i * matrix->size + j) = INT_MAX;
}

void find_cur_ver(int i, int k, struct matrix_graph *res, struct matrix_graph matrix, int *way)
{
    //printf("*\n");
    int cur = 0, min_ind = -1, min = INT_MAX;
    for (int j = 0; j < res->size; j++)
    {
        cur = *(matrix.adjacence_matrix + k * res->size + j);
        /*if (cur != 0)
            printf("k %d j %d new %d prev %d\n",k, j, *(res->adjacence_matrix + i * res->size + k) + cur, *(res->adjacence_matrix + i * res->size + j));*/
        if (cur != 0 && way[min_ind] == 0 && *(res->adjacence_matrix + i * res->size + k) + cur < *(res->adjacence_matrix + i * res->size + j))
        {
            if (*(res->adjacence_matrix + i * res->size + k) + cur < min)
            {
                min = *(res->adjacence_matrix + i * res->size + k) + cur;
                min_ind = j;
            }
            *(res->adjacence_matrix + i * res->size + j) = *(res->adjacence_matrix + i * res->size + k) + cur;
        }
        else if (cur != 0 && way[min_ind] == 0 && way[j] == 0)
        {
            min = *(res->adjacence_matrix + i * res->size + j);
            min_ind = j;
        }
    }
    if (min_ind != -1)
    {
        way[min_ind] = 1;
        find_cur_ver(i, min_ind, res, matrix, way);
    }
}
