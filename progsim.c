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


struct queue readFile(char* fileName, FILE* fp){
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


    //printf("%d \n", queue_length(q));

    fclose(fp);
}

int main(int argc, char* argv[]){
    FILE* fp;
    struct queue q = readFile(argv[1], fp);

}
