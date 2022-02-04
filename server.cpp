//#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using std::string;
using std::cout;

class Connection {

public:
    Connection() {}

    void Init() {

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        gai = getaddrinfo(NULL, "1234", &hints, &servinfo);
        if (gai != 0) {
            printf("getaddrinfo: %s\n", gai_strerror(gai));
            exit(1);
        }
        sckt = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
        if (sckt == -1) {
            perror("socket");
            exit(1);
        }
        if (setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sckt, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
            printf("bind: %s\n", strerror(errno));
            exit(1);
        }
        if (listen(sckt, 5) != 0) {
            perror("listen");
            exit(1);
        }
        if ((acpt = accept(sckt, NULL, NULL)) == -1) {
            printf("accept: %s\n", strerror(errno));
            exit(1);
        }
    }
    void Receive() {
        memset(buf, 0, BUFSIZ);
      	rcv = recv(acpt, buf, BUFSIZ, 0);
	if (rcv == 0) {
            printf("Client closed connection\n");
            exit(1);
	} else if (rcv == -1) {
            perror("recv");
            exit(1);
	}
    }
    string GetData() {
        return string(buf);
    }
    void Close() {
        close(sckt);
        freeaddrinfo(servinfo);
    }
    
private:
    int gai;
    int sckt;
    int optval = 1;
    int acpt;
    int rcv;

    char buf[BUFSIZ];

    struct addrinfo hints;
    struct addrinfo* servinfo;
};

int main() {
//    int gai;
//    int sckt;
//    int acpt;
//    int rcv;
//    char buf[BUFSIZ];
//
//    struct addrinfo hints;
//    struct addrinfo* servinfo;
//
//    memset(&hints, 0, sizeof hints);
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE;
//
//    gai = getaddrinfo(NULL, "1234", &hints, &servinfo);
//    if (gai != 0) {
//        printf("getaddrinfo: %s\n", gai_strerror(gai));
//        exit(1);
//    }
//
//    sckt = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
//    if (sckt == -1) {
//        perror("socket");
//        exit(1);
//    }
//
//    int yes = 1;
//    if (setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
//        perror("setsockopt");
//        exit(1);
//    }
//
//    if (bind(sckt, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
//        printf("bind: %s\n", strerror(errno));
//        exit(1);
//    }
//
//    if (listen(sckt, 5) != 0) {
//        perror("listen");
//        exit(1);
//    }
//
//    if ((acpt = accept(sckt, NULL, NULL)) == -1) {
//        printf("accept: %s\n", strerror(errno));
//        exit(1);
//    }

    Connection connect;
    connect.Init();

    while(true) {
//	rcv = recv(acpt, buf, BUFSIZ, 0);
//	if (rcv == 0) {
//            printf("Client closed connection\n");
//            exit(1);
//	} else if (rcv == -1) {
//            perror("recv");
//            exit(1);
//	}
//	string result(buf);
        connect.Receive();
        string result = connect.GetData();
	cout << "Data: " << result << " ";
        if (result.size() > 2 && !(stoi(result)%32)) {
            cout << "Correct data\n";
        } else {
            cout << "Wrong data\n";
        }
//        memset(buf, 0, BUFSIZ);
    }
    connect.Close();
//    close(sckt);
//    freeaddrinfo(servinfo);
    return 0;
}
