#define _GNU_SOURCE  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sched.h>
#include <sys/syscall.h>
#include <time.h>

double a;

int main(int argc, char *argv[])
{
    int cpu, status, i;
    int *address_from_malloc;
    cpu_set_t my_set;        // Define your cpu_set bit mask. 
    CPU_ZERO(&my_set);       // Initialize it all to 0, i.e. no CPUs selected. 
    CPU_SET(4, &my_set);     // set the bit that represents core 7. 
    sched_setaffinity(0, sizeof(cpu_set_t), &my_set); // Set affinity of this process to the defined mask, i.e. only 7. 
    status = syscall(SYS_getcpu, &cpu, NULL, NULL);
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s process_nickname\n",argv[0]);
        exit(1);
    }
//    address_from_malloc = (int *)malloc(sizeof(int));
    srand((int)time(NULL)+(int)getpid());
    a = rand();
    fprintf(stderr, "\nProcess %s is using CPU: %d. Value of a is %lf and address of a is %p\n",argv[1], cpu, a, &a);
//    for(i = 0 ; i < 10 ; i++);
    sleep(1);
    fprintf(stderr, "\nProcess %s is using CPU: %d. Value of a is %lf and address of a is %p\n",argv[1], cpu, a, &a);
//    fprintf(stderr, "\naddress_from_malloc in Process %s is %p\n",address_from_malloc);
        sleep(3);
    return 0;
}