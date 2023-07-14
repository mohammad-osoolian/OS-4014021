//ref: pesudo code from slides and geekforgeeks;
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include<stdlib.h>
#include <stdio.h>

#define N 5
#define TH 2
#define HU 1
#define EA 0

void* phil_thread(void*);
void run_phils();
void eat(int);
void get_fork(int);
void free_fork(int);
void wait_phils();
void init();

int status[N];
pthread_t thread_id[N];
int p[N];
sem_t mutex, S[N];


void run_phils(){
    for (int i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL, phil_thread, &p[i]);
	}
}

void free_fork(int phnum)
{
	sem_wait(&mutex);

	status[phnum] = TH;
	printf("P%d FREE forks: %d - %d\n", phnum + 1, (phnum-1+N)%N + 1, phnum + 1);
    
    int leftph = (phnum-1+N)%N;
    int rightph = (phnum+1)%N;
	eat(leftph);
	eat(rightph);
	sem_post(&mutex);
}

void wait_phils(){
    for (int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}

void get_fork(int phnum){
	sem_wait(&mutex);
	status[phnum] = HU;
	printf("P%d status = HU\n", phnum + 1);
    
	eat(phnum);
	sem_post(&mutex);
	sem_wait(&S[phnum]);
	sleep(1);
}

void* phil_thread(void* num){ 
    int phnum = *((int*)num);
	while (1) {
        printf("P%d status = TH\n", phnum + 1);
		sleep(1);
		get_fork(phnum);
		sleep(0);
		free_fork(phnum);
	}
}

void init(){
    sem_init(&mutex, 0, 1);
	for (int i = 0; i < N; i++)
		sem_init(&S[i], 0, 0);
    for (int i = 0; i < N; i++)
        p[i] = i;
}
void eat(int phnum)
{
    int rightph = (phnum+1)%N;
    int leftph = (phnum-1+N)%N;

	if (status[phnum] == HU && status[leftph] != EA && status[rightph] != EA) {
		status[phnum] = EA;
		sleep(2);
        printf("P%d GET forks: %d - %d\n", phnum + 1, (phnum-1+N)%N + 1, phnum + 1);
		printf("P%d status = EA\n", phnum + 1);
		
        sem_post(&S[phnum]);
	}
}

int main()
{
    printf("simulation started!\n");
    printf("number of philosophers = %d (Could be changed)\n", N);
    init();
	run_phils();
    wait_phils();
    printf("simulation finished!\n");

    return 0;
}
