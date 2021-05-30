#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include "interface.h"

void delete_rec(int ind, int count, struct student *array);

void key_shift(int i, int j, struct key *keys);
void output_key_hint();

void arr_shift(int i, int j, struct student *arr);

unsigned long long time_of_table_sorting(int count, struct student *array);
unsigned long long time_of_key_sorting(int count, struct key *keys);

unsigned long long time_of_qsorting_key(int count, struct key *a);
unsigned long long time_of_qsorting_table(int count, struct student *a);

unsigned long long tick(void);

void app_hint()
{
    printf("Choose what you want to do with data:\n"
           "1 - Output data\n"
           "2 - Output student, living in hostel, certain year of enrollment\n"
           "3 - Add data in the end of list\n"
           "4 - Delete one record by field 'Group'\n"
           "5 - Sorted key by field 'Surname' without sorted data\n"
           "6 - Output sorted data by field 'surname' (by keys)\n"
           "7 - Output sorted data by table\n"
           "8 - Output results of efficiency sorting using table and keys\n"
           "9 - Output results of using any types of sorting\n"
           "0 - Exit\n"
           "Choose your variant: ");
}

void output_data_all(int count, struct student *array)
{
    out_hint();
    for (int i = 0; i < count; i++)
    {
        printf("%d", i + 1);
        if (i < 9)
            printf(" ");
        printf(" | %s", array[i].surname);
        long_out(' ', MAX_LEN - strlen(array[i].surname));
        printf("| %s", array[i].name);
        long_out(' ', MAX_LEN - strlen(array[i].name));
        printf("| %s", array[i].group);
        long_out(' ', MAX_LEN_GROUP - strlen(array[i].group));
        printf("|  %c   |", array[i].gender[0]);
        printf("%d | %.2f | ", array[i].age, array[i].average_score);
        printf("%s", array[i].date);
        long_out(' ', 7);
        printf("| ");
        if (strcmp(array[i].housing, "hostel") == 0)
        {
            printf("Hostel num %d, room num %d", array[i].adress.sweet_home.hostel_number,
                   array[i].adress.sweet_home.room_number);
        }
        else
        {
            printf("Str. %s, h. %d, ap. %d", array[i].adress.home.street, array[i].adress.home.house_number,
                   array[i].adress.home.apart_number);
        }
        printf("\n");
    }
}

void output_data_certain(int count, struct student *array)
{
    printf("Input certain year of enrollment (1900 - 2020): ");
    int year;
    int rc = scanf("%d", &year);
    if (!rc)
        printf("Incorrect date!\n");
    else if (year < 1900 || year > 2020)
        printf("It can't be!\n");
    else
    {
        char year_str[5];
        itoa(year, year_str, 10);
        out_hint();
        int j = 1;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(array[i].housing, "hostel") == 0 && strstr(array[i].date, year_str) != NULL)
            {
                printf("%d  | ", j);
                printf("%s", array[i].surname);
                long_out(' ', MAX_LEN - strlen(array[i].surname));
                printf("| %s", array[i].name);
                long_out(' ', MAX_LEN - strlen(array[i].name));
                printf("| %s", array[i].group);
                long_out(' ', MAX_LEN_GROUP - strlen(array[i].group));
                printf("|  %c   |", array[i].gender[0]);
                printf("%d | %.2f | ", array[i].age, array[i].average_score);
                printf("%s", array[i].date);
                long_out(' ', 7);
                printf("| ");
                printf("Hostel num %d, room num %d", array[i].adress.sweet_home.hostel_number,
                           array[i].adress.sweet_home.room_number);
                printf("\n");
                j++;
            }
        }
    }
}

void out_hint()
{
    long_out('-', 140);
    printf("\nRec|");
    long_out(' ', 8);
    printf("Surname");
    long_out(' ', 7);
    printf("|");
    long_out(' ', 9);
    printf("Name");
    long_out(' ', 9);
    printf("|");
    printf("    Group   |Gender|Age| Score |Date of enrollment|");
    long_out(' ', 14);
    printf("Adress");
    long_out(' ', 14);
    printf("\n");
    long_out('-', 140);
    printf("\n");
}

