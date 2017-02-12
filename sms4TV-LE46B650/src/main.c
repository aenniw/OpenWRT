//--------------------------------------------------------------------------------
// SMS4TV message sender for TV ver.1.03
//--------------------------------------------------------------------------------
// (c)2010 by geo650
//         based on free java source code from Ewoud Werkman and pter-s
//--------------------------------------------------------------------------------
// compile using Dev-C++ (gcc) in Windows
// compile using m.sh script in Linux (define UTC2LOCAL)
// compile using make.sh script in Linux for cross-compilation
//--------------------------------------------------------------------------------
/* WARNING: you can see such message when compiling ARM version of SMS4TV:

  In file included from /usr/local/lib/gcc/arm-SamyGO-linux-gnueabi/4.2.0/../../../../arm-SamyGO-linux-gnueabi/include/sys/socket.h:35,
                   from /usr/local/lib/gcc/arm-SamyGO-linux-gnueabi/4.2.0/../../../../arm-SamyGO-linux-gnueabi/include/netinet/in.h:24,
                   from /usr/local/lib/gcc/arm-SamyGO-linux-gnueabi/4.2.0/../../../../arm-SamyGO-linux-gnueabi/include/netdb.h:28,
                   from sms.c:23:
  /usr/local/lib/gcc/arm-SamyGO-linux-gnueabi/4.2.0/../../../../arm-SamyGO-linux-gnueabi/include/bits/socket.h:311:24: error: asm/socket.h: No such file or directory

You must install additional files (like asm/socket.h) from your Samsung opensource files.
*/

#define SUMMER_TIME   0     // local timezone (UTC+...) for summer time
#define WINTER_TIME   0     // local timezone (UTC+...) for winter time

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock.h>    // windows only
#else

#include <netdb.h>      // linux only
#include <unistd.h>

#define UTC2LOCAL       // conversion from UTC time to local time? (use for cross-compilation only)
#endif

#define PORT 52235
#define MAXDATASIZE 512
#define DEFAULT_HOST "192.168.43.2"

int sockfd, numbytes;
struct hostent *he;
struct sockaddr_in their_addr;
char *serviceURI = "/PMR/control/MessageBoxService";
char host[128];
char buffer[MAXDATASIZE + 1];
char mesbuf[MAXDATASIZE + 1];
int messageID = 0;


// send data to the socket
int datasend(char *s) {
    int bytes_to_send, bytes_sent;

    bytes_to_send = strlen(s);
    if (bytes_to_send > 0) {
        if ((bytes_sent = send(sockfd, s, bytes_to_send, 0)) == -1) {
            printf("SMS4TV ERROR: cannot send!\n");
            return 1;
        }

        if (bytes_sent != bytes_to_send) {
            printf("SMS4TV ERROR: not all bytes has been sent!\n");
            return 1;
        }
    }

    return 0;
}


