#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "../include/routes.h"

#define PORT 8080
#define BUFSIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFSIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        return 1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return 1;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        return 1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        return 1;
    }

    while (1)
    {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            return 1;
        }
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        if (pid == 0)
        {
            // child process
            close(server_fd);
            int valread = read(new_socket, buffer, BUFSIZE);
            if (valread == 0)
            {
                break;
            }
            else if (valread < 0)
            {
                perror("read");
                return 1;
            }

            char method[BUFSIZE];
            char path[BUFSIZE];
            sscanf(buffer, "%s %s", method, path);

            if (strcmp(path, "/home") == 0)
            {
                handle_home(new_socket);
            }
            else if (strcmp(path, "/home/about") == 0)
            {
                handle_about(new_socket);
            }
            else
            {
                handle_404(new_socket);
            }
            close(new_socket);
            exit(0);
        }
        else
        {
            // parent process
            close(new_socket);
        }
    }

    return 0;
}
