#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    unsigned buffL = 1024;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[buffL];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname:port\n", argv[0]);
        exit(0);
    }
    char ip[32];
    int count = 0;
    while (argv[1][count] != ':') {
        ip[count] = argv[1][count];
        count++;
    }
    ip[count] = '\0';
    count++;

    portno = atoi(argv[1] + count);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(ip);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server->h_name);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    bzero(buffer, buffL);
    unsigned buffC = 0;
    for (int i = 2; i < argc; i++) {
        unsigned tmp = 0;
        if (buffC < buffL && i > 2) {
            buffer[buffC] = ' ';
            buffC++;
        }
        while (buffC < buffL && argv[i][tmp] != '\0') {
            buffer[buffC] = argv[i][tmp];
            buffC++;
            tmp++;
        }
    }
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    return 0;
}
