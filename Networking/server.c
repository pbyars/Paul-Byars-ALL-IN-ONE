#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXLINE 512     // max text line length
#define SERV_PORT 2017  // port
#define LISTENQ 8       // maximum number of client connections

//#include "configuration.h"

char *requestHandler();

int main(int argc, char **argv)
{
    srand(time(NULL));
    // fd: file description
    int listenfd, connfd, n;
    socklen_t clilen;

    struct sockaddr_in cliaddr, servaddr;

    //creation of the socket
    // socket()
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // bind()
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // listen()
    listen(listenfd, LISTENQ);

    //printf("Server is running on port %d...\n", SERV_PORT);
    //printf("Waiting for connections...\n");

    char *buf = (char *)calloc(MAXLINE, sizeof(char));
    char* resp;

    for (;;)
    {

        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        //printf("Received request from port %d...\n", cliaddr.sin_port);

        while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
        {
            resp = requestHandler();
            //printf("Response to the client: %s\n", resp);
            send(connfd, resp, MAXLINE, 0);
        }

        if (n < 0)
        {
            perror("Read error");
            exit(1);
        }
        close(connfd);
    }
    //close listening socket
    close(listenfd);
}

char *requestHandler()
{
    char *result = (char *)calloc(MAXLINE, sizeof(char));
    const char *quote[] = {
        "Respect is a rational process. - Dr. McCoy",
        "Change is the essential process of all existence. - Mr. Spock",
        "Genius doesn’t work on an assembly line basis. - Capt. Kirk",
        "Insufficient facts always invite danger. - Mr. Spock",
        "There is a way out of any cage. - Capt. Pike",
        "Vulcans never bluff. - Mr. Spock",
        "Peace was the way. - Capt. Kirk",
        "Not one hundred percent efficient, of course... but nothing ever is. - Capt. Kirk",
        "There are always alternatives. - Mr. Spock"
    };

    int randomIndex = 0 + rand() % (8 + 1 - 0);
    strcpy(result, quote[randomIndex]);

    return result;
}