void handle_root(int socket)
{
    handle_url(socket, "/", "templates/root.html");
}

void handle_home(int socket)
{
    handle_url(socket, "/home", "templates/index.html");
}

void handle_about(int socket)
{
    handle_url(socket, "/home/about", "templates/about.html");
}
