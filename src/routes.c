#include <unistd.h>

void handle_home(int socket) {
    char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<h1>Welcome to the home page</h1>";
    write(socket, response, sizeof(response));
}

void handle_about(int socket) {
    char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<h1>About us</h1>";
    write(socket, response, sizeof(response));
}

void handle_404(int socket) {
    char response[] = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<h1>404 Page Not Found</h1>";
    write(socket, response, sizeof(response));
}
