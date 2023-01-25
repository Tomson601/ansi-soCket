#ifndef ROUTES_H
#define ROUTES_H

#define BUFSIZE 1024

void handle_404(int socket);
void handle_url(int socket, const char *path, const char *file_path);
void handle_root(int socket);

#endif