// build SOAP message
int compileSoapMessage(char *receiver, char *message, char *sender) {
    int i, ln;
    int allInfo = 1;
    messageID++;
    char *receiverNumber = "";
    char *senderNumber = "";
    ln = strlen(message);
    if (ln > 0) {
        for (i = 0; i < ln; i++) {
#ifdef _WIN32
            switch(message[i])
            {
                // remove national characters (Polish)
            case 0xA5:
                message[i]='a';
                break;
            case 0x86:
                message[i]='c';
                break;
            case 0xA9:
                message[i]='e';
                break;
            case 0xE4:
                message[i]='n';
                break;
            case 0xA2:
                message[i]='o';
                break;
            case 0x98:
                message[i]='s';
                break;
            case 0xAB:
            case 0xBE:
                message[i]='z';
                break;
            case 0xA4:
                message[i]='A';
                break;
            case 0x8F:
                message[i]='C';
                break;
            case 0xA8:
                message[i]='E';
                break;
            case 0xE3:
                message[i]='N';
                break;
            case 0xE0:
                message[i]='O';
                break;
            case 0x97:
                message[i]='S';
                break;
            case 0x8D:
            case 0xBD:
                message[i]='Z';
                break;
            default:
                ;
            }
#endif
            if (!(((message[i] >= 'A') && (message[i] <= 'Z')) ||
                  ((message[i] >= 'a') && (message[i] <= 'z')) ||
                  ((message[i] >= '0') && (message[i] <= '9')) ||
                  (message[i] == ' ') || (message[i] == '_') ||
                  (message[i] == '!') || (message[i] == '?') ||
                  (message[i] == ':') || (message[i] == ';') ||
                  (message[i] == '.') || (message[i] == ',') ||
                  (message[i] == '-') || (message[i] == '+') ||
                  (message[i] == '*') || (message[i] == '=') ||
                  (message[i] == '@') || (message[i] == '#') ||
                  (message[i] == '$') || (message[i] == '%') ||
                  (message[i] == '^') || (message[i] == '&') ||
                  (message[i] == '(') || (message[i] == ')') ||
                  (message[i] == '[') || (message[i] == ']') ||
                  (message[i] == '{') || (message[i] == '}') ||
                  (message[i] == '/') || (message[i] == '\'')
            ))
                message[i] = '_';
        }
    } else return 1;

    if (receiver[0] == 0) strcpy(receiver, "?");
    if (sender[0] == 0) strcpy(sender, "?");

    time_t t;
    struct tm *tm;
    t = time(NULL);
    tm = localtime(&t);
    int y = tm->tm_year;
    if (y < 1900) y += 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int wday = tm->tm_wday;
    int hour = tm->tm_hour;

#ifdef UTC2LOCAL
    int monthtab[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap = ((y % 4) == 0) ? 1 : 0;
    if ((y % 100) == 0) leap = ((y % 400) == 0) ? 1 : 0;
    monthtab[2] = 28 + leap;
    int zone = WINTER_TIME;
    if ((month) && (month < 10)) zone = SUMMER_TIME;
    else if ((month == 3) || (month == 10)) {
        if ((day + 8) > monthtab[month]) {
            if (month == 3) {
                zone = SUMMER_TIME;
                if ((wday == 0) && (hour < 1)) zone = WINTER_TIME;
            }
            if ((month == 10) && (wday == 0) && (hour < 1)) zone = SUMMER_TIME;
        } else {
            if (month == 10) zone = SUMMER_TIME;
        }
    }

    hour += zone;
    if (hour > 23) {
        hour -= 24;
        wday++;
        if (wday > 6) wday = 0;
        day++;
        if (day > monthtab[month]) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                y++;
                tm->tm_year++;
            }
        }
    } else if (hour < 0) {
        hour += 24;
        wday--;
        if (wday < 0) wday = 6;
        day--;
        if (day < 1) {
            month--;
            if (month < 1) {
                month = 12;
                y--;
                tm->tm_year--;
            }
            day = monthtab[month];
        }
    }
