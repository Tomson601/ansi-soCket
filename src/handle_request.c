#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

void handle_request(int client_sock) {
    char client_message[2000];
    int read_size;
    // Open HTML file
    FILE *file = fopen("../static/index.html", "r");
    if (file == NULL)
    {
        perror("fopen failed");
        return;
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
        return;
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
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
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
    close(client_sock);
}
