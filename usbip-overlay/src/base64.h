#ifndef USBIP_REST_BASE64_H
#define USBIP_REST_BASE64_H

#include <stdio.h>
#include <stdlib.h>

static const unsigned char base64_table[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const char *src, size_t len);

#endif //USBIP_REST_BASE64_H
