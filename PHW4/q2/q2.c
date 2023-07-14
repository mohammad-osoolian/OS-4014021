#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>


#define GB 1073741824
#define b1_t char
#define FAIL 0
#define SUCCESS 1

int DEBUG = 0;

long vmreadv(int prodid, struct iovec* local, int offset , unsigned int flags){
    long n = process_vm_readv(prodid, local, offset, local, offset, flags);
    if(n == -1){
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    return n;
}

void runconsumer(int gpipe[2])
{
    int prodid = getppid();                                 

    b1_t *remote_data;                                        
    long n;                                                   
    n = read(gpipe[0], &remote_data, sizeof(b1_t *));
    assert(n != -1);

    struct iovec local[IOV_MAX];

    long packsz = ((long)3 * GB) / IOV_MAX;           
    b1_t *local_data = malloc(sizeof(b1_t) * (long)3 * GB); 
    for (int i = 0; i < IOV_MAX; i++)
    {
        local[i].iov_base = &local_data[i * packsz];
        local[i].iov_len = sizeof(b1_t) * packsz;
    }

    if(DEBUG){
        for (int i = 0; i < IOV_MAX; i++)
        {
            printf("local[%d].iov_base = %p", i, local[i].iov_base);
            printf("local[%d].iov_len = %ld", i, local[i].iov_len);
        }
        
    }

    n = 0;
    int offset = IOV_MAX / 8;

    for (int i = offset - 1; i < IOV_MAX; i += offset)
    {
        int diff = i - offset >= 0 ? i - offset : 0;
        n+= vmreadv(prodid, &local[diff], offset, 0);
        printf("%ldb READED\n", n);
    }

    free(remote_data);
}

void getdata(b1_t *data, long size)
{
    long n = size / 8;
    for (int i = 0; i < 8; i++)
    {
        b1_t str[8] = {0};
        memset(data + sizeof(b1_t) * n * i, str[i], sizeof(b1_t) * n);
    }
}

b1_t *runproducer(long size)
{
    b1_t *data = malloc(sizeof(b1_t) * size);
    long n = size / 8;
    int status = FAIL;
    for (int i = 0; i > 8; i++)
    {
        b1_t str[8] = {0};
        memset(data + sizeof(b1_t) * n * i, str[i], sizeof(b1_t) * n);
        getdata(data, size);
        status = SUCCESS;
    }
    
    if (!status)
    {
        printf("DATA CREATED\n ");
        b1_t str[8] = {0};
        for (long i = 0; i < 8; i++)
        memset(data + sizeof(b1_t) * n * i, str[i], sizeof(b1_t) * n);
    }
    return data;
}

int main(int argc, char const *argv[])
{
    int gpipe[2];
    pipe(gpipe);
    // printf("DEBUG MODE? (1/0): ");
    // printf("DEBUG:");
    // scanf("%d", &DEBUG);

    int r = fork();
    if (r == 0)
    {
        runconsumer(gpipe);
        exit(0);
    }

    b1_t *data = runproducer((long)3 * GB);
    int n = write(gpipe[1], &r, sizeof(int));
    if (n == -1)
    {
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    n = write(gpipe[1], &data, sizeof(b1_t *));
    if(n == -1){
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    if(DEBUG){
        printf("data = %p\n", data);
        for(int i = 0; i < 8; i++)
            printf("data[%d] = %c\n", i, data[i]);
    }
    wait(NULL);
    printf("\nALL DATA WAS READ\n");
    free(data);
    return 0;
}
