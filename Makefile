server:
	gcc -lpthread -o server.o server.c
client:
	gcc -o client.o client.c
clean:
	rm -rf server.o client.o
