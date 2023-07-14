#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int profits[100] = {0};
int n;

int main(){
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &profits[i]);
    }

    int max = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            int sum = 0;
            for (int k = i; k <= j; k++)
            {
                sum += profits[k];
            }
            
            if(sum > max)
                max = sum;
        }
    }
    
    printf("%d", max);
    return 0;
}