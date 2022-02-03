all:		server	

server:		server.o
	g++ -o server server.o

hello.o:	server.cpp
	g++ -c server.cpp
