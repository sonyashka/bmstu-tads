#include <stdio.h>
#include <string.h>
#include "file_func.h"
#include "interface.h"

int read_struct(FILE *f, struct student *array)
{
    int count = 0;
    while (!feof(f))
    {
        fgets(array[count].surname, MAX_LEN, f);
        array[count].surname[strlen(array[count].surname) - 1] = '\0';
        fgets(array[count].name, MAX_LEN, f);
        array[count].name[strlen(array[count].name) - 1] = '\0';
        fgets(array[count].group, MAX_LEN_GROUP, f);
        array[count].group[strlen(array[count].group) - 1] = '\0';
        fscanf(f, "%c\n", &array[count].gender[0]);
        fscanf(f, "%d\n", &array[count].age);
        fscanf(f, "%f\n", &array[count].average_score);
        fgets(array[count].date, MAX_LEN, f);
        array[count].date[strlen(array[count].date) - 1] = '\0';
        fgets(array[count].housing, MAX_LEN, f);
        array[count].housing[strlen(array[count].housing) - 1] = '\0';
        if (strcmp(array[count].housing, "hostel") == 0)
        {
            fscanf(f, "%d\n", &array[count].adress.sweet_home.hostel_number);
            fscanf(f, "%d\n", &array[count].adress.sweet_home.room_number);
        }
        else
        {
            fgets(array[count].adress.home.street, MAX_LEN, f);
            array[count].adress.home.street[strlen(array[count].adress.home.street) - 1] = '\0';
            fscanf(f, "%d\n", &array[count].adress.home.house_number);
            fscanf(f, "%d\n", &array[count].adress.home.apart_number);
        }
        count++;
    }
    return count;
}
