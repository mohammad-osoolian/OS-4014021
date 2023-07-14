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

#define MAX_NO_OF_PIPES 50
#define MAX_NO_OF_PRIMES 1000
#define PIPE_EXIT_CODE -999
#define DEB if(DEBUG)

struct prime{
    int value;
    int counter;
};

int DEBUG = 1;
int NO_OF_PRIMES;
int NO_OF_PIPES;

int primenums[MAX_NO_OF_PRIMES+10];
struct prime primes[MAX_NO_OF_PRIMES+10];
struct prime recievedprimes[MAX_NO_OF_PRIMES+10];
int pipes[MAX_NO_OF_PIPES+1][2];  //[[#<-#][#<-#][#<-#][#<-#][#<-#][#<-#][#<-#][#<-#]]
int gpipe[2];



int isprime(int n){
    int i;
    for(i=2; i<n; i++){
        if(n%i == 0){
            return 0;
        }
    }
    return 1;
}

void findprimes(){
    int i = 2;
    int j = 0;
    while(j<NO_OF_PRIMES){
        if(isprime(i)){
            primenums[j] = i;
            j++;
        }
        i++;
    }
}


void makestructs(){
    int i;
    for(i=0; i<NO_OF_PRIMES; i++){
        primes[i].value = primenums[i];
        primes[i].counter = primes[i].value;
    }
}

void PU(int read_pipe, int write_pipe, int index){
    // printf("PU%d started\n", index);
    while(1){
        struct prime input;
        read(read_pipe, &input, sizeof(struct prime));
        if(input.counter == PIPE_EXIT_CODE){
            // printf("PU%d exiting\n", index);
            write(write_pipe, &input, sizeof(struct prime));
            return;
        }
        DEB{printf("PU%d GOT %d - %d\n",index, input.value, input.counter);}
        input.value += index;
        input.counter-=1;
        if(input.counter == 0) 
            write(gpipe[1], &input, sizeof(struct prime));
        else 
            write(write_pipe, &input, sizeof(struct prime));
    }
}

void initpipes(){
    for(int i=0; i<NO_OF_PIPES; i++)
        pipe(pipes[i]);
    pipe(gpipe);

    for(int i=0; i<NO_OF_PIPES; i++){
        int r = fork();
        if(r == 0){
            PU(pipes[i][0], pipes[(i+1)%NO_OF_PIPES][1], i);
            exit(0);
        }
    }
}

void killpipes(){
    struct prime exitcode;
    exitcode.value = PIPE_EXIT_CODE;
    exitcode.counter = PIPE_EXIT_CODE;
    write(pipes[0][1], &exitcode, sizeof(struct prime));
}


int main(){
    printf("------------------------------------------------------------------------------------\n"
            "DEBUG mode: \nin debug mode, every process will print out the values it sends and receives.\n"
            "Also, the parent process will wait 1 second between every item it sends to th pipe so you can see "
            "how pipe is working.\n\n"
            "NO_OF_PRIMES: \nThis is the number of primes that will be sent through the pipe.\n\n"
            "NO_OF_PIPES: \nThis is the number of processes that will be used to recieve the primes.\n"
            "------------------------------------------------------------------------------------\n\n");
    printf("DEBUG mode (1/0): ");
    scanf("%d", &DEBUG);
    printf("NO_OF_PRIMES (MAX=%d): ", MAX_NO_OF_PRIMES);
    scanf("%d", &NO_OF_PRIMES);
    printf("NO_OF_PIPES (MAX=%d): ", MAX_NO_OF_PIPES);
    scanf("%d", &NO_OF_PIPES);

    findprimes();
    makestructs();
    initpipes();

    int r = fork();
    if(r == 0){
        for(int i=0; i<NO_OF_PRIMES; i++){
            DEB{printf("\nSent: %d - %d\n", primes[i].value, primes[i].counter);}
            write(pipes[primes[i].value%NO_OF_PIPES][1], &primes[i], sizeof(struct prime));
            DEB{sleep(1);}
        }
        exit(0);
    }
    else{
        for (int i = 0; i < NO_OF_PRIMES; i++)
        {
            struct prime input;
            read(gpipe[0], &input, sizeof(struct prime));
            DEB{printf("Received: %d - %d\n", input.value, input.counter);}
            recievedprimes[i] = input;
        }
    }

    killpipes();

    printf("\n-------------------------- Sent Items -----------------------------\n");
    for(int i=0; i<NO_OF_PRIMES; i++){
        printf("%d - %d\n", primes[i].value, primes[i].counter);
    }
    printf("\n-------------------------- Recieved Items -------------------------\n");
    for(int i=0; i<NO_OF_PRIMES; i++){
        printf("%d - %d\n", recievedprimes[i].value, recievedprimes[i].counter);
    }

    return 0;
}

