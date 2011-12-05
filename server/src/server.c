/*
 * 		Name: server.c
 *      Author: Petr Mateju
 *      Version: 1.0
 *      Copyright: ©2011 Petr Mateju
 *      Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

//Declaring process variables.
int port;
int server_sockfd, client_sockfd;
int server_len;
int rc;
unsigned client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;

void * service(void *nothing) {
	char tmp[256];
	char name[16];
	char message[256];
	int sending = 1;

	//receiving client's name
	recv(client_sockfd, &tmp, 16, 0);
	if (tmp[strlen(tmp) - 1] == '0') {
		int i = 0;
		while (tmp[i] != '0') {
			name[i] = tmp[i];
			i++;
		}
		printf("Received client's name: ");
		for (i = 0; i < strlen(name); i++) {
			printf("%c", name[i]);
		}
		printf("\n");
	} else {
		printf("Client's name was sended in wrong format!");
	}

	//receiving messages
	while (message[0] != '@' && message[1] != 'e' && message[2] != 'x'
			&& message[3] != 'i' && message[4] != 't' && message[5] != '@') {
		recv(client_sockfd, &message, 256, 0);
		//close(client_sockfd);
		int doubledot = 0;
		int i = 0;
		for (i = 0; i < strlen(message); i++) {
			if (message[i] == ':') {
				doubledot++;
			}
		}
		if (doubledot == 0) {
			printf("Received message is in wrong format!\n");
		} else {
			printf("Received message: ");
			for (i = 0; i < strlen(message); i++) {
				printf("%c", message[i]);
			}
			printf("\n");
		}
	}
	printf("Client has disconnected\n");
}

int main(int argc, char *argv[]) {

	port = atoi(argv[1]);
	int numberOfThreads = 0;
	pthread_t threads[32];
	int accepting = 1;

	//Remove any old socket and create an unnamed socket for the server.
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);
	server_address.sin_port = htons(port);
	server_len = sizeof(server_address);

	rc = bind(server_sockfd, (struct sockaddr *) &server_address, server_len);
	if (rc != 0) {
		fprintf(stderr, "Error in bind, rc = %d\n", rc);
		exit(0);
	}

	//Create a connection queue and wait for clients
	rc = listen(server_sockfd, 5);
	if (rc != 0) {
		fprintf(stderr, "Error in listen, rc = %d\n", rc);
	}

	printf("Server started, IP = 127.0.0.1, port = %s\n", argv[1]);

	while (accepting) {
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,
				(struct sockaddr *) &client_address, &client_len);
		printf("client connected = %d\n", client_sockfd);
		pthread_create(&threads[numberOfThreads], NULL, service, NULL); //Create thread
		numberOfThreads++;
	}
	printf("Server exiting\n");

}
