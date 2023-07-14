#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define N 1000


void fillnums();
void printnums(int*);
void bubblesort(int);
void runprocesses();
void merge();
void initnums();
int minptr();

int* nums;
int sortednums[N];
int ptrs[N];
int n,m;
int k;

int main(){
    nums = mmap(NULL, N*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    srand(time(0));

    n = 24; //test
    m = 6; //test

    printf("enter number of random numbers to generate: ");
    scanf("%d", &n);
    printf("enter number of sorting processes: ");
    scanf("%d", &m);
    k = n%m == 0? n/m : n/m+1;

    initnums();  // set all numbers in nums[] to int_max
    fillnums();  // make n random numbers and fill nums[] with them

    printf("\nrandom numbers created:\n");
    printnums(nums);

    runprocesses();

    for (int i = 0; i < m; i++)
    {
        wait(NULL);
    }

    printf("\narray after threads finished bubblesorting:\n");
    printnums(nums);

    merge();
    printf("\narray after merging\n");
    printnums(sortednums);

    return 0;
}

void runprocesses(){
    for (int i = 0; i < m; i++)
    {
        int r = fork();
        if (r == 0){
            bubblesort(i*k);
            exit(0);
        }
    }
    return;
}

void fillnums(){
    for (int i = 0; i < n; i++)
    {
        nums[i] = rand()%10000;
    }

}

void bubblesort(int s){
    int e = s+k-1;
    for (int i = s; i < e; i++)
    {
        for (int j = s; j < e-(i-s); j++)
        {
            if (nums[j] > nums[j+1]){
                int temp = nums[j];
                nums[j] = nums[j+1];
                nums[j+1] = temp;
            }
        }
    }
}

void initnums(){
    for (int i = 0; i < N; i++)
    {
        nums[i] = INT_MAX;
    }
}

void printnums(int* arr){
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(){
    for (int i = 0; i < m; i++)
    {
        ptrs[i] = i*k;
    }

    for (int i = 0; i < n; i++)
    {
        int idx = minptr();
        sortednums[i] = nums[ptrs[idx]];
        nums[ptrs[idx]] = INT_MAX;
        ptrs[idx]++;
    }
}

int minptr(){
    int idx = 0;
    for (int i = 1; i < m; i++)
    {
        if(nums[ptrs[i]] < nums[ptrs[idx]]){
            idx = i;
        }
    }
    return idx;
}