#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "queue_func.h"

void add_mem(struct queue_slot *slot, struct mem_slot **mem);
void delete_mem(struct queue_slot *slot, struct mem_slot *mem);

unsigned long simulate_service(struct mem_slot **mem, int *mem_used)
{
    struct queue queue;
    queue.pin = queue.pout = NULL;
    queue.len = queue.in_num = queue.state = queue.max = 0;
    queue.total_stay_time = 0;
    struct machine machine;
    machine.time = machine.downtime = 0;
    machine.processed_count = machine.triggering = 0;
    double total_time = 0;
    int f = 0;
    printf("\n        SIMULATING STARTED\n\n");
    unsigned long time = tick(), t_buf1 = 0, t_buf2 = 0;
    while (machine.processed_count < TOTAL_NEED)
    {
        while (queue.len == 0 || machine.time > total_time)
        {
            new_req(&total_time, &queue, &machine, mem);
        }
        f = processing(&machine, &queue, mem);
        t_buf1 = tick();
        if (f && machine.processed_count % INTER_NEED == 0)
        {
            printf("---Processed reqeuest number is %d---\n", machine.processed_count);
            printf("Current queue length is %d\n", queue.len);
            printf("Several queue length is %.2f\n\n", (float)(queue.state / machine.triggering));
        }
        t_buf2 += tick() - t_buf1;
        if (f)
            f = 0;
    }
    time = tick() - time - t_buf2;
    double model = 0;
    /*if ((int)(COMING_END + COMING_START) / 2 > (int)(PROCESSING_END + PROCESSING_START) / 2)
        model = (float)(COMING_END + COMING_START) / 2 * TOTAL_NEED;
    else
        model = (float)(PROCESSING_END + PROCESSING_START) / 2 * TOTAL_NEED * (1 / 0.2);*/
    model = (float)(PROCESSING_END + PROCESSING_START) / 2 * TOTAL_NEED * (1 / 0.2);
    *mem_used = queue.max * sizeof(struct queue_slot);
    printf("Vaiting time of modeling: %.2f\n", model);
    printf("Overal time of modeling: %.2f\n", machine.time);
    printf("Fault: ~%.2f%%\n\n", (machine.time - model) / model * 100);
    printf("Number of coming requests: %d\n", queue.in_num);
    printf("Number of quiting requests: %d\n", machine.processed_count);
    printf("Several time of standing in queue: %.2f\n", queue.total_stay_time / machine.processed_count);
    printf("Time of standing machine: %.2f\n", machine.downtime);
    printf("Count of triggering machine: %d\n\n", machine.triggering);
    return time;
}

void new_req(double *total_time, struct queue *queue, struct machine *machine, struct mem_slot **mem)
{
    double time = get_time(COMING_START, COMING_END);
    *total_time += time;
    if (queue->len == 0 && *total_time > machine->time)
    {
        machine->downtime += *total_time - machine->time;
        machine->time = *total_time;
    }
    struct queue_slot *buf = malloc(sizeof(struct queue_slot));
    if (queue->pin != NULL)
        queue->pin->next = buf;
    buf->next = NULL;
    buf->arrival_time = *total_time;
    queue->pin = buf;
    if (queue->len == 0)
        queue->pout = buf;
    queue->len++;
    queue->in_num++;
    add_mem(buf, mem);
}

void add_mem(struct queue_slot *slot, struct mem_slot **mem)
{
    struct mem_slot *buf = malloc(sizeof(struct mem_slot));
    buf->queue_slot = slot;
    buf->busy = 1;
    buf->next = *mem;
    *mem = buf;
}

int processing(struct machine *machine, struct queue *queue, struct mem_slot **mem)
{
    int f = 0;
    queue->state += queue->len;
    if (queue->len > queue->max)
        queue->max = queue->len;
    machine->triggering++;
    machine->time += get_time(PROCESSING_START, PROCESSING_END);
    struct queue_slot *buf = queue->pout;
    if (queue->len > 1)
        queue->pout = buf->next;
    if (!chance())
    {
        queue->pin->next = buf;
        queue->pin = buf;
        buf->next = NULL;
    }
    else
    {
        if (queue->len == 1)
            queue->pin = queue->pout = NULL;
        queue->total_stay_time += machine->time - buf->arrival_time;
        machine->processed_count++;
        queue->len--;
        free(buf);
        delete_mem(buf, *mem);
        f = 1;
    }
    return f;
}

void delete_mem(struct queue_slot *slot, struct mem_slot *mem)
{
    struct mem_slot *buf = mem;
    int f = 0;
    while (!f && buf != NULL)
    {
        if (buf->queue_slot == slot)
        {
            f = 1;
            buf->busy = 0;
        }
        buf = buf->next;
    }
}

