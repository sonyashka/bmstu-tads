#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>
#include "interface.h"

void input_vector(int len, int *vec, int  *vec_non_zero);
void input_matrix(int m, int n, int *matrix, int *matrix_non_zero);
void sparse_vec_init(int vec_len, int *vec, int *V, int *JV);
void sparse_init(int m, int n, int *matrix, int *A, int *IA, int *JA);

void ja_check(int n, int m, int *JA);
void vector_output(int len, int *V, int *JV, int non_zero);
void sparse_output(int len, int *A, int *IA, int *JA, int n);

int find_index(int ind, int non_zero, int *JV);

void generate_vector(int len, int non_zero, int *vec);
void generate_matrix(int m, int n, int non_zero, int *matrix);

void efficiency_classic(int vec_len, int vec_non_zero, int m, int matrix_non_zero);
unsigned long long get_time_of_sparse_mult(int *V, int *JV, int n, int *A, int *IA, int *JA, int *non_zero_res, int *R);
unsigned long long get_time_of_classic_mult(int *vec, int m, int n, int *matrix, int *res);
void multiplication(int *vec, int m, int n, int *matrix, int *res);
unsigned long long tick(void);

void classic_mult()
{
    printf("In this mode input ALL elements!\n");
    int vec_len = 0, rc = 0;
    printf("Input lenght of string-vector: ");
    rc = scanf("%d", &vec_len);
    if (!rc || vec_len < 1)
    {
        wrong_data();
        return;
    }

    int *vec = calloc(vec_len, sizeof(int));
    int vec_non_zero = 0;
    printf("Input integer values (new elem on new string).\n");
    input_vector(vec_len, vec, &vec_non_zero);
    if (vec_non_zero == 0)
    {
        printf("All elements equal zero!\n");
        free(vec);
        return;
    }

    int *V = 0, *JV = 0;
    V = malloc(sizeof(int) * vec_non_zero);
    JV = malloc(sizeof(int) * (vec_len + 1));
    sparse_vec_init(vec_len, vec, V, JV);
    *(JV + vec_len) = vec_non_zero;
    free(vec);
    printf("V: ");
    vector_output(vec_len, V, JV, vec_non_zero);

    int m = 0, n = 0;
    printf("Input size of matrix (in one line): ");
    rc = scanf("%d %d", &m, &n);
    if (rc != 2 || m < 1 || n < 1 || m != vec_len)
    {
        wrong_data();
        free(V);
        free(JV);
        return;
    }

    int *matrix = calloc(m * n, sizeof(int));
    int matrix_non_zero = 0;
    printf("Input integer values (new elem on new string).\n");
    input_matrix(m, n, matrix, &matrix_non_zero);
    if (matrix_non_zero == 0)
    {
        printf("All elements equal zero!\n");
        free(matrix);
        free(V);
        free(JV);
        return;
    }

    int *A = 0, *IA = 0, *JA = 0;
    A = malloc(sizeof(int) * matrix_non_zero);
    IA = malloc(sizeof(int) * matrix_non_zero);
    JA = malloc(sizeof(int) * (n + 1));
    *(JA + n) = matrix_non_zero;
    sparse_init(m, n, matrix, A, IA, JA);
    free(matrix);
    sparse_output(matrix_non_zero, A, IA, JA, n + 1);

    int *res = 0, res_non_zero = 0, *R = 0, *JR = 0;
    res = calloc(n, sizeof(int));
    sparse_multiplication(V, JV, n, A, IA, JA, &res_non_zero, res);
    R = malloc(sizeof(int) * res_non_zero);
    JR = malloc(sizeof(int) * (n + 1));
    sparse_vec_init(n, res, R, JR);
    *(JR + n) = res_non_zero;

    if (res_non_zero != 0)
    {
        printf("RES: ");
        vector_output(n, R, JR, res_non_zero);
    }
    else
        printf("All elements equal zero!\n");
    free(res);
    free(R);
    free(JR);
    free(V);
    free(JV);
    free(A);
    free(IA);
    free(JA);
}

void sparse_multiplication(int *V, int *JV, int n, int *A, int *IA, int *JA, int *non_zero_res, int *res)
{
    int buf = 0, ind = 0;
    for (int j = 0; j < n; j++)
        for (int i = *(JA + j); i < *(JA + j + 1); i++)
        {
            buf = *(IA + i);
            ind = *(JV + buf);
            if (*(JV + buf + 1) - ind == 1)
                *(res + j) += *(A + i) * *(V + ind);
        }

    for (int i = 0; i < n; i++)
        if (*(res + i) != 0)
            (*non_zero_res)++;
}

