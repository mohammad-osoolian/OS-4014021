#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv){
    int r = fork();
    if (r==0)
    {   
        char* catpath = "/bin/cat";
        char* args[] = {catpath , argv[1], NULL};
        execvp(catpath, args);
        exit(0);
    }

    wait(NULL);
    return 0;
}