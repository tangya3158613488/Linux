.PHONY:all
all:ClientPipe ServerPipe
ClientPipe:ClientPipe.c
	gcc -o $@ $^
ServerPipe:ServerPipe.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f ClientPipe ServerPipe
