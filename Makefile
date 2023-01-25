all:
	gcc -Wall -Wextra -Werror -Iinclude src/main.c src/server.c src/routes.c -o server

clean:
	rm -f server