unsigned long simulate_service_arr(int *mem_used)
{
    int all = TOTAL_NEED;
    struct queue_slot *arr = malloc(all * sizeof(struct queue_slot));
    struct queue queue;
    queue.pin = queue.pout = arr;
    queue.len = queue.in_num = queue.state = 0;
    queue.total_stay_time = 0;
    struct machine machine;
    machine.time = machine.downtime = 0;
    machine.processed_count = machine.triggering = 0;
    double total_time = 0;
    int f = 0;
    printf("\n        SIMULATING STARTED\n\n");
    unsigned long time = tick(), t_buf1 = 0, t_buf2 = 0;
    while (f != 2 && machine.processed_count < TOTAL_NEED)
    {
        while (queue.len == 0 || machine.time > total_time)
        {
                new_req_arr(&total_time, &machine, &queue, arr, all);
                if (queue.len != 0 && queue.len != 1 && queue.pout == queue.pin)
                    f = 2;
        }
        if (f != 2)
            f = processing_arr(&machine, &queue, arr, all);
        t_buf1 = tick();
        if (f == 1 && machine.processed_count % INTER_NEED == 0)
        {
            printf("---Processed reqeuest number is %d---\n", machine.processed_count);
            printf("Current queue length is %d\n", queue.len);
            printf("Several queue length is %.2f\n\n", (float)(queue.state / machine.triggering));
        }
        t_buf2 += tick() - t_buf1;
        if (f == 1)
            f = 0;
    }
    time = tick() - time - t_buf2;
    if (f == 2)
        printf("Array is full! Simulating stopped.\nQueue len is %d\n", queue.len);
    else
    {
        double model = 0;
        /*if ((int)(COMING_END + COMING_START) / 2 > (int)(PROCESSING_END + PROCESSING_START) / 2)
            model = (float)(COMING_END + COMING_START) / 2 * TOTAL_NEED;
        else
            model = (float)(PROCESSING_END + PROCESSING_START) / 2 * TOTAL_NEED * (1 / 0.2);*/
        model = (float)(PROCESSING_END + PROCESSING_START) / 2 * TOTAL_NEED * (1 / 0.2);
        *mem_used = all * (sizeof(struct queue_slot) - sizeof(struct queue_slot *));
        printf("Vaiting time of modeling: %.2f\n", model);
        printf("Overal time of modeling: %.2f\n", machine.time);
        printf("Fault: ~%.2f%%\n\n", (machine.time - model) / model * 100);
        printf("Number of coming requests: %d\n", queue.in_num);
        printf("Number of quiting requests: %d\n", machine.processed_count);
        printf("Several time of standing in queue: %.2f\n", queue.total_stay_time / machine.processed_count);
        printf("Time of standing machine: %.2f\n", machine.downtime);
        printf("Count of triggering machine: %d\n\n", machine.triggering);
    }
    free(arr);
    return time;
}

void new_req_arr(double *total_time, struct machine *machine, struct queue *queue, struct queue_slot *array, int mass_len)
{
    double time = get_time(COMING_START, COMING_END);
    *total_time += time;
    if (queue->len == 0 && *total_time > machine->time)
    {
        machine->downtime += *total_time - machine->time;
        machine->time = *total_time;
    }
    if (queue->len > 1 && queue->pin != queue->pout)
    {
        if (queue->pin == array + mass_len - 1)
            queue->pin = array;
        else
            queue->pin++;
    }
    if (queue->len == 1)
    {
        if (queue->pin == array + mass_len - 1)
            queue->pin = array;
        else
            queue->pin++;
    }
    //if queue.len == 0 то pin++ не требуется
    queue->pin->arrival_time = *total_time;
    queue->len++;
    queue->in_num++;
}

int processing_arr(struct machine *machine, struct queue *queue, struct queue_slot *arr, int arr_len)
{
    int f = 0;
    queue->state += queue->len;
    machine->triggering++;
    machine->time += get_time(PROCESSING_START, PROCESSING_END);
    struct queue_slot *buf = queue->pout;
    if (queue->pout == arr + arr_len - 1)
        queue->pout = arr;
    else
        queue->pout++;
    if (!chance())
    {
        if (queue->pin == arr + arr_len - 1)
            queue->pin = arr;
        else
            queue->pin++;
        //queue->pin++;
        queue->pin->arrival_time = buf->arrival_time;
    }
    else
    {
        if (queue->len == 1)
            queue->pin = queue->pout;
        queue->total_stay_time += machine->time - buf->arrival_time;
        machine->processed_count++;
        queue->len--;
        f = 1;
    }
    return f;
}

void show_mem(struct mem_slot **mem)
{
    struct mem_slot *buf = *mem, *buf1;
    if (buf == NULL)
        printf("\nSimulating on list didn't use yet.\n\n");
    else
    {
        printf("Usage of memory:\n");
        while (buf != NULL)
        {
            printf("%p is ", buf->queue_slot);
            if (buf->busy == 1)
                printf("used.\n");
            else
                printf("not used.\n");
            buf1 = buf;
            buf = buf->next;
            free(buf1);
        }
        *mem = NULL;
    }
}

void queue_compare(struct mem_slot **mem)
{
    int mem1 = 0, mem2 = 0;
    unsigned long t1 = simulate_service(mem, &mem1);
    unsigned long t2 = simulate_service_arr(&mem2);
    printf("Show mem list? 1 - yes, 2 - no : ");
    int show = 0, rc = 0;
    rc = scanf("%d", &show);
    if (rc && show == 1)
        show_mem(mem);
    struct mem_slot *buf = *mem;
    while (buf)
    {
        *mem = buf->next;
        free(buf);
        buf = *mem;
    }
    printf("Time of list-queue: %lu\n", t1);
    printf("Time of array-queue: %lu\n", t2);
    printf("Memory of list-queue: %d\n", mem1);
    printf("Memory of array-queue: %d\n", mem2);
}
