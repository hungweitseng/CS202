#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    FILE *fp;
    char line[2048];
    fp = fopen(argv[1],"r");
    memset(line, 0, 2048);
    while(fgets(line, 2048, fp) != NULL)
    {
        fprintf(stderr,"%s\n",line);
        memset(line, 0, 2048);
    }
    return 0;
}