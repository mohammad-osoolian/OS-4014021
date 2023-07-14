#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 1000

int n,m,limit;
long matrix[N][N] = {{1,2,3},{4,5,6},{7,8,9}};
long next[N][N];
sem_t freel, resetl;
pthread_t threads[N*N];
int inputs[N*N];
int freec, resetc;


void printmatrix(){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++)
            printf("%ld ", next[i][j]);
        printf("\n");
    }
}

void* cal_element(void* arg){
    for (int t = 0; t < m-1; t++)
    {
        int temp = *((int*)arg);
        int row = temp / n;
        int col = temp % n;
        long sum = 0;
        for (int i = 0; i < n; i++)
            sum += matrix[row][i] * next[i][col];
        
        // printmatrix();
        // printf("debug\n");
        sem_wait(&freel);
        if (freec == 1)
            resetc = limit;
        freec--;
        sem_post(&freel);
        while (freec)
            continue;
        
        // printf("finish\n");
        next[row][col] = sum;
        
        sem_wait(&resetl);
        if(resetc == 1)
            freec = limit;
        resetc--;
        sem_post(&resetl);
        while (resetc)
            continue;

    }
    return NULL;
}

void init(){
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            next[i][j] = matrix[i][j];
    for(int i = 0; i < n * n; i++)
        inputs[i] = i;
    
    freec = limit;
    resetc = limit;
}


int main(){
    printf("n: the length of the matrix | MAX=1000 (could be changed)\n");
    printf("m: power of the matrix | depends on the input matrix (final ans must not be out of int range)\n");

    int q;
    printf("enter 1 to  run test matrix, 0 to input your own: ");
    scanf("%d", &q);
    if (q == 0)
    {
        printf("enter n, m: ");
        scanf("%d %d", &n, &m);
        printf("enter the matrix: \n");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%ld", &matrix[i][j]);
    }
    else
    {
        n = 3;
        m = 3;
        printf("n = %d, m = %d\n", n, m);
        printf("test matrix: \n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                printf("%ld ", matrix[i][j]);
            printf("\n");
        }
    }

    limit = n * n;
    init();
    sem_init(&freel, 0, 1);
    sem_init(&resetl, 0, 1);

    for(int i = 0; i < n * n; i++)
        pthread_create(&threads[i], NULL, cal_element, (void*)&inputs[i]);
    
    for(int i = 0; i < n * n; i++)
        pthread_join(threads[i], NULL);
    
    printmatrix();

    
    return 0;
}