#ifndef ROUTES_H
#define ROUTES_H

#define BUFSIZE 1024

void handle_home(int socket, char* file_name);
void handle_about(int socket);
void handle_404(int socket);

#endif