int find_index(int ind, int non_zero, int *JV)
{
    int res = -1;
    for (int i = 0; i < non_zero && res == -1; i++)
    {
        if (*(JV + i) == ind)
            res = i;
    }
    return res;
}

void vector_output(int len, int *V, int *JV, int non_zero)
{
    for (int i = 0; i < non_zero; i++)
    {
        printf("%d ", *(V + i));
    }
    printf("\nJ: ");
    for (int i = 0; i < len; i++)
    {
        if (*(JV + i + 1) - *(JV + i) == 1)
            printf("%d ", i);
    }
    printf("\n");
}

void sparse_output(int len, int *A, int *IA, int *JA, int n)
{
    printf("Matrix in sparse form\n"
           "A: ");
    for (int i = 0; i < len; i++)
        printf("%d ", *(A + i));
    printf("\nIA: ");
    for (int i = 0; i < len; i++)
        printf("%d ", *(IA + i));
    printf("\nJA: ");
    for (int i = 0; i < n; i++)
        printf("%d ", *(JA + i));
    printf("\n");
}

void random_mult()
{
    int vec_len = 0, rc = 0;
    printf("Input lenght of string-vector: ");
    rc = scanf("%d", &vec_len);
    if (rc != 1 || vec_len < 1)
    {
        wrong_data();
        return;
    }

    int vec_non_zero = 0;
    printf("Input integer percentage of zero elems(num without percente): ");
    rc = scanf("%d", &vec_non_zero);
    if (rc != 1 || vec_non_zero < 0 || vec_non_zero > 100)
    {
        wrong_data();
        return;
    }
    vec_non_zero = (int)((float)(100 - vec_non_zero) / 100 * vec_len);

    int *vec = 0;
    vec = calloc(vec_len, sizeof(int));
    generate_vector(vec_len, vec_non_zero, vec);
    printf("Generated vector: \n");
    for (int i = 0; i < vec_len; i++)
        printf("%d ", *(vec + i));
    printf("\n");

    int *V = 0, *JV = 0;
    V = malloc(sizeof(int) * vec_non_zero);
    JV = malloc(sizeof(int) * (vec_len + 1));
    sparse_vec_init(vec_len, vec, V, JV);
    *(JV + vec_len) = vec_non_zero;
    free(vec);
    printf("V: ");
    vector_output(vec_len, V, JV, vec_non_zero);

    int m = 0, n = 0;
    printf("Input size of matrix (in one line): ");
    rc = scanf("%d %d", &m, &n);
    if (rc != 2 || m < 1 || n < 1 || m != vec_len)
    {
        wrong_data();
        free(V);
        free(JV);
        return;
    }

    int matrix_non_zero = 0;
    printf("Input integer percentage of zero elements(num without percente): ");
    rc = scanf("%d", &matrix_non_zero);
    if (!rc || matrix_non_zero < 0 || matrix_non_zero > 100)
    {
        wrong_data();
        free(V);
        free(JV);
        return;
    }
    matrix_non_zero = (int)((float)(100 - matrix_non_zero) / 100 * m * n);

    int *matrix = calloc(m * n, sizeof(int));
    generate_matrix(m, n, matrix_non_zero, matrix);
    printf("Generated matrix: \n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", *(matrix + i * n + j));
        printf("\n");
    }

    int *A = 0, *IA = 0, *JA = 0;
    A = malloc(sizeof(int) * matrix_non_zero);
    IA = malloc(sizeof(int) * matrix_non_zero);
    JA = malloc(sizeof(int) * (n + 1));
    sparse_init(m, n, matrix, A, IA, JA);
    *(JA + n) = matrix_non_zero;
    free(matrix);
    sparse_output(matrix_non_zero, A, IA, JA, n + 1);

    int res_non_zero = 0, *R = 0, *JR = 0;
    int *res = calloc(n, sizeof(int));
    sparse_multiplication(V, JV, n, A, IA, JA, &res_non_zero, res);
    R = malloc(sizeof(int) * res_non_zero);
    JR = malloc(sizeof(int) * (n + 1));
    sparse_vec_init(n, res, R, JR);
    *(JR + n) = res_non_zero;
    free(res);
    free(V);
    free(JV);
    free(A);
    free(IA);
    free(JA);

    if (res_non_zero != 0)
    {
        printf("RES: ");
        vector_output(n, R, JR, res_non_zero);
    }
    else
        printf("All elements equal zero!\n");
    free(R);
    free(JR);
}

