#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>

#include "routes.h"
#include "user_routes.h"
#include "logger.h"

#define PORT 8080
#define MAX_CONNECTIONS 100

int connections[MAX_CONNECTIONS];

volatile sig_atomic_t running = 1;

void handle_sigint()
{
    running = 0;
    log_message("\nShutting down...\n");
    exit(0);
}

void start_server()
{
    signal(SIGINT, handle_sigint);
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFSIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(1);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(1);
    }

    while (running)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
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
                exit(1);
            }

            char method[BUFSIZE];
            char path[BUFSIZE];
            sscanf(buffer, "%s %s", method, path);

            if (strcmp(path, "/home") == 0)
            {
                handle_home(new_socket);
                printf("\nMethod: %s on: %s\n", method, path);
                findUserAgent(buffer);
            }
            else if (strcmp(path, "/home/about") == 0)
            {
                handle_about(new_socket);
                printf("\nMethod: %s on: %s\n", method, path);
                findUserAgent(buffer);
            }
            else
            {
                handle_404(new_socket);
                printf("\nMethod: %s on: %s\n", method, path);
                printf("Specified URL does not exist!\n");
                findUserAgent(buffer);
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
    // Cleanup: close all open sockets and connections
    close(server_fd);
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (connections[i] > 0)
        {
            close(connections[i]);
            printf("Closing connection: %i", connections[i]);
        }
    }
}
