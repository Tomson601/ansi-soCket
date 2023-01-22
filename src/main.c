#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9999);

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

    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    // Open HTML file
    FILE *file = fopen("../static/index.html", "r");
    if (file == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Read file content
    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL)
    {
        perror("malloc failed");
        fclose(file);
        return 1;
    }

    fread(file_content, file_size, 1, file);
    fclose(file);
    file_content[file_size] = '\0';

    // Send HTTP headers
    char http_headers[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    write(client_sock, http_headers, sizeof(http_headers) - 1);

    // Send HTML content
    write(client_sock, file_content, file_size);
    free(file_content);

    // Receive a message from client
    while ((read_size = recv(client_sock, client_message, 20, 0)) > 0)
    {
    // Send the message back to client
    write(client_sock, client_message, strlen(client_message));
    }
    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}