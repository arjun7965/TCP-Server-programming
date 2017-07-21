#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#include "client_server_app.h"

int fibo_client(int socket) {
	int num, n;
	printf("\nEnter how many numbers");
	scanf("%d", &num);
    	my_send(num, socket);
	n = my_recieve(socket);
	printf("\nAnswer:");
	printf(" %d", n);
}

void sort_client(int socket) {

        int     array[100], i, n;

        /*
         * ToDo : replace static allocation of array by malloc
         */

	printf("\nEnter maximum number of element:\n");
	scanf("%d", &n);
    	my_send(n, socket);

	printf("\nEnter elements\n");
	// send numbers
        for(i = 0; i < n; i++) {
		scanf("%d", &array[i]);
                my_send(array[i], socket);
        }

	printf("\nSorted elements\n");
        // revieve numebers
        for(i = 0; i < n; i++) {
                array[i] = my_recieve(socket);
		printf(" %d", array[i]);
        }
}

void random_client(int socket) {
	int n;
	printf("\nEnter lower limit number:");
	scanf("%d", &n);
	my_send(n, socket);
	printf("\nEnter upper limit number:");
	scanf("%d", &n);
	my_send(n, socket);
	n = my_recieve(socket);
	printf("\n Random number: %d", n);
}
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    struct message msg;
    char cmd[50];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // ToDo: Accept IP from user
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
	
	/*
	 * ToDo: replace command parsing using strtok() funtion
	 */

  	printf("\nYour connected to server. Please enter help for more information:");
	do {
		printf("\n# ");
		scanf("%s", cmd);
		if (strcmp(cmd, "exit") == 0) {
    			my_send(4, sock);
		} else if (strcmp(cmd, "fibonacci") == 0) {
    			my_send(1, sock);
			fibo_client(sock);
		} else if (strcmp(cmd, "sort") == 0) {
    			my_send(2, sock);
			sort_client(sock);
		} else if (strcmp(cmd, "random") == 0) {
    			my_send(3, sock);
			random_client(sock);
		} else if (strcmp(cmd, "help") == 0) {
			printf("\n1.fibonacci\n2.sort\n3.random\n4.exit\n");
		}
	} while (strcmp(cmd, "exit") != 0);
    return 0;
}
