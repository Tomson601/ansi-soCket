#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "handle_request.h"
#include <signal.h>

int socket_desc;

void handle_sigint(int sig)
{
    printf("\nServer shutting down...\n");
    close(socket_desc); //zamykanie socketu
    exit(0);
}

int main(int argc, char *argv[])
{
    int client_sock, c;
    struct sockaddr_in server, client;
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    signal(SIGINT, handle_sigint);

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9999);

    int optval = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    printf("Server started on port: %i\n", server.sin_port);

    while (1)
    {
        printf("Client connected from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        // accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");
        handle_request(client_sock);
        close(client_sock);
    }
    return 0;
}