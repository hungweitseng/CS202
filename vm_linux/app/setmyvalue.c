#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <mymodule/mymodule.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
     struct mymodule_cmd cmd;
     int devfd;
     devfd = open("/dev/mymodule",O_RDWR);
     cmd.value = 1;
     fprintf(stderr, "My value for process %d is %d\n",getpid(),ioctl(devfd, MYMODULE_IOCTL_GET, &cmd));
     sleep(1);
     cmd.value = getpid();
     fprintf(stderr, "Set my value by process %d as %d\n",getpid(),ioctl(devfd, MYMODULE_IOCTL_SET, &cmd));
     return 0;
}

