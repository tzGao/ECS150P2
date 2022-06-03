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
        printf("--- %s checking for block, calculated probability at %f \n", head->name, r);
        if (head->prob >= r && head->time > 0) {
            int r = ((int) random() % head->time) + 1;
            printf("-- %s will block at %d, CURRENT REMAINING TIME: %d\n", head->name, r, head->time);
            head->timeToBlock = time + r - 1;
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
    bool ignore = false;
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
            printf("CPU ACTIVE, CURRENT PROCESS is %s AT TIME: %d\n", CPU->name, CPU->time);
            if (CPU->timeToBlock == time && CPU->timeToBlock != -1 && CPU->willBlock) {
                printf("%s exiting CPU to IO Queue, TIME: %d\n", CPU->name, time);
                CPU->timeToBlock = 0;
                CPU->time--; //RESYNCHRONIZE
                queue_enqueue(ioQ, CPU);
                CPU = NULL;
                ignore = true;
            } else {
                CPU->time--;
                if (CPU->time <= 0) {
                    printf("%s finished, TIME: %d\n", CPU->name, time);
                    free(CPU);
                    CPU = NULL;
                }
            }
            cpuBusy++;
        } else {
            cpuIdle++;
        }

        
        printf("/-------/IO QUEUE FUNCTION/----------/\n");
        printf("IO QUEUE LENGTH %d\n", queue_length(ioQ));
        if (queue_length(ioQ) > 0) {
            if (IO == NULL && !ignore) {
                struct Process* curr = ioQ->head->value;
                int r = 0;
                if (curr->time > 0) {
                    int rand = (int) random();
                    printf("Calculated %d", rand);
                    r = (rand % 30) + 1;
                }
                printf("*** %s entering IO\n", curr->name);
                curr->ioTime = r;
                printf("** %s ioTime will be %d, current TIME is: %d\n", curr->name, r, curr->time);
                IO = curr;
                queue_delete(ioQ, curr);
                ioDispatches++;
            }
        }
        ignore = false;

        
        printf("/-------/IO FUNCTION/----------------/\n");

        if (IO != NULL) {
            IO->ioTime--;
            printf("IO ACTIVE, CURRENT PROCESS is %s at ioTIME: %d\n", IO->name, IO->ioTime);
            if (IO->ioTime <= 0) {
                printf("*** %s exiting IO, entering CPU, TIME: %d\n", IO->name, time);
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
