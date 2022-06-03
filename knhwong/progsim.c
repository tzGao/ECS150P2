#include <stdio.h>
#include <stdlib.h>
#include  <stdbool.h> 
#include <string.h>
#include "queue.h"

struct Process {
    char name[256];
    int time;
    float prob;
    int ioTime;
    int timeToBlock;
    bool willBlock;
};

struct Process* makeProcess(char* name, int time, float prob) {
    struct Process* p = malloc(sizeof(struct Process));
    strcpy(p->name, name);
    p->time = time;
    p->prob = prob;
    p->timeToBlock = -1;
    return p;
}


queue_t readFile(char* fileName, FILE* fp){
    fp = fopen(fileName, "r");

    queue_t q = queue_create();
    
    
    char name[256];
    int time;
    float prob;
    int curr = EOF;
    do
    {   
        curr = fscanf(fp, "%s %d %f", name, &time, &prob);
        if (curr != -1) {
            struct Process* p = makeProcess(name, time, prob);
            printf("%s %d %f \n", p->name, p->time, p->prob);
            queue_enqueue(q, p);

        }
    } while (curr != EOF);

    return q;
}

void checkBlock(struct Process* head, int time) {
        float r = (float) random() / RAND_MAX;
        printf("<%s> checking for block, calculated probability at %f \n", head->name, r);
        if (head->prob >= r && head->time > 0) {
            int r = ((int) random() % head->time) + 1;
            printf("<%s> will block at %d, CURRENT REMAINING TIME: %d\n", head->name, r, head->time);
            head->timeToBlock = time + r ;
            if (head->time >= 2) {
                head->willBlock = true;
            } else {
                head->willBlock = false;
            }
        }
}



int main(int argc, char* argv[]){
    (void) srandom(12345);
    FILE* fp;
    queue_t cpuQ = readFile(argv[1], fp);
    queue_t ioQ = queue_create();


    int cpuBusy = 0;
    int cpuIdle = 0;
    int cpuDispatches = 0;

    int ioBusy = 0;
    int ioIdle = 0;
    int ioDispatches = 0;


    
    printf("/--------------------------/\n");
    int time = 1;
    struct Process* CPU = NULL;
    struct Process* IO = NULL;
    while (queue_length(cpuQ) != 0 || queue_length(ioQ) != 0 || CPU != NULL || IO != NULL) {
        printf("/----------------------------CURRENT TICK %d----------------------------/\n", time);

        printf("/-------/READY QUEUE FUNCTION/-------/\n");
        if (queue_length(cpuQ) > 0) {
            if (CPU == NULL) {
                struct Process* head = cpuQ->head->value;
                checkBlock(head, time);
                CPU = head;
                queue_delete(cpuQ, head);
                cpuDispatches++;
            }
        }
        
        printf("/-------/CPU FUNCTION/---------------/\n");
        if (CPU != NULL) {
            printf("CPU ACTIVE: <%s> TIME: %d\n", CPU->name, CPU->time);
            if (CPU->time == 0) {
                printf("<%s> HAS FINISHED, TIME: %d\n", CPU->name, time);
                free(CPU);
                CPU = NULL;

            } else {
                CPU->time--;
                if (CPU->timeToBlock == time && CPU->timeToBlock != 0 && CPU->willBlock) {
                    printf("<%s> EXITING CPU TO IO QUEUE, CPU TIME: %d\n", CPU->name, CPU->time);
                    CPU->timeToBlock = 0;
                    queue_enqueue(ioQ, CPU);
                    CPU = NULL;
                }
            }
            cpuBusy++;
        } else {
            cpuIdle++;
        }

        
        printf("/-------/IO QUEUE FUNCTION/----------/\n");
        if (queue_length(ioQ) > 0) {
            if (IO == NULL) {
                struct Process* curr = ioQ->head->value;
                int r = 0;
                if (curr->time > 0) {
                    int rand = (int) random();
                    r = (rand % 30) + 1;
                }
                curr->ioTime = r;
                printf("|<%s>| ENTERING IO, ioTime: %d, time: %d\n", curr->name, r, curr->time);
                IO = curr;
                queue_delete(ioQ, curr);
                ioDispatches++;
            }
        }

        
        printf("/-------/IO FUNCTION/----------------/\n");
        if (IO != NULL) {
            IO->ioTime--;
            printf("IO ACTIVE, CURRENT PROCESS <%s> ioTIME: %d\n", IO->name, IO->ioTime);
            if (IO->ioTime <= 0) {
                printf("|<%s>| EXITING IO, entering CPU\n", IO->name);
                queue_enqueue(cpuQ, IO);
                IO = NULL;
                }
            ioBusy++;
        } else {
            ioIdle++;
        }
        time++;
    }
    printf("System:\nThe wall clock time at which the simulation finished: %d\n", time);
    printf("CPU:\nTotal time spent busy: %d\nTotal time spent idle: %d\nNumber of dispatches: %d\n", cpuBusy, cpuIdle, cpuDispatches);
    printf("IO:\nTotal time spent busy: %d\nTotal time spent idle: %d\nNumber of dispatches: %d\n", ioBusy, ioIdle, ioDispatches);
   return 0;
}
