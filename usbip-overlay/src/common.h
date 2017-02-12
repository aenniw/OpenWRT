#ifndef USBIP_REST_COMMON_H
#define USBIP_REST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int pclose(FILE *fp);

extern FILE *popen(__const char *__command, __const char *__modes);

char *getComandOutput(const char *cmd);

char *readLine(int socket);

int find(char c, char *s);

void *release(void *p);

#endif //USBIP_REST_COMMON_H
