#pragma once
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <string>

using std::string;

class Connection {

public:
    Connection():optval(1) {}

    void Init();
    void Receive();
    string GetData();
    void Close();

private:
    int gai;
    int sckt;
    int optval;
    int acpt;
    int rcv;

    char buf[BUFSIZ];

    struct addrinfo hints;
    struct addrinfo* servinfo;

};
