.PHONY:all
all:client server
CC=gcc

client:client.c
	$(CC) -o $@ $^ 
server:server.c
	$(CC) -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f client server
