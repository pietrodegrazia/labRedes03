CC = gcc
FLAGS = -Wall 

all: server client

server: server.c
	$(CC) server.c -lc -o server $(FLAGS)

client: client.c
	$(CC) client.c -lc -o client $(FLAGS)

clean:
	rm server client