#endif

    strcpy(mesbuf, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    strcat(mesbuf + strlen(mesbuf), "<s:Envelope");
    strcat(mesbuf + strlen(mesbuf), " xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"");
    strcat(mesbuf + strlen(mesbuf), " s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">");
    strcat(mesbuf + strlen(mesbuf), "<s:Body>");
    strcat(mesbuf + strlen(mesbuf), "<u:AddMessage xmlns:u=\"urn:samsung.com:service:MessageBoxService:1\">");
    strcat(mesbuf + strlen(mesbuf), "<MessageType>text/xml</MessageType>");
    sprintf(mesbuf + strlen(mesbuf), "<MessageID>%d</MessageID>", messageID);

    strcat(mesbuf + strlen(mesbuf), "<Message>");
    strcat(mesbuf + strlen(mesbuf), "&lt;Category&gt;SMS&lt;/Category&gt;");
    sprintf(mesbuf + strlen(mesbuf), "&lt;DisplayType&gt;%s&lt;/DisplayType&gt;", allInfo ? "Maximum" : "Basic");
    strcat(mesbuf + strlen(mesbuf), "&lt;ReceiveTime&gt;");
    sprintf(mesbuf + strlen(mesbuf), "&lt;Date&gt;%04d-%02d-%02d&lt;/Date&gt;", y, month, day);
    sprintf(mesbuf + strlen(mesbuf), "&lt;Time&gt;%02d:%02d:%02d&lt;/Time&gt;", hour, tm->tm_min, tm->tm_sec);
    strcat(mesbuf + strlen(mesbuf), "&lt;/ReceiveTime&gt;");
    strcat(mesbuf + strlen(mesbuf), "&lt;Receiver&gt;");
    sprintf(mesbuf + strlen(mesbuf), "&lt;Number&gt;%s&lt;/Number&gt;", receiverNumber);
    sprintf(mesbuf + strlen(mesbuf), "&lt;Name&gt;%s&lt;/Name&gt;", receiver);
    strcat(mesbuf + strlen(mesbuf), "&lt;/Receiver&gt;");
    strcat(mesbuf + strlen(mesbuf), "&lt;Sender&gt;");
    sprintf(mesbuf + strlen(mesbuf), "&lt;Number&gt;%s&lt;/Number&gt;", senderNumber);
    sprintf(mesbuf + strlen(mesbuf), "&lt;Name&gt;%s&lt;/Name&gt;", sender);
    strcat(mesbuf + strlen(mesbuf), "&lt;/Sender&gt;");
    sprintf(mesbuf + strlen(mesbuf), "&lt;Body&gt;%s&lt;/Body&gt;", message);
    strcat(mesbuf + strlen(mesbuf), "</Message>");

    strcat(mesbuf + strlen(mesbuf), "</u:AddMessage>");
    strcat(mesbuf + strlen(mesbuf), "</s:Body>");
    strcat(mesbuf + strlen(mesbuf), "</s:Envelope>\r\n");

    return 0;
}

int backupMessage(char *receiver, char *message, char *sender) {
    FILE *f;
    f = fopen("/etc/sms4TV.msg", "a");
    fprintf(f, "@@%s@@ @@%s@@ @@%s@@\n", receiver, message, sender);
    fclose(f);
    return 0;
}

// main function
int main(int argc, char *argv[]) {
    char s[512];
    int err = 0;
    int resend = 0;
    int status = 1;
    strcpy(host, DEFAULT_HOST);
    if ((argc == 2) && (0 == strcmp(argv[1], "-resend"))) { resend = 1; }
    if ((resend == 0) && ((argc < 3 + 1) || (argc > 4 + 1))) {
        printf("SMS4TV 1.03: freeware command-line utility for Samsung TV-set. (c)2010 geo650\n\n");
        printf("This tool can send short message to your TV-set using network connection.\n\n");
        printf("usage: sms <to> <message> <from> [host]\n");
        printf("example:\nsms \"My Dear\" \"Hello! I'll be back at 7pm. See you.\" George 192.168.1.5\n");
        printf("\nCredits: thanks to Ewoud Werkman and pter-s for their java applications\n");
        printf("and thanks to all the people from SamyGO forum.\n");
        return 1;
    }
    if (resend == 0) {
        if (argc > 3 + 1) strcpy(host, argv[4]);
        backupMessage(argv[1], argv[2], argv[3]);
    }

    FILE *fi;
    fi = fopen("/etc/sms4TV.msg", "r");
    if (fi == NULL)return 1;
    char line[256];
    char out[3][256];
    while (fgets(line, sizeof(line), fi) != NULL) {
        int konec = strlen(line);
        int i;
        int nasiel = 0;
        int pocitadlo = 0;
        for (i = 0; i < konec - 2; i++) {
            if ((line[i] == '@') && (line[i + 1] == '@')) {
                nasiel++;
                if (nasiel % 2 == 0)out[((nasiel - 1) / 2)][pocitadlo] = '\0';
                pocitadlo = 0;
                i = i + 2;
            }
            if (nasiel % 2 == 1) {
                out[(nasiel / 2)][pocitadlo] = line[i];
                pocitadlo++;
            }
        }
        //tamtie sracky
#ifdef _WIN32
        WSADATA wsaData;
        int iResult;

        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != NO_ERROR)
        {
            printf("SMS4TV: WSAStartup failed: %d\n", iResult);
            fclose(fi);
            return 1;
        }
#endif

        if ((he = gethostbyname(host)) == NULL) {
            printf("SMS4TV ERROR: cannot get host info!\n");
            fclose(fi);
            return 1;
        }

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            printf("SMS4TV ERROR: cannot create socket!\n");
            fclose(fi);
            return 1;
        }

        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(PORT);
        their_addr.sin_addr = *((struct in_addr *) he->h_addr);
        memset(&(their_addr.sin_zero), '\0', 8);

        if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1) {
            printf("SMS4TV ERROR: cannot connect!\n");
            fclose(fi);
            return 1;
        }

        err += compileSoapMessage(out[0], out[1], out[2]);
        sprintf(buffer, err ? "" : "POST %s HTTP/1.0\r\n", serviceURI);
        err += datasend(buffer);
        sprintf(buffer, err ? "" : "Host: %s\r\n", host);
        err += datasend(buffer);
        strcpy(buffer, err ? "" : "Content-Type: text/xml; charset=UTF-8\r\n");
        err += datasend(buffer);
        sprintf(buffer, err ? "" : "Content-Length: %d\r\n", strlen(mesbuf));
        err += datasend(buffer);
        strcpy(buffer, err ? "" : "SOAPACTION: \"urn:samsung.com:service:MessageBoxService:1#AddMessage\"\r\n");
        err += datasend(buffer);
        strcpy(buffer, err ? "" : "Connection: close\r\n");
        err += datasend(buffer);
        strcpy(buffer, err ? "" : "\r\n");
        err += datasend(buffer);
        err += datasend(mesbuf);

        numbytes = 1;
        status = 1;
        while (numbytes && status) {
            if (err == 0) {
                if ((numbytes = recv(sockfd, buffer, MAXDATASIZE, 0)) == -1) {
                    printf("SMS4TV ERROR: cannot receive!\n");
                    fclose(fi);
                    return 1;
                }
                if (strstr(buffer, "200 OK")) status = 0;
            }
        }

#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        close(sockfd);
#endif

        printf((err + status) ? "SMS not sent!\n" : "SMS sent successfully.\n");
    }
    fclose(fi);
    remove("/etc/sms4TV.msg");
    return 0;
}

