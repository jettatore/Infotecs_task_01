#include "connection.h"

void Connection::Init() {

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

void Connection::Receive() {
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

string Connection::GetData() {
    return string(buf);
}

void Connection::Close() {
    close(sckt);
    freeaddrinfo(servinfo);
}
