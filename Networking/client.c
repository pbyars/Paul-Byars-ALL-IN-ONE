#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 512     // max text line length
#define SERV_PORT 2017  // port
#define LISTENQ 8       // maximum number of client connections

//#include "configuration.h"

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server>");
        exit(1);
    }

    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }

    char *recvline = (char *)calloc(MAXLINE, sizeof(char));
    char sendline[] = "Bring me quote";

    // printf("Socket ID: %d\n", sockfd);

    send(sockfd, sendline, strlen(sendline), 0);

    if (recv(sockfd, recvline, MAXLINE, 0) == 0)
    {
        // ERROR: Server terminated prematurely
        perror("The server terminated prematurely");
        exit(4);
    }
		
	printf("Quote of the Day: %s\n", recvline);

    free(recvline);

    exit(0);
}