void long_out(char a,int count)
{
    for (int i = 0; i < count; i++)
        printf("%c", a);
}

int get_data(int max_len, char *str)
{
   int res = 0, i = 0;
   char c;
   while ((c = getc(stdin)) != '\n')
   {
        if (i < max_len - 1)
        {
            str[i] = c;
            i++;
        }
        else
            res = 1;
   }
   str[i] = '\0';
   return res;
}

void add_data(int *count, struct student *array, FILE *f)
{
    int rc = 0;
    printf("Input surname of student (no more 20 symbols): ");
    char surname[MAX_LEN];
    fgets(surname, MAX_LEN, stdin);
    rc = get_data(MAX_LEN, surname);
    if (strlen(surname) == 0)
    {
        printf("Empty surname!\n");
        return;
    }
    else if (rc)
    {
        printf("So long surname!\n");
        return;
    }
    printf("Input name of student (no more 20 symbols): ");
    char name[MAX_LEN];
    rc = get_data(MAX_LEN, name);
    if (strlen(name) == 0)
    {
        printf("Empty name!\n");
        return;
    }
    else if (rc)
    {
        printf("So long name!\n");
        return;
    }
    printf("Input group of student (no more 10 symbols): ");
    char group[MAX_LEN_GROUP];
    rc = get_data(MAX_LEN_GROUP, group);
    if (rc)
    {
        printf("So long group!\n");
        return;
    }
    else if (strlen(group) == 0)
    {
        printf("Empty group!\n");
        return;
    }
    printf("Input gender of student ('F' or 'M', like female/male): ");
    char gender[12];
    rc = get_data(2, gender);
    if (rc || (strcmp(gender, "F") != 0 && strcmp(gender, "M") != 0))
    {
        printf("Wrong gender!\n");
        return;
    }
    printf("Input age of student: ");
    int age;
    rc = scanf("%d", &age);
    if (!rc || age < 16 || age > 40)
    {
        printf("Wrong age!\n");
        return;
    }
    printf("Input average score of student (precision 2 decimal places desirable): ");
    float score;
    rc = scanf("%f", &score);
    if (!rc || score < 3 || score > 5)
    {
        printf("Wrong average score!\n");
        return;
    }
    printf("Input date of enrollment of student (int format DD/MM/YYYY): ");
    char date[MAX_DATE_LEN];
    fgets(date, MAX_LEN, stdin);
    rc = get_data(MAX_DATE_LEN, date);
    if (rc || strlen(date) == 0 || date_parser(date))
    {
        printf("Wrong date!\n");
        return;
    }
    printf("Which type of adress you choose: house or hostel? "
           "(write 'house' or 'hostel' to choose) ");
    char housing[MAX_LEN];
    rc = get_data(MAX_LEN, housing);
    if (!rc && strcmp(housing, "house") == 0)
    {
        printf("Input street of living (no more 20 symbols): ");
        char street[MAX_LEN];
        rc = get_data(MAX_LEN, street);
        if (rc || strlen(street) == 0)
        {
            printf("Wrong street!\n");
            return;
        }
        printf("Input number of house living: ");
        int house_number;
        rc = scanf("%d", &house_number);
        if (!rc || house_number < 1)
        {
            printf("Wrong house number!");
            return;
        }
        printf("Input number of apartment living: ");
        int apart_number;
        rc = scanf("%d", &apart_number);
        if (!rc || apart_number < 1)
        {
            printf("Wrong apartment number!");
            return;
        }
        for (int i = 0; i < (int)strlen(street); i++)
            array[*count].adress.home.street[i] = street[i];
        array[*count].adress.home.house_number = house_number;
        array[*count].adress.home.apart_number = apart_number;
    }
    else if (!rc && strcmp(housing, "hostel") == 0)
    {
        printf("Input hostel number: ");
        int hostel_number;
        rc = scanf("%d", &hostel_number);
        if (!rc || hostel_number < 1 || hostel_number > 20)
        {
            printf("Wrong hostel number!\n");
            return;
        }
        printf("Input room number: ");
        int room_number;
        rc = scanf("%d", &room_number);
        if (!rc || room_number < 1)
        {
            printf("Wrong room number!\n");
            return;
        }
        array[*count].adress.sweet_home.room_number = room_number;
        array[*count].adress.sweet_home.hostel_number = hostel_number;
    }
    else
    {
        printf("Wrong housing type!\n");
        return;
    }
    for (int i = 0; i < (int)strlen(surname); i++)
        array[*count].surname[i] = surname[i];
    for (int i = 0; i < (int)strlen(name); i++)
        array[*count].name[i] = name[i];
    for (int i = 0; i < (int)strlen(group); i++)
        array[*count].group[i] = group[i];
    for (int i = 0; i < (int)strlen(gender); i++)
        array[*count].gender[i] = gender[i];
    array[*count].age = age;
    array[*count].average_score = score;
    for (int i = 0; i < (int)strlen(date); i++)
        array[*count].date[i] = date[i];
    for (int i = 0; i < (int)strlen(housing); i++)
        array[*count].housing[i] = housing[i];
    fseek(f, 0, SEEK_END);
    fprintf(f, "%s\n%s\n%s\n%s\n%d\n%.2f\n%s\n%s\n", surname, name, group, gender, age, score, date, housing);
    if (strcmp(housing, "house") == 0)
        fprintf(f, "%s\n%d\n%d\n", array[*count].adress.home.street, array[*count].adress.home.house_number, array[*count].adress.home.apart_number);
    else
        fprintf(f, "%d\n%d\n", array[*count].adress.sweet_home.hostel_number, array[*count].adress.sweet_home.room_number);
    (*count)++;
    printf("Added successfully!\n");
}

