.PHONY:all
all:tcpClient tcpServer
tcpClient:tcpClient.cc
	g++ -o $@ $^
tcpServer:tcpServer.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f tcpClient tcpServer
