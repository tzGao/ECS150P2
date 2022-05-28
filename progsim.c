//Just Parser right now.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char name[256];
    int time;
    float prob;
};

void readFile(char* fileName, FILE* fp){
    fp = fopen(fileName, "r");
    char name[256];
    int time;
    float prob;
    char curr = EOF;
    do
    {   
        curr = fscanf(fp, "%s %d %f", name, &time, &prob);
        //Pointer Malloc so we enqueue in.
        struct Process* p = malloc(sizeof(struct Process));
        strcpy(p->name, name);
        p->time = time;
        p->prob = prob;

        printf("%s %d %f \n", p->name, p->time, p->prob);
    } while (curr != EOF);

    fclose(fp);
}

int main(int argc, char* argv[]){
    FILE* fp;
    readFile(argv[1], fp);
}
