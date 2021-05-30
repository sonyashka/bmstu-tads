#include <stdio.h>
#include <stdlib.h>

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define MAX 10
#define TOTAL_NEED 1000
#define INTER_NEED 100
#define COMING_START 0
#define COMING_END 6
#define PROCESSING_START 0
#define PROCESSING_END 1

struct queue_slot
{
    double arrival_time;
    struct queue_slot *next;
};

struct queue
{
    struct queue_slot *pin;
    struct queue_slot *pout;
    int len;
    int in_num;
    int state;
    int max;
    double total_stay_time;
};

struct machine
{
    double time;
    double downtime;
    int triggering;
    int processed_count;
};

struct mem_slot
{
    struct queue_slot *queue_slot;
    int busy;
    struct mem_slot *next;
};

void user_hint();
void end_hint();
unsigned long long tick(void);
double get_time(int t1, int t2);
int chance(void);

#endif
