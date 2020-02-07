#define _GNU_SOURCE  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sched.h>
#include <sys/syscall.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int child_pid;
    fork();
    fprintf(stderr,"moo\n");
    fork();
    fprintf(stderr,"oink\n");
    fork();
    fprintf(stderr,"baa\n");
    return 0;
}
