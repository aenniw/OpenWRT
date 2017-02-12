#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage :: port DIR\n");
        return 1;
    }
    DIR *dir = opendir(argv[2]);
    if (dir == NULL) {
        fprintf(stderr, "Directory does not exist.\n");
        return 1;
    }

    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        fprintf(stderr, "Could not create socket\n");
    }
    fprintf(stdout, "Socket created\n");

    int port = 0, ext = 1, count = strlen(argv[1]) - 1;
    while (argv[1][count] != '\0') {
        port += (argv[1][count] - 48) * ext;
        ext *= 10;
        count--;
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        //print the error message
        fprintf(stderr, "bind failed. Error\n");
        return 1;
    }
    fprintf(stdout, "Bind done.\n");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);
    unsigned int running = 1;
    while (running) {
        fprintf(stdout, "Waiting for incoming connections...\n");

        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
        if (client_sock < 0) {
            fprintf(stderr, "accept failed\n");
            close(client_sock);
            return 1;
        }
        fprintf(stdout, "Connection accepted\n");
        //Receive a message from client
        while (recv(client_sock, &read_size, sizeof(int), 0) > 0) {
            char *client_message = NULL;
            client_message = calloc(read_size, sizeof(char));
            if (recv(client_sock, client_message, read_size, 0) > 0) {
                //Send the message back to client
                fprintf(stdout, "%s\n", client_message);
                fflush(stdout);
                struct dirent *pDirent;
                char *scriptN = NULL;
                int counter = 0;
                while (client_message[counter] != ' ' && client_message[counter] != '\0') {
                    counter++;
                }
                counter++;
                scriptN = malloc(sizeof(char) * counter);
                counter--;
                while (counter >= 0) {
                    scriptN[counter] = client_message[counter];
                    counter--;
                }
                int returnVaulue = 1;
                while ((pDirent = readdir(dir)) != NULL) {
                    if (strcmp(scriptN, pDirent->d_name) == 0) {
                        char *tmp = malloc(sizeof(char) * (2 + strlen(client_message) + strlen(argv[2])));
                        strcpy(tmp, "sh ");
                        if (argv[2][strlen(argv[2]) - 1] != '/') {
                            strcat(tmp, argv[2]);
                            strcat(tmp, "/");
                            strcat(tmp, client_message);
                            returnVaulue = system(tmp);
                        } else {
                            strcat(tmp, argv[2]);
                            strcat(tmp, client_message);
                            returnVaulue = system(tmp);
                        }
                        free(tmp);
                        break;
                    }
                }

                int s = 15 * sizeof(char);
                if (returnVaulue == 0) {
                    send(client_sock, &s, sizeof(int), 0);
                    send(client_sock, "Run status : 0", s, 0);
                } else {
                    send(client_sock, &s, sizeof(int), 0);
                    send(client_sock, "Run status : 1", s, 0);
                }
                rewinddir(dir);
                free(scriptN);
                sleep(1);

            } else fprintf(stdout, "ERROR recieveing data\n");
            free(client_message);
        }
        fprintf(stdout, "Client disconeceted.\n");
    }
    fprintf(stdout, "Stoping server !\n");
    //close connection
    closedir(dir);
    close(client_sock);
    return 0;
}
