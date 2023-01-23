build:
	gcc -o server.o src/main.c src/handle_request.c
clean:
	rm *.o