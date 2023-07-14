#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
const int clk = 1;
int alarmm;
int timee = 0;
bool exitt = false;
bool showtime = true;

void* tick(){
    while(!exitt){
        sleep(clk);
        pthread_mutex_lock(&mutex);
        timee++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        if(showtime)
            printf("time: %d\n", timee);
    }
}

void* checkalarmm(){
    bool wakeup = false;
    while(!wakeup){
        pthread_mutex_lock(&mutex);
        while(timee < alarmm){
            pthread_cond_wait(&cond, &mutex);
        }
        showtime = false;
        printf("Alarm!\n");
        pthread_mutex_unlock(&mutex);
        printf("Enter 0 to wakeup or snooze timee: ");
        int temp;
        scanf("%d", &temp);
        if(temp == 0)
            wakeup = true;
        else{
            alarmm = timee + temp;
            printf("new alarm set for %d\n", alarmm);
            showtime = true;
        }
    }
    printf("Waked up!\n");
}

int main(){
    printf("Enter alarm time: ");
    scanf("%d", &alarmm);

    pthread_t tickid, checkid;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&tickid, NULL, tick, NULL);
    pthread_create(&checkid, NULL, checkalarmm, NULL);
    pthread_join(checkid, NULL);
    exitt = true;
    pthread_join(tickid, NULL);
    // pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&cond);
    return 0;
}