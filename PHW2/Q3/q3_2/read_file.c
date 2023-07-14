#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/wait.h>
#include <time.h>
#include<string.h>

int main(int argc, char **argv){
    char command[500];
    strcpy(command, "/bin/cat ");
    strcat(command, argv[1]);
    
    system(command);
    return 0;
}