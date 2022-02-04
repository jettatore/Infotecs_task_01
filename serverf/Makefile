all:	server

server:	server.o connection.o
	g++ -o server server.o connection.o

connection.o:	connection.cpp connection.h
	g++ -c connection.cpp

server.o:	server.cpp
	g++ -c server.cpp

clean:
	rm -fr *.o
