//#include <stdio.h>
#include <cstring>
#include <string>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using std::string;

int main() {
    int gai;
    int sckt;
    int acpt;
    int rcv;
    char buf[BUFSIZ];

    struct addrinfo hints;
    struct addrinfo* servinfo;

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
        //printf("socket: %s\n", strerror(errno));
        exit(1);
    }

    int yes = 1;
    if (setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(sckt, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        printf("bind: %s\n", strerror(errno));
        exit(1);
    }

    if (listen(sckt, 5) != 0) {
        printf("listen: %s\n", strerror(errno));
        exit(1);
    }

    if ((acpt = accept(sckt, NULL, NULL)) == -1) {
        printf("accept: %s\n", strerror(errno));
        exit(1);
    }

    while(true) {
	rcv = recv(acpt, buf, BUFSIZ, 0);
	if (rcv == 0) {
            printf("Client closed connection\n");
            exit(1);
	} else if (rcv == -1) {
            perror("recv");
            exit(1);
	}
	string result(buf);
	printf("Data: %s. ", buf);
        if (result.size() > 2 && !(stoi(result)%32)) {
            printf("Correct data\n");
        } else {
            printf("Wrong data\n");
        }
        memset(buf, 0, BUFSIZ);
    }

    close(sckt);
    freeaddrinfo(servinfo);
    return 0;
}
