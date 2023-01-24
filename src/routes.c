#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "routes.h"

void handle_404(int socket)
{
    char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<h1>404 Page Not Found</h1>";
    write(socket, response, sizeof(response));
}

void handle_url(int socket, const char *url, const char *file_path)
{
    char *buffer;
    size_t file_size;
    FILE *fp;
    if (file_path == NULL)
    {
        char response[1024];
        sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<h1>Welcome to %s</h1>", url);
        write(socket, response, sizeof(response));
    }
    else
    {
        fp = fopen(file_path, "r");
        if (fp == NULL)
        {
            handle_404(socket);
            return;
        }
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);

        buffer = (char *)malloc(file_size);
        fread(buffer, 1, file_size, fp);

        write(socket, buffer, file_size);

        fclose(fp);
        free(buffer);
    }
}
