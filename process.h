#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct process {
	char *name;
    int time;
    int totalCPU;
    int completeTime;
    int givenCPU;
    int BlockedIO;
    int doingIO;
    float prob;
};

struct resource {
	char *name; // CPU or IO
    int busy;
    int idle;
    double utilization; // busy / (busy + idle)
    int number; // dispatches or IO time
    double throughput;
};

// queue_t q;
// struct resource *sysCPU, *sysIO;

struct process* generateProcess(char *name, int time,int totalCPU, int completeTime, int givenCPU, int BlockedIO, int doingIO,float prob) {
    struct process* tmp = (struct process*)malloc(sizeof(struct process));
    tmp->name = name;
    tmp->time = time;
    tmp->totalCPU = totalCPU;
	tmp->completeTime = completeTime;
	tmp->givenCPU = givenCPU;
	tmp->BlockedIO = BlockedIO;
    tmp->doingIO = doingIO;
    tmp->prob = prob;
    return tmp;
}

void displayProcess(struct process* p) {
    printf("name\ttime\ttotalCPU\tcompleteTime\tgivenCPU\tBlockedIO\tdoingIO\tprob\n");
    printf("%s\t%d\t%d\t\t%d\t\t%d\t\t%d\t\t%dt\t%dt\t%f\n", p->name,p->time, p->totalCPU, p->completeTime, p->givenCPU, p->BlockedIO, p->doingIO, p->prob);
}

struct resource* buildResource(char *name, int busy, int idle, int number) {
    struct resource* tmp = (struct resource*)malloc(sizeof(struct resource));
    tmp->name = name;
    tmp->busy = busy;
	tmp->idle = idle;
	tmp->utilization = (double)busy / (double)(busy + idle);
	tmp->number = number;
    tmp->throughput = (double)number / (double)(busy + idle);
    return tmp;
}

void displayResource(struct resource* res) {
    if(strcmp(res->name, "CPU")) {
        printf("%s:\n", res->name);
        printf("Total time spent busy: %d\n", res->busy);
        printf("Total time spent idle: %d\n", res->idle);
        printf("CPU utilization: %.2f\n", res->utilization);
        printf("Number of dispatches: %d\n", res->number);
        printf("Overall throughput: %.2f\n", res->throughput);
    }
    else if(strcmp(res->name, "IO")) {
        printf("%s:\n", res->name);
        printf("Total time spent busy: %d\n", res->busy);
        printf("Total time spent idle: %d\n", res->idle);
        printf("I/O device utilization: %.2f\n", res->utilization);
        printf("Number of times I/O was started: %d\n", res->number);
        printf("Overall throughput: %.2f\n", res->throughput);
    }
    else {
        printf("Error Resource Type");
    }
}