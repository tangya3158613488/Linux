.PHONY:all
all:server client
server:server.c comm.c
	gcc -o $@ $^
client:client.c comm.c
	gcc -o $@ $^
.PHONY:clean all
	rm -f all
