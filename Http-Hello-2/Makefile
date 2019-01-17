.PHONY:all
all:server client

server:Server.cpp
	g++ -o $@ $^ -lpthread -std=c++11
client:Client.cpp 
	g++ -o $@ $^ -lpthread -std=c++11

.PHONY:clean 
clean:
	rm -f client server
