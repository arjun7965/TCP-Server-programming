all:
	gcc -lpthread -o server.o server.c
	gcc -o client.o client.c
server:
	gcc -lpthread -o server.o server.c
client:
	gcc -o client.o client.c
clean:
	rm -rf server.o client.o