int date_parser(char *date)
{
    int res = 0;
    char day_c[3], month_c[3], year_c[5];
    for (int i = 0; i < 2; i++)
        day_c[i] = date[i];
    day_c[2] = '\0';
    for (int i = 0; i < 2; i++)
        month_c[i] = date[i + 3];
    month_c[2] = '\0';
    for (int i = 0; i < 4; i++)
        year_c[i] = date[i + 6];
    year_c[4] = '\0';
    int day = atoi(day_c), month = atoi(month_c), year = atoi(year_c);
    if (day > 31 || day <= 0 || month <= 0 || month > 12 || year < 1900 || year > 2020)
        res = 1;
    return res;
}

void delete_by_field(int *count, struct student *array)
{
    printf("You can delete ONE record by field 'Group'.\n"
           "Input value of field (no more 10 symbols): ");
    char group[MAX_LEN_GROUP];
    fgets(group, MAX_LEN_GROUP, stdin);
    int rc = get_data(MAX_LEN_GROUP, group);
    if (rc || strlen(group) == 0)
    {
        printf("Wrong group!\n");
        return;
    }
    int i = 0, find = 0;
    while (i < *count && !find)
    {
        if (strcmp(array[i].group, group) == 0)
            find = 1;
        i++;
    }
    if (!find)
    {
        printf("Records not found!\n");
        return;
    }
    else
    {
        delete_rec(i - 1, *count, array);
        (*count)--;
        printf("Record deleted successfully!\n");
    }
}

void delete_rec(int ind, int count, struct student *array)
{
    for (int j = ind; j < count - 1; j++)
    {
        for (int k = 0; k < (int)strlen(array[j + 1].surname); k++)
            array[j].surname[k] = array[j + 1].surname[k];
        for (int k = 0; k < (int)strlen(array[j + 1].name); k++)
            array[j].name[k] = array[j + 1].name[k];
        for (int k = 0; k < (int)strlen(array[j + 1].group); k++)
            array[j].group[k] = array[j + 1].group[k];
        array[j].gender[0] = array[j + 1].gender[0];
        array[j].age = array[j + 1].age;
        array[j].average_score = array[j + 1].average_score;
        for (int k = 0; k < (int)strlen(array[j + 1].date); k++)
            array[j].date[k] = array[j + 1].date[k];
        for (int k = 0; k < (int)strlen(array[j + 1].housing); k++)
            array[j].housing[k] = array[j + 1].housing[k];
        if (strcmp(array[j + 1].housing, "house") == 0)
        {
            for (int k = 0; k < (int)strlen(array[j + 1].adress.home.street); k++)
                array[j].adress.home.street[k] = array[j + 1].adress.home.street[k];
            array[j].adress.home.house_number = array[j + 1].adress.home.house_number;
            array[j].adress.home.apart_number = array[j + 1].adress.home.apart_number;
        }
        else
        {
            array[j].adress.sweet_home.room_number = array[j + 1].adress.sweet_home.room_number;
            array[j].adress.sweet_home.hostel_number = array[j + 1].adress.sweet_home.hostel_number;
        }
    }
}

