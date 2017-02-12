#include "common.h"

char *getComandOutput(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    int size = 0, bufferSize = 20;
    char *line = malloc(bufferSize * sizeof(char));
    if (cmd == NULL || fp == NULL || line == NULL)
        return NULL;
    for (char c = (char) fgetc(fp); c != EOF; c = (char) fgetc(fp), size++) {
        line[size] = c;
        if (size > bufferSize - 2)
            line = realloc(line, sizeof(char) * (bufferSize = bufferSize * 2));
    }
    pclose(fp);
    line[size] = '\0';
    return line;
}

char *readLine(int socket) {
    int size = 0, bufferSize = 20;
    char c = '\0', *line = malloc(sizeof(char) * bufferSize);
    while (read(socket, &c, 1) == 1 && c != '\0' && c != '\n') {
        if (c == '\r')
            continue;
        line[size] = c;
        size++;
        if (size > bufferSize - 2)
            line = realloc(line, sizeof(char) * (bufferSize = bufferSize * 2));
    }
    line[size] = '\0';
    if (size == 0) {
        free(line);
        return NULL;
    }
    return line;
}

int find(char c, char *s) {
    int i;
    for (i = 0; s[i]; s[i] == c ? i++ : *s++);
    return i;
}

void *release(void *p) {
    if (p != NULL)
        free(p);
    return NULL;
}