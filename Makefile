CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

all: server

server: src/main.c src/routes.c
	$(CC) $(CFLAGS) src/main.c src/routes.c -o server

clean:
	rm -f server