void key_init(int count, struct student *array, struct key *keys)
{
    for (int i = 0; i < count; i++)
    {
        keys[i].id = i;
        for (int j = 0; j < (int)strlen(array[i].surname); j++)
            keys[i].surname[j] = array[i].surname[j];
    }
}

void sort_key_by_surname(int count, struct key *keys)
{
    int f;
    for (int i = 1; i < count; i++)
    {
        f = 0;
        for (int j = 0; j < i && !f; j++)
        {
            if (strcmp(keys[i].surname, keys[j].surname) < 0)
            {
                f = 1;
                key_shift(j, i, keys);
            }
        }
    }
}

void key_shift(int i, int j, struct key *keys)
{
    struct key buf;
    for (int k = i; k < j; k++)
    {
        buf.id = keys[k].id;
        keys[k].id = keys[j].id;
        keys[j].id = buf.id;
        for (int l = 0; l <= (int)strlen(keys[k].surname); l++)
            buf.surname[l] = keys[k].surname[l];
        for (int l = 0; l <= (int)strlen(keys[j].surname); l++)
            keys[k].surname[l] = keys[j].surname[l];
        for (int l = 0; l <= (int)strlen(buf.surname); l++)
            keys[j].surname[l] = buf.surname[l];
    }
}

void print_keys(int count, struct key *keys)
{
    output_key_hint();
    char num[4];
    for (int i = 0; i < count; i++)
    {
        printf(" %d | %d", i + 1, keys[i].id);
        long_out(' ', 4 - (int)strlen(itoa(keys[i].id, num, 10)));
        printf("| %s\n", keys[i].surname);
    }
}

void output_key_hint()
{
    long_out('-', 30);
    printf("\nNum| ID  |      Surname\n");
    long_out('-', 30);
    printf("\n");
}

void output_data_by_key(int count, struct key *keys, struct student *array)
{
    out_hint();
    for (int i = 0; i < count; i++)
    {
        printf("%d  | ", i + 1);
        printf("%s", array[keys[i].id].surname);
        long_out(' ', MAX_LEN - strlen(array[keys[i].id].surname));
        printf("| %s", array[keys[i].id].name);
        long_out(' ', MAX_LEN - strlen(array[keys[i].id].name));
        printf("| %s", array[keys[i].id].group);
        long_out(' ', MAX_LEN_GROUP - strlen(array[keys[i].id].group));
        printf("|  %c   |", array[keys[i].id].gender[0]);
        printf("%d | %.2f | ", array[keys[i].id].age, array[keys[i].id].average_score);
        printf("%s", array[keys[i].id].date);
        long_out(' ', 7);
        printf("| ");
        if (strcmp(array[keys[i].id].housing, "hostel") == 0)
        {
            printf("Hostel num %d, room num %d", array[keys[i].id].adress.sweet_home.hostel_number,
                   array[keys[i].id].adress.sweet_home.room_number);
        }
        else
        {
            printf("Str. %s, h. %d, ap. %d", array[keys[i].id].adress.home.street, array[keys[i].id].adress.home.house_number,
                   array[keys[i].id].adress.home.apart_number);
        }
        printf("\n");
    }
}

void sort_data_by_surname(int count, struct student *array)
{
    int f;
    for (int i = 1; i < count; i++)
    {
        f = 0;
        for (int j = 0; j < i && !f; j++)
        {
            if (strcmp(array[i].surname, array[j].surname) < 0)
            {
                f = 1;
                arr_shift(j, i, array);
            }
        }
    }
}

