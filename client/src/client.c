/*
 * 		Name: client.c
 *      Author: Petr Mateju
 *      Version: 1.0
 *      Copyright: ©2011 Petr Mateju
 *      Description: Client program to comunicate with server
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
	printf("This is the client program\n");

	int port = atoi(argv[2]);
	int sockfd;
	int len, rc;
	struct sockaddr_in address;
	int result;
	char name[16];
	char message[256];

	//Create socket for client.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf(stderr, "Socket for client didn't create.\n");
		return -1;
	}

	//Name the socket as agreed with server.
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(argv[1]); //"127.0.0.1");
	address.sin_port = htons(port);
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *) &address, len);
	if (result == -1) {
		perror("Error has occurred");
		exit(-1);
	}

	//sending client's name
	printf("Type a name, which you want use: ");
	scanf("%s", &name);

	rc = send(sockfd, name, 16, 0);
	if (rc == 16) {
		printf("Sended name: ");
		int i = 0;
		for (i = 0; i < strlen(name); i++) {
			printf("%c", name[i]);
		}
		printf(", length is: %d\n", strlen(name));
	} else {
		fprintf(stderr, "Error: Name didn't send");
	}

	//sending messages
	while (1) {
		printf("Type a message, which you want send: ");
		scanf("%s", &message);
		//when typed "exit", quit the program
		if (message[0] == 'e' && message[1] == 'x' && message[2] == 'i'
				&& message[3] == 't') {
			printf("Client exiting");
			send(sockfd, "@exit@", 6, 0);
			close(sockfd);
			exit(0);
		} else {
			rc = send(sockfd, message, 256, 0);
			if (rc == 256) {
				printf("Sended message: ");
				int i = 0;
				for (i = 0; i < strlen(message); i++) {
					printf("%c", message[i]);
				}
				printf(", length is: %d\n", strlen(message));
			} else {
				fprintf(stderr, "Error: Message didn't send");
			}
		}

	};
}
