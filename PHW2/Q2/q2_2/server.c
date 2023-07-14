#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>


// in your browser type: http://localhost:8090
// IF error: address in use then change the PORT number
#define PORT 8096
#define capacity 1000

void* serve(void*);
pthread_t threads[capacity];
int sockets[capacity];
int idx = -1;
char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

int main(int argc, char const *argv[])
{
    printf("ON PORT %d", PORT);
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        idx = (idx+1)%capacity;
        sockets[idx] = new_socket;
        // printf("REQUEST NUMBER = %d\n", ++idx);
        pthread_create(&threads[idx], NULL, serve, (void*)(&sockets[idx]));
    }
    return 0;
}

void* serve(void* socketptr){
    int* temp = (int*) socketptr;
    int new_socket = *temp;
    char buffer[30000] = {0};
    read( new_socket , buffer, 30000);
    printf("%s\n",buffer );
    // uncomment following line and connect many clients
    sleep(5);
    write(new_socket , hello , strlen(hello));
    printf("-------------Hello message sent---------------\n");
    close(new_socket);
}