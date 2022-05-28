#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct Process {
    char name[256];
    int time;
    float prob;
};

struct Process* makeProcess(char* name, int time, float prob) {
    struct Process* p = malloc(sizeof(struct Process));
    strcpy(p->name, name);
    p->time = time;
    p->prob = prob;
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

    fclose(fp);
    return q
}

int main(int argc, char* argv[]){
    (void) srandom(12345);
    FILE* fp;
    queue_t cpuQ = readFile(argv[1], fp);
    queue_t ioQ = queue_create();
    int time = 0;
    /*
    while (queue_length(cpuQ) != 0 && queue_length(ioQ) != 0) {
    }
    */
    int r = (int) random() % 100;
    struct Process* p = cpuQ->head->value;
    printf("%d", p->time);


}

