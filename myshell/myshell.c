#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp)
{
    if (fgets(buffer, buflen, fp) != 0)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n')
            buffer[len-1] = '\0';
        return buffer;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int child_pid, status;
    char cmd[1024];
    memset(cmd, 0 , 1024);
    fprintf(stderr,"CS202-myshell$ ");
    while(fgets_wrapper(cmd,1024,stdin))
    {
         if(strcmp("exit",cmd)==0)
             exit(1);
         child_pid = fork();
         if (child_pid == 0) 
         {
             execvp(cmd,NULL);
         }
         else
         {
             memset(cmd, 0 , 1024);
             fprintf(stderr,"\nCS202-myshell$ ");
         }
    }
    return 0;
}
