#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define MAX_LEN 21
#define MAX_LEN_GROUP 11
#define MAX_DATE_LEN 11
#define MAX_ARR 1000

struct house
{
    char street[MAX_LEN];
    int house_number;
    int apart_number;
};

struct hostel
{
    int hostel_number;
    int room_number;
};

struct student
{
    char surname[MAX_LEN];
    char name[MAX_LEN];
    char group[MAX_LEN_GROUP];
    char gender[2];
    int age;
    float average_score;
    char date[MAX_DATE_LEN];
    char housing[MAX_LEN];
    union adress{
        struct house home;
        struct hostel sweet_home;
    }adress;
};

struct key
{
    int id;
    char surname[MAX_LEN];
};

void app_hint();

void output_data_all(int count, struct student *array);
void output_data_certain(int count, struct student *array);
void out_hint();
void long_out(char a,int count);

void add_data(int *count, struct student *array, FILE *f);
int date_parser(char *date);

void delete_by_field(int *count, struct student *array);

void key_init(int count, struct student *array, struct key *keys);
void sort_key_by_surname(int count, struct key *keys);
void print_keys(int count, struct key *keys);

void output_data_by_key(int count, struct key *keys, struct student *array);

void sort_data_by_surname(int count, struct student *array);

void key_efficiency(int count, struct student *array);

void type_of_sorting_efficiency(int count, struct student *arr);

int compare(const void *x, const void *y);
int compare_2(const void *a1, const void *a2);

void end_hint();

#endif
