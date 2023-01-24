#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 100

int connections[MAX_CONNECTIONS];

void handle_home(int socket, char* file_name);
void handle_about(int socket);
void handle_404(int socket);
void start_server(char* file_name);

#endif
