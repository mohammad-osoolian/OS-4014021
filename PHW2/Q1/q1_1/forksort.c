#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
// #include <sys/wait.h>
#include <time.h>


#define N 1000


void fillnums();
void printnums(int*);
void bubblesort(int, int);
void merge();
void initnums();
int minptr();

int nums[N];
int sortednums[N];
int rs[N];
int ptrs[N];
int n,m;
int k;

int main(){
    srand(time(0));

    n = 24; //test
    m = 6; //test

    printf("enter n: ");
    scanf("%d", &n);
    printf("enter m: ");
    scanf("%d", &m);
    k = n%m == 0? n/m : n/m+1;
    
    initnums();
    fillnums();
    printf("random numbers created:\n");
    printnums(nums);
    
    for (int i = 0; i < m; i++)
    {
        rs[i] = vfork();
        if (rs[i] == 0){
            // printf("I'm child process idx=[%d - %d]\n", i*k, i*k + k -1);
            bubblesort(i*k, i*k + k -1);
            // printnums(nums);
            exit(0);
        }
    }
    printf("\narray after child processes finished bubblesorting:\n");
    printnums(nums);
    
    merge();

    printf("\narray after merging\n");
    printnums(sortednums);


    return 0;
}

void fillnums(){
    for (int i = 0; i < n; i++)
    {
        nums[i] = rand()%10000;
    }
    
}

void bubblesort(int s, int e){
    for (int i = s; i < e; i++)
    {
        for (int j = s; j < e-(i-s); j++)
        {
            // printf("check %d, %d\n", j, j+1);    
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