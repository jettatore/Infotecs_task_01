all:		client

client:		client.o data.o functions.o
	g++ -pthread -o client client.o data.o functions.o

functions.o:	functions.cpp functions.h
	g++ -c functions.cpp

data.o:		data.cpp data.h
	g++ -c data.cpp

client.o:	client.cpp
	g++ -c client.cpp

clean:
	rm -rf *.o
