#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"

#ifndef _QUEUE_FUNC_H_
#define _QUEUE_FUNC_H_

unsigned long simulate_service(struct mem_slot **mem, int *mem_used);
void new_req(double *total_time, struct queue *queue, struct machine *machine, struct mem_slot **mem);
int processing(struct machine *machine, struct queue *queue, struct mem_slot **mem);

unsigned long simulate_service_arr(int *mem_used);
void new_req_arr(double *total_time, struct machine *machine, struct queue *queue, struct queue_slot *array, int mass_len);
int processing_arr(struct machine *machine, struct queue *queue, struct queue_slot *arr, int arr_len);

void show_mem(struct mem_slot **mem);
void queue_compare(struct mem_slot **mem);

#endif
