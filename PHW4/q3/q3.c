#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MAX_NO_OF_PRODUCERS
#define LOG_LENGTH 100

int logpipe[2];
int NO_OF_PRODUCERS;
const char* FILE_ADDRESS = "logs.txt";

void producer(int index){
    printf("start producer %d\n", index);
    int n = 1;
    while(1){
        sleep((double)(rand()%1000)/100.0);
        char log[LOG_LENGTH] = {0};
        sprintf(log, "log: P%d - LOG NUMBER %d", index, n);
        write(logpipe[1], log, LOG_LENGTH);
        n++;
    }
}

void initproducers(){
    for(int i = 0; i < NO_OF_PRODUCERS; i++){
        int r = fork();
        if(r == 0){
            srand(time(NULL)+NO_OF_PRODUCERS*i);
            producer(i);
            exit(0);
        }
    }
}

int main(){
    printf("----------------------------------------------------\n"
            "In this program, given number of producers will be created and they will send logs to the logger.\n"
            "Every producer will repeat generating logs and waiting for a random time from 0 to 10 seconds "
            "and then generates next log.\n\n"
            "Logs are generated in this format:\nlog: P{index of producer} - LOG NUMBER {the number of log from this producer}\n\n"
            "The Logger writes logs in 'logs.txt'.\n\n"
            "The program will terminate when you press Ctrl+C\n"
            "----------------------------------------------------\n\n");
    printf("Enter the number of producers:  ");
    scanf("%d", &NO_OF_PRODUCERS);
    printf("\n\n");
    
    FILE *fp = fopen(FILE_ADDRESS, "w");
    pipe(logpipe);
    printf("Logger started\n");
    initproducers();


    while (1)
    {
        char buffer[LOG_LENGTH] = {0};
        read(logpipe[0], buffer, LOG_LENGTH);
        printf("\n%s", buffer);
        fprintf(fp, "%s\n", buffer);
        fflush(fp);
    }

    fclose(fp);
    
    return 0;
}