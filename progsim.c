#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "process.h"

// struct Process {
//     char name[256];
//     int time;
//     float prob;
// };

// struct Process* makeProcess(char* name, int time, float prob) {
//     struct Process* p = malloc(sizeof(struct Process));
//     strcpy(p->name, name);
//     p->time = time;
//     p->prob = prob;
//     return p;
// }

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
            struct process* p = generateProcess(name, time,time,0,0,0,0,prob);
            displayProcess(p);
            queue_enqueue(q, p);
        }
    } while (curr != EOF);

    fclose(fp);
    return q;
}
void run(queue_t cpuQ,queue_t ioQ,struct resource* sysCPU,struct resource* sysIO ){
    struct process *cpu;		/* points to process on cpu */
    struct process *iodev; 	/* points to process on io device */
    int io_servicetime[10000];
    int io_total_num=-1;//total index of io process
    int io_current_num=-1;//current index of io process
    cpu = &cpuQ[0];
    sysCPU->number+=1;
    
    int current_time =0;
    while (queue_length(cpuQ) != 0 && queue_length(ioQ) != 0) {
        
        //cpu part
        if(queue_length(cpuQ) != 0){
            

            //has at least 2 time units left to run
            if (cpu->time >2){
                float r = (int) rand() % 100;
                if (r < cpu->prob){
                    //block
                    (void) srand(30);
                    io_total_num+=1;
                    io_servicetime[io_total_num] = (int) rand();
                    if (io_current_num==-1){
                        io_current_num+=1;
                    }
                    queue_dequeue(cpuQ,cpu);
                    queue_enqueue(ioQ,cpu);
                    if(queue_length(ioQ) == 0){
                        //initial pointer when ioq is empty
                        iodev = &ioQ[0];
                    }
                    if(queue_length(cpuQ) != 0){
                        //get next pocess when it's not empty
                        cpu = &cpuQ[0];
                        sysCPU->number+=1;
                    }
                }else{
                    //didn't block
                    sysCPU->busy +=1;
                    cpu->time = cpu->time-1;
                }
            //has 2 time units left to run, don't block
            }else {
                if (cpu->time == 0){
                    if(queue_length(cpuQ) != 0){
                            cpu = &cpuQ[0];
                            sysCPU->number+=1;
                    }else{
                            //no more process in cpuQ
                            queue_dequeue(cpuQ,cpu);
                            
                    }
                }else{
                    sysCPU->busy +=1;
                    cpu->time = cpu->time-1;
                }
            }
        }else{
            sysCPU->idle +=1;
        }

        // io part
        if(queue_length(ioQ) != 0){
            if(io_servicetime[io_current_num]!=0){
                io_servicetime[io_current_num] -=1;
            }else{
                if(iodev->time ==0){
                    queue_dequeue(ioQ,iodev);
                    if (queue_length(ioQ)!=0){
                        iodev = &ioQ[0];
                        io_current_num+=1;
                    }
                }else{
                    //put back to cpu queue
                    queue_dequeue(ioQ,iodev);
                    queue_enqueue(cpuQ,cpu);
                    if (queue_length(ioQ)!=0){
                        iodev = &ioQ[0];
                        io_current_num+=1;
                    }
                }
                
            }
        }
    }

}

int main(int argc, char* argv[]){
    FILE* fp;
    queue_t cpuQ = readFile(argv[1], fp);
    queue_t ioQ = queue_create();

    struct resource *sysCPU = buildResource("sysCPU",0,0,0);
    struct resource *sysIO = buildResource("sysIO",0,0,0);
    run(cpuQ,ioQ,sysCPU,sysIO);

}

