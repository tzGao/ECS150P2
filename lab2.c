#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <assert.h>
#include <stdio.h>



struct process {
	char *name;
    int totalCPU;
    int completeTime;
    int givenCPU;
    int BlockedIO;
    int doingIO;
};

struct resource {
	char *name; // CPU or IO
    int busy;
    int idle;
    double utilization; // busy / (busy + idle)
    int number; // dispatches or IO time
    double throughput;
};



struct process* generateProcess(char *name, int totalCPU, int completeTime, int givenCPU, int BlockedIO, int doingIO) {
    struct process* tmp = (struct process*)malloc(sizeof(struct process));
    tmp->name = name;
    tmp->totalCPU = totalCPU;
	tmp->completeTime = completeTime;
	tmp->givenCPU = givenCPU;
	tmp->BlockedIO = BlockedIO;
    tmp->doingIO = doingIO;
    return tmp;
}

void displayProcess(struct process* p) {
    printf("name\ttotalCPU\tcompleteTime\tgivenCPU\tBlockedIO\tdoingIO\n");
    printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p->name, p->totalCPU, p->completeTime, p->givenCPU, p->BlockedIO, p->doingIO);
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


queue_t readFile(char* fileName, FILE* filePointer){
    queue_t cpuqueue = queue_create();
    FILE *fp;
    char line[256];
    fp = fopen(fileName, "r");
    if( fp == NULL ) {
        // The following string is prepended to the error description.
        errno = ENOENT;
        perror(fileName);
    }else{
        
        int line_num = 0;
        while (fgets(line, sizeof(line), fp)) {
            line_num +=1;
            printf("%s", line); 
            char *name;
            char *runtime;
            char *prob;
            int taken;
            
            int size = sizeof(line) / sizeof(line[0]);
            for(int i =0; i< size;i++){
                printf("%c",line[i]);
            }
            int current;//determine which part we are going to put information
            for(int i =0; i< size;i++){
               
                if ((line[i] == ' ')|| (line[i]=='\n')){
                    if (taken == 1){
                        current +=1;
                        taken ==0;
                    }
                }else{
                    taken =1;
                    if (current == 0){
                        name+=line[i];
                    }else if (current ==1){
                        runtime+=line[i];
                    }
                    else if (current ==3){
                        prob+=line[i];
                    }
                    //
                }
            }
            // printf("name: ");
            // printf(name);
            struct process* p = generateProcess(name,atoi(runtime),0,0,0,0);
            //displayProcess(p);
            queue_enqueue(cpuqueue,p);
        }
        

        fclose(fp); 
        printf("hi");
        printf("%d",line_num);
        
    }
    
    return cpuqueue;
    
}

int main(int argc, char* argv[]){
    FILE* fp;
    readFile(argv[1], fp);
}