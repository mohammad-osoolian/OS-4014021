#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int find_index(char*);

char attendies[10][100];
int profits[10] = {0};
int n;

int main(){
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char name[100];
        scanf("%s",name);
        strcpy(attendies[i], name);   
    }

    for (int i = 0; i < n; i++)
    {
        char name[100];
        int money;
        int m;
        int index;
        scanf("%s", name);
        scanf("%d %d", &money, &m);
        if (m == 0)
            continue;
        
        int otherprofit = money/m;
        index = find_index(name);

        for (int j = 0; j < m; j++)
        {
            char other[100];
            scanf("%s", other);
            int otherindex = find_index(other);
            profits[otherindex] += otherprofit;
            profits[index] -= otherprofit;
        }
    }

    for (int i = 0; i < n; i++)
    {
        printf("%s %d\n", attendies[i], profits[i]);
    }
    

    return 0;
}

int find_index(char* name){
    for (int i = 0; i < n; i++)
    {
        if (strcmp(name, attendies[i]) == 0)
        {
            return i;
        }
        
    }

    return -1;
}