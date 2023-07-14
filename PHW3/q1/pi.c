#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define N 10000000000
#define M 1000000

long n = 0, m = 0, k =0;
double results[M];
int inputs[M];
pthread_t threads[M];

int powwnegone(int n){
    return n%2 == 0 ? 1 : -1;
}

int min(int a, int b){
    return a < b ? a : b;
}

double formula(int n){
    double r = 0;
    r = 4*((double)powwnegone(n) / (double)(2 * n + 1));
    return r;
}

void* calculate(void* arg){
    int num = *((int*)arg);
    // printf("thread started\n");
    for(int i = num*k; i < min((num+1)*k, n); i++){
        results[num] += formula(i);
        // printf("i=%d\n", i);
    }
    // printf("thread finished\n");
}

void init(){
    for(int i = 0; i < m; i++){
        inputs[i] = i;
        results[i] = 0;
    }
}

long myceil(double x){
    long y = (long)x;
    if(x - y > 0){
        return y + 1;
    }
    return y;
}

int main(){
    printf("n: the number of elements of sequence | MAX=1e9\n");
    printf("m: the number of threads to be run | MAX=1e6\n\n");
    printf("input n and m: ");
    scanf("%ld %ld", &n, &m);
    k = myceil((double)(n)/(double)(m));
    // printf("k = %ld\n", k);

    init();
    for(int i = 0; i < m; i++){
        pthread_create(&threads[i], NULL, calculate, (void*)(&inputs[i]));
    }

    for(int i = 0; i < m; i++){
        pthread_join(threads[i], NULL);
    }

    double pi = 0;
    for(int i = 0; i < m; i++){
        pi += results[i];
    }

    // printf("number of threads: %ld\n", m);
    // printf("number of elements of sequence: %ld\n", n);
    printf("pi = %lf\n", pi);

    return 0;
}