void input_vector(int len, int *vec, int *vec_non_zero)
{
    printf("Do you wanna input all vector (1) or by coordinates (2) - input 1 or 2 for choose: ");
    int buf = 0, rc = 0;
    rc = scanf("%d", &buf);
    if (rc && buf == 1)
    {
        printf("Input value: ");
        for (int i = 0; i < len; i++)
            if (scanf("%d", &buf) != 1)
            {
                wrong_data();
                printf("Try again!\n");
                i--;
            }
            else
            {
                *(vec + i) = buf;
                if (buf != 0)
                    (*vec_non_zero)++;
            }
    }
    else if (rc && buf == 2)
    {
        rc = 0;
        int j = 0;
        printf("Input count of non-zero elements: ");
        while (!rc)
        {
            rc = scanf("%d", vec_non_zero);
            if (!rc)
                printf("Try again!\n");
        }
        printf("Input elements by 'column-value'(in one line)\n");
        for (int i = 0; i < *vec_non_zero; i++)
            if (scanf("%d %d", &j, &buf) != 2 || j < 0 || j > len)
            {
                wrong_data();
                printf("Try again!\n");
                i--;
            }
            else
                *(vec + j) = buf;
    }
    else
    {
        printf("Try again!\n");
        input_vector(len, vec, vec_non_zero);
    }
}

void input_matrix(int m, int n, int *matrix, int *matrix_non_zero)
{
    printf("Do you wanna input all matrix (1) or by coordinates (2) - input 1 or 2 for choose: ");
    int buf = 0, rc = 0;
    rc = scanf("%d", &buf);
    if (rc && buf == 1)
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (scanf("%d", &buf) != 1)
                {
                    wrong_data();
                    printf("Try again!\n");
                    j--;
                }
                else
                {
                    *(matrix + i * n + j) = buf;
                    if (buf != 0)
                        (*matrix_non_zero)++;
                }
    else if (rc && buf == 2)
    {
        rc = 0;
        int i = 0, j = 0;
        printf("Input count of non-zero elements: ");
        while (!rc)
        {
            rc = scanf("%d", matrix_non_zero);
            if (!rc)
                printf("Try again!\n");
        }
        printf("Input elements by 'row-column-value'(in one line)\n");
        for (int l = 0; l < *matrix_non_zero; l++)
            if (scanf("%d %d %d", &i, &j, &buf) != 3 || i < 0 || j < 0 || i > m - 1 || j > n - 1)
            {
                 wrong_data();
                 printf("Try again!\n");
                 j--;
            }
            else
                 *(matrix + i * n + j) = buf;
    }
    else
    {
        printf("Try again!\n");
        input_matrix(m, n, matrix, matrix_non_zero);
    }
}

void sparse_vec_init(int vec_len, int *vec, int *V, int *JV)
{
    int k = 0;
    for (int i = 0; i < vec_len; i++)
    {
        *(JV + i) = k;
        if (*(vec + i) != 0)
        {
            *(V + k) = *(vec + i);
            k++;
        }
    }
}

void sparse_init(int m, int n, int *matrix, int *A, int *IA, int *JA)
{
    int k = 0;
    for (int j = 0; j < n; j++)
    {
        *(JA + j) = k;
        for (int i = 0; i < m; i++)
        {
            if (*(matrix + i * n + j) != 0)
            {
                *(A + k) = *(matrix + i * n + j);
                *(IA + k) = i;
                k++;
            }
        }
    }
}

