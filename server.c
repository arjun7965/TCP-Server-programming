#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8008
#include "client_server_app.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* client_handler(void *arg);

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++) {
        // Last i elements are already in place  
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] < arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

int server_fibo(int socket)
{
    int n, sum, a, b, i;
    n = my_recieve(socket);

    a = 0;
    b = 1;
    for(i = 2; i <= n; i++) {
        sum = a + b;
        a = b;
        b = sum;
    }
    my_send(sum, socket);
}

int server_sort(int socket)
{
    int	array[100], i, n;

    /*
     * ToDo : replace static allocation of array by malloc
     * keeping maximum size of array as 100 and handling it at client side
    */

    // revieve numebers
    n = my_recieve(socket);
    for(i = 0; i < n; i++) {
        array[i] = my_recieve(socket);
    }
    // sort
    bubbleSort(array, n);

    // send numbers
    for(i = 0; i < n; i++) {
        my_send(array[i], socket);
    }
}

int server_rand(int socket)
{
    int a, b, n;
    a = my_recieve(socket);
    b = my_recieve(socket);
		
    n = rand();
    while(!(n>=a && n<=b)) {
        n = rand();
    }

    my_send(n, socket);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread, count, num;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    struct message msg;
    int cmd_num;
    pthread_t threadID;

    printf("\nWaiting for client..");
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8008
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        	perror("setsockopt(SO_REUSEADDR) failed");
        	exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8008
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
       	perror("bind failed");
       	exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
       	perror("listen");
       	exit(EXIT_FAILURE);
    }
    while(1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        } else {
            send(new_socket, hello, strlen(hello), 0);
            valread = read(new_socket, buffer, 1024);
	    printf("\n%s\n", buffer);
	}
        pthread_create(&threadID, NULL, client_handler, (void *) &new_socket);
    }
	return 0;
}

void* client_handler(void *arg)
{
    int new_socket, count, cmd_num, valread;
    new_socket = *((int *) arg);
    char buffer[1024] = {0};

    /* 
     * ToDo : move this code in new thread for each connection
    */

    do {
        cmd_num = my_recieve(new_socket);
        switch(cmd_num) {
            case 1:
                server_fibo(new_socket);
                break;
            case 2:
                server_sort(new_socket);
                break;
            case 3:
                server_rand(new_socket);
                break;
            case 4:
                valread = read(new_socket, buffer, 1024);
                printf("%s\n",buffer);
                close(new_socket);
                break;
            default:
                printf("ERROR: Recieved unknown command\n");
        }
    } while (cmd_num != 4);
    return 0;
}
