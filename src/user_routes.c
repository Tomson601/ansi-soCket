#include "routes.h"

void handle_home(int socket)
{
    handle_url(socket, "/home", "../static/index.html");
}

void handle_about(int socket)
{
    handle_url(socket, "/home/about", "../static/about.html");
}
