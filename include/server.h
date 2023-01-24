#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 100

int connections[MAX_CONNECTIONS];

void start_server(char* file_name);

#endif