void file_mult()
{
    FILE *f;
    f = fopen("input.txt", "r");
    int vec_len = 0, vec_non_zero = 0;
    fscanf(f, "%d", &vec_len);
    int *vec = malloc(sizeof(int) * vec_len);
    for (int i = 0; i < vec_len; i++)
    {
        fscanf(f, "%d", vec + i);
        if (*(vec + i) != 0)
            vec_non_zero++;
    }
    if (vec_non_zero == 0)
    {
        wrong_data();
        free(vec);
        return;
    }

    int *V = 0, *JV = 0;
    V = malloc(sizeof(int) * vec_non_zero);
    JV = malloc(sizeof(int) * (vec_len + 1));
    sparse_vec_init(vec_len, vec, V, JV);
    *(JV + vec_len) = vec_non_zero;
    free(vec);
    printf("V: ");
    vector_output(vec_len, V, JV, vec_non_zero);

    int m = 0, n = 0, matrix_non_zero = 0;
    fscanf(f, "%d %d", &m, &n);
    int *matrix = calloc(m * n, sizeof(int));
    for (int i = 0; i < m * n; i++)
    {
        fscanf(f, "%d", matrix + i);
        if (*(matrix + i) != 0)
            matrix_non_zero++;
    }
    if (matrix_non_zero == 0)
    {
        wrong_data();
        free(V);
        free(JV);
        free(matrix);
        return;
    }

    int *A = 0, *IA = 0, *JA = 0;
    A = malloc(sizeof(int) * matrix_non_zero);
    IA = malloc(sizeof(int) * matrix_non_zero);
    JA = malloc(sizeof(int) * (n + 1));
    sparse_init(m, n, matrix, A, IA, JA);
    free(matrix);
    *(JA + n) = matrix_non_zero;
    sparse_output(matrix_non_zero, A, IA, JA, n + 1);

    int res_non_zero = 0, *R = 0, *JR = 0;
    int *res = calloc(n, sizeof(int));
    sparse_multiplication(V, JV, n, A, IA, JA, &res_non_zero, res);
    free(A);
    free(IA);
    free(JA);
    free(V);
    free(JV);
    if (res_non_zero == 0)
    {
        printf("All elements are zero!\n");
        free(res);
        return;
    }
    R = malloc(sizeof(int) * res_non_zero);
    JR = malloc(sizeof(int) * (n + 1));
    sparse_vec_init(n, res, R, JR);
    *(JR + n) = res_non_zero;
    free(res);

    if (res_non_zero != 0)
    {
        printf("RES: ");
        vector_output(n, R, JR, res_non_zero);
    }
    else
        printf("All elements equal zero!\n");
    free(R);
    free(JR);
}

void generate_vector(int len, int non_zero, int *vec)
{
    int j = 0;
    for (int i = 0; i < non_zero; i++)
    {
        j = rand() % len;
        if (*(vec + j) == 0)
        {
            *(vec + j) = rand() % 10;
            while(*(vec + j) == 0)
                *(vec + j) = rand() % 10;
        }
        else
            i--;
    }
}

void generate_matrix(int m, int n, int non_zero, int *matrix)
{
    int i = 0, j = 0;
    for (int k = 0; k < non_zero; k++)
    {
        i = rand() % m;
        j = rand() % n;
        if (*(matrix + i * n + j) == 0)
        {
            *(matrix + i * n + j) = rand() % 10;
            while(*(matrix + i * n + j) == 0)
                *(matrix + i * n + j) = rand() % 10;
        }
        else
            k--;
    }
}

void efficiency_table()
{
    printf("Table of multiplication efficiency\n");
    printf("Size of matrix, %% non-zero|Classis time|Sparse time|||Classic mem|Sparse mem\n");
    long_out('-', 76);
    printf("\n      10x10       10%%     |");
    efficiency_classic(10, 1, 10, 10);
    printf("\n      10x10       25%%     |");
    efficiency_classic(10, 3, 10, 25);
    printf("\n      10x10       40%%     |");
    efficiency_classic(10, 4, 10, 40);
    printf("\n      10x10       50%%     |");
    efficiency_classic(10, 5, 10, 50);
    printf("\n      10x10       75%%     |");
    efficiency_classic(10, 8, 10, 75);
    printf("\n      10x10       90%%     |");
    efficiency_classic(10, 9, 10, 90);
    printf("\n      50x50       10%%     |");
    efficiency_classic(50, 5, 50, 250);
    printf("\n      50x50       25%%     |");
    efficiency_classic(50, 13, 50, 625);
    printf("\n      50x50       40%%     |");
    efficiency_classic(50, 20, 50, 1000);
    printf("\n      50x50       50%%     |");
    efficiency_classic(50, 25, 50, 1250);
    printf("\n      50x50       75%%     |");
    efficiency_classic(50, 38, 50, 1875);
    printf("\n      50x50       90%%     |");
    efficiency_classic(50, 45, 50, 2250);
    printf("\n     100x100      10%%     |");
    efficiency_classic(100, 10, 100, 1000);
    printf("\n     100x100      25%%     |");
    efficiency_classic(100, 25, 100, 2500);
    printf("\n     100x100      40%%     |");
    efficiency_classic(100, 40, 100, 4000);
    printf("\n     100x100      50%%     |");
    efficiency_classic(100, 50, 100, 5000);
    printf("\n     100x100      75%%     |");
    efficiency_classic(100, 75, 100, 7500);
    printf("\n     100x100      90%%     |");
    efficiency_classic(100, 90, 100, 9000);
    printf("\n     200x200      10%%     |");
    efficiency_classic(200, 20, 500, 2000);
    printf("\n     200x200      25%%     |");
    efficiency_classic(200, 50, 200, 5000);
    printf("\n     200x200      40%%     |");
    efficiency_classic(200, 80, 200, 8000);
    printf("\n     200x200      50%%     |");
    efficiency_classic(200, 100, 200, 10000);
    printf("\n     200x200      75%%     |");
    efficiency_classic(200, 150, 200, 15000);
    printf("\n     200x200      90%%     |");
    efficiency_classic(200, 180, 200, 18000);
    printf("\n");
}