void arr_shift(int i, int j, struct student *arr)
{
    struct student buf;
    for (int k = i; k < j; k++)
    {
        for (int l = 0; l <= (int)strlen(arr[k].surname); l++)
            buf.surname[l] = arr[k].surname[l];
        for (int l = 0; l <= (int)strlen(arr[j].surname); l++)
            arr[k].surname[l] = arr[j].surname[l];
        for (int l = 0; l <= (int)strlen(buf.surname); l++)
            arr[j].surname[l] = buf.surname[l];

        for (int l = 0; l <= (int)strlen(arr[k].name); l++)
            buf.name[l] = arr[k].name[l];
        for (int l = 0; l <= (int)strlen(arr[j].name); l++)
            arr[k].name[l] = arr[j].name[l];
        for (int l = 0; l <= (int)strlen(buf.name); l++)
            arr[j].name[l] = buf.name[l];

        for (int l = 0; l <= (int)strlen(arr[k].group); l++)
            buf.group[l] = arr[k].group[l];
        for (int l = 0; l <= (int)strlen(arr[j].group); l++)
            arr[k].group[l] = arr[j].group[l];
        for (int l = 0; l <= (int)strlen(buf.group); l++)
            arr[j].group[l] = buf.group[l];

        buf.gender[0] = arr[k].gender[0];
        arr[k].gender[0] = arr[j].gender[0];
        arr[j].gender[0] = buf.gender[0];

        buf.age = arr[k].age;
        arr[k].age = arr[j].age;
        arr[j].age = buf.age;

        buf.average_score = arr[k].average_score;
        arr[k].average_score = arr[j].average_score;
        arr[j].average_score = buf.average_score;

        for (int l = 0; l <= (int)strlen(arr[k].date); l++)
            buf.date[l] = arr[k].date[l];
        for (int l = 0; l <= (int)strlen(arr[j].date); l++)
            arr[k].date[l] = arr[j].date[l];
        for (int l = 0; l <= (int)strlen(buf.date); l++)
            arr[j].date[l] = buf.date[l];

        if (strcmp(arr[k].housing, "house") == 0)
        {
            for (int l = 0; l <= (int)strlen(arr[k].adress.home.street); l++)
                buf.adress.home.street[l] = arr[k].adress.home.street[l];

            buf.adress.home.house_number = arr[k].adress.home.house_number;
            buf.adress.home.apart_number = arr[k].adress.home.apart_number;

            if (strcmp(arr[j].housing, "house") == 0)
            {
                //printf("1s\n");
                for (int l = 0; l <= (int)strlen(arr[j].adress.home.street); l++)
                    arr[k].adress.home.street[l] = arr[j].adress.home.street[l];
                for (int l = 0; l <= (int)strlen(buf.adress.home.street); l++)
                    arr[j].adress.home.street[l] = buf.adress.home.street[l];

                arr[k].adress.home.house_number = arr[j].adress.home.house_number;
                arr[k].adress.home.apart_number = arr[j].adress.home.apart_number;

                arr[j].adress.home.house_number = buf.adress.home.house_number;
                arr[j].adress.home.apart_number = buf.adress.home.apart_number;
            }
            else
            {
                //printf("2s %s %s\n", arr[k].surname, arr[j].surname);
                arr[k].adress.sweet_home.hostel_number = arr[j].adress.sweet_home.hostel_number;
                arr[k].adress.sweet_home.room_number = arr[j].adress.sweet_home.room_number;

                for (int l = 0; l <= (int)strlen(buf.adress.home.street); l++)
                    arr[j].adress.home.street[l] = buf.adress.home.street[l];

                arr[j].adress.home.house_number = buf.adress.home.house_number;
                arr[j].adress.home.apart_number = buf.adress.home.apart_number;
            }
        }
        else
        {
            buf.adress.sweet_home.hostel_number = arr[k].adress.sweet_home.hostel_number;
            buf.adress.sweet_home.room_number = arr[k].adress.sweet_home.room_number;

            if (strcmp(arr[j].housing, "house") == 0)
            {
                //printf("3s %s %s\n", arr[k].surname, arr[j].surname);
                for (int l = 0; l <= (int)strlen(arr[j].adress.home.street); l++)
                    arr[k].adress.home.street[l] = arr[j].adress.home.street[l];

                arr[k].adress.home.house_number = arr[j].adress.home.house_number;
                arr[k].adress.home.apart_number = arr[j].adress.home.apart_number;

                arr[j].adress.sweet_home.hostel_number = buf.adress.sweet_home.hostel_number;
                arr[j].adress.sweet_home.room_number = buf.adress.sweet_home.room_number;
            }
            else
            {
                //printf("4s\n");
                arr[k].adress.sweet_home.hostel_number = arr[j].adress.sweet_home.hostel_number;
                arr[j].adress.sweet_home.hostel_number = buf.adress.sweet_home.hostel_number;

                arr[k].adress.sweet_home.room_number = arr[j].adress.sweet_home.room_number;
                arr[j].adress.sweet_home.room_number = buf.adress.sweet_home.room_number;
            }
        }

        for (int l = 0; l <= (int)strlen(arr[k].housing); l++)
            buf.housing[l] = arr[k].housing[l];
        for (int l = 0; l <= (int)strlen(arr[j].housing); l++)
            arr[k].housing[l] = arr[j].housing[l];
        for (int l = 0; l <= (int)strlen(buf.housing); l++)
            arr[j].housing[l] = buf.housing[l];
    }
}

