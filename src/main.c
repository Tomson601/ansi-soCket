#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "routes.h"

int main(int argc, char* argv[])
{
    char* file_name = NULL;
    if (argc > 1) {
        file_name = argv[1];
    }
    start_server(file_name);
    return 0;
}