void efficiency_classic(int vec_len, int vec_non_zero, int m, int matrix_non_zero)
{
    int *vec = 0, *matrix = 0;
    vec = calloc(vec_len, sizeof(int));
    matrix = calloc(m * m, sizeof(int));
    generate_vector(vec_len, vec_non_zero, vec);
    generate_matrix(m, m, matrix_non_zero, matrix);

    int *V = 0, *JV = 0, *A = 0, *IA = 0, *JA = 0;
    V = malloc(sizeof(int) * vec_non_zero);
    JV = malloc(sizeof(int) * (vec_len + 1));
    sparse_vec_init(vec_len, vec, V, JV);
    *(JV + vec_len) = vec_non_zero;
    free(vec);

    A = malloc(sizeof(int) * matrix_non_zero);
    IA = malloc(sizeof(int) * matrix_non_zero);
    JA = malloc(sizeof(int) * (m + 1));
    sparse_init(m, m, matrix, A, IA, JA);
    free(matrix);

    int res_non_zero = 0, *res = 0;
    res = calloc(m, sizeof(int));
    long int sum1 = 0, sum2 = 0;
    for (int i = 0; i < 20; i++)
    {
        res_non_zero = 0;
        sum2 += get_time_of_classic_mult(vec, m, m, matrix, res);
        sum1 += get_time_of_sparse_mult(V, JV, m, A, IA, JA, &res_non_zero, res);
    }
    double sr1 = sum1 / 20;
    double sr2 = sum2 / 20;
    printf("%12ld|%11ld|||%11d|%10d", (long)sr2, (long)sr1, 2 * m + m * m, vec_non_zero * 2 + m + 1 + matrix_non_zero * 2 + res_non_zero * 2);
    free(V);
    free(JV);
    free(A);
    free(JA);
    free(IA);
    free(res);
}

unsigned long long get_time_of_sparse_mult(int *V, int *JV, int n, int *A, int *IA, int *JA, int *non_zero_res, int *R)
{
    unsigned long long t = tick();
    sparse_multiplication(V, JV, n, A, IA, JA, non_zero_res, R);
    t = tick() - t;
    return t;
}

unsigned long long get_time_of_classic_mult(int *vec, int m, int n, int *matrix, int *res)
{
    unsigned long long t = tick();
    multiplication(vec, m, n, matrix, res);
    t = tick() - t;
    return t;
}

void multiplication(int *vec, int m, int n, int *matrix, int *res)
{
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < m; i++)
        {
            *(res + j) += *(vec + j) * *(matrix + i * m + j);
        }
    }
}

void long_out(char a, int count)
{
    for (int i = 0; i < count; i++)
        printf("%c", a);
}

void user_hint()
{
    printf("Choose what you wanna do:\n"
           "1 - Multiplication sparse matrix on vector(console input)\n"
           "2 - Multiplicate randome matrix on vector(with input percentage of zero elements)\n"
           "3 - Multiplicate spare matrix on vector(file input)\n"
           "4 - Efficiency results\n"
           "0 - Exit\n"
           "Choose what you want: ");
}

unsigned long long tick(void)
{
unsigned long long d;
__asm__ __volatile__ ("rdtsc": "=A" (d));
return d;
}

void wrong_data()
{
    printf("Wrong values!\n");
}

void end_hint()
{
    printf("Its end of working the porogram!\n");
}
