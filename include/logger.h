#include <stdio.h>
#include <string.h>
#include <time.h>

void log_message(const char *message)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);

    printf("%s %s\n", buffer, message);
}

void findUserAgent(char* text) {
    char userAgent[512] = "";
    char *pos;
    pos = strstr(text, "User-Agent: ");
    if(pos != NULL) {
        strcpy(userAgent, pos + strlen("User-Agent: "));
        pos = strstr(userAgent, "\n");
        if(pos != NULL) {
            *pos = '\0';
        }
    }
    printf("User-Agent: %s\n", userAgent);
}