void key_efficiency(int count, struct student *array)
{
    struct student a[MAX_ARR];
    struct key keys[MAX_ARR];
    int M = 50;
    int64_t sum_0 = 0, sum_1 = 0;


    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < count; j++)
            a[j] = array[j];

        key_init(count, array, keys);

        sum_0 += time_of_table_sorting(count, a);
        sum_1 += time_of_key_sorting(count, keys);
    }

    float sr_0 = sum_0 / M;
    float sr_1 = sum_1 / M;
    printf("Average time of table sorintg is %d\n", (int)sr_0);
    printf("Average time of key sorintg is %d\n", (int)sr_1);
    printf("Key sorting is efficient by time on %d percent\n", (int)((sr_0 / sr_1 - 1) * 100));
    //printf("Key sorting is unefficient by memory on %d percent\n", (int)(sizeof(struct key) / sizeof(struct student)) * 100);
    //printf("%d %d\n", (int)(sizeof(struct student)), (int)sizeof(struct key));
    printf("Key sorting (%d records) is unefficient by memory on %d percent\n", count, (int) (25.0 / 124.0 * 100));
}

unsigned long long time_of_table_sorting(int count, struct student *array)
{
    unsigned long long t = tick();
    sort_data_by_surname(count, array);
    t = tick() - t;
    return t;

}

unsigned long long time_of_key_sorting(int count, struct key *keys)
{
    unsigned long long t = tick();
    sort_key_by_surname(count, keys);
    t = tick() - t;
    return t;

}

void type_of_sorting_efficiency(int count, struct student *arr)
{
    struct student a[MAX_ARR];
    int M = 50;
    int64_t sum_0 = 0, sum_1 = 0;


    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < count; j++)
            a[j] = arr[j];

        sum_0 += time_of_table_sorting(count, a);

        for (int j = 0; j < count; j++)
            a[j] = arr[j];

        sum_1 += time_of_qsorting_table(count, a);
    }

    float sr_0 = sum_0 / M;
    float sr_1 = sum_1 / M;
    printf("Average time of insertion sorintg is %ld\n", (long)sr_0);
    printf("Average time of quick sorintg is %ld\n", (long)sr_1);
    printf("Key sorting is efficient on %ld percent\n", (long)((sr_0 / sr_1 - 1) * 100));
}

unsigned long long time_of_qsorting_key(int count, struct key *a)
{
    unsigned long long t = tick();
    qsort(a, count, sizeof(struct key), compare);
    t = tick() - t;
    return t;
}

int compare(const void *sur1, const void *sur2)
{
    return strcmp((char *)sur1, (char *)sur2);
}

unsigned long long time_of_qsorting_table(int count, struct student *a)
{
    unsigned long long t = tick();
    qsort(a, count, sizeof(struct student), compare);
    t = tick() - t;
    return t;
}

int compare_2(const void *a1, const void *a2)
{
    return strcmp((char *)a1, (char *)a2);
}

unsigned long long tick(void)
{
unsigned long long d;
__asm__ __volatile__ ("rdtsc": "=A" (d));
return d;
}

void end_hint()
{
    printf("Its end of working the program!\n");
}
