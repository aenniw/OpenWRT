#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "base64.h"

#ifndef DEBUG
#define CMD(cmd)    cmd
#else
#define CMD(cmd)    "sudo " cmd
#endif

const char bind_cmd[] = CMD("usbip-overlay bind -b "),
        unbind_cmd[] = CMD("usbip-overlay unbind -b "),
        list_cmd[] = CMD("usbip-overlay list -l 2>&1"),
        help[] = "Usage\t [binary] [port] [?user:pass]";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stdout, "%s\n", help);
        return 0;
    }
    char *credentials = argc >= 3 ? base64_encode(argv[2], sizeof(char) * strlen(argv[2])) : NULL;
    int create_socket = 0, client_socket = 0;
    struct sockaddr_in address = {.sin_family =AF_INET, .sin_addr.s_addr =INADDR_ANY, .sin_port=htons(
            (uint16_t) atoi(argv[1]))};
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ||
        bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
        return 1;
    }
    fprintf(stdout, "The socket was created and binded to port %d\n", atoi(argv[1]));
    socklen_t addrlen;
    while (1) {
        if (listen(create_socket, 10) < 0 ||
            (client_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
            break;
        }
        char *auth = NULL, *path = NULL, *msg = NULL;
        for (char *line = readLine(client_socket); line != NULL; release(line), line = readLine(client_socket)) {
            if (strstr(line, "GET") && find('/', line) >= 2) {
                char *line_ = strstr(line, "/");
                unsigned long size = strstr(line_, " ") - line_;
                path = malloc((size + 1) * sizeof(char));
                strncpy(path, line_, size);
                path[size] = '\0';
            } else if (strstr(line, "Authorization: Basic ") && strlen(line) > 21) {
                char *line_ = line + 21;
                size_t size = strlen(line_);
                auth = malloc((size + 1) * sizeof(char));
                strncpy(auth, line_, size);
                auth[size] = '\0';
            }
        }
        if (path != NULL && (credentials == NULL || (auth != NULL && !strcmp(credentials, auth)))) {
            if (strcmp(path, "/list") == 0) {
                msg = getComandOutput(list_cmd);
            } else if (find('/', path) == 2) {
                char *arg = NULL;
                if (strstr(path, "/bind/")) {
                    arg = malloc((strlen(path + 6) + 20) * sizeof(char));
                    strcpy(arg, bind_cmd);
                    strcat(arg, path + 6);
                    msg = getComandOutput(strcat(arg, " 2>&1"));
                } else if (strstr(path, "/unbind/")) {
                    arg = malloc((strlen(path + 8) + 22) * sizeof(char));
                    strcpy(arg, unbind_cmd);
                    strcat(arg, path + 8);
                    msg = getComandOutput(strcat(arg, " 2>&1"));
                }
                release(arg);
            }
        }
        if (msg == NULL)
            write(client_socket, "HTTP/1.1 404 Not Found\n", 24);
        else {
            char *msg_ = malloc((strlen(msg) + 19) * sizeof(char));
            strcpy(msg_, "HTTP/1.1 200 OK\n\n");
            strcat(msg_, msg);
            write(client_socket, msg_, strlen(msg_));
            release(msg_);
        }
        shutdown(client_socket, SHUT_RDWR);
        close(client_socket);
        release(auth);
        release(path);
        release(msg);
    }
    release(credentials);
    close(create_socket);
    return 0;
}