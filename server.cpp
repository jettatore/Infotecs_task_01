//#include <stdio.h>
#include <cstring>
#include <string>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>

using std::string;

int main() {

    struct addrinfo hints;
    struct addrinfo* servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "1234", &hints, &servinfo);
    int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    printf("Socet: %i\n", s);

    bind(s, servinfo->ai_addr, servinfo->ai_addrlen);
    listen(s, 5);
    int ac = accept(s, NULL, NULL);

    char buf[BUFSIZ];

    while(true) {
	int rc = recv(ac, buf, BUFSIZ, 0);
	if (rc <= 0) break;
	string result(buf);
	printf("Data: %s. ", buf);
    if (result.size() > 2 && !(stoi(result)%32)) {
        printf("Correct data\n");
	} else {
        printf("Wrong data\n");
    }
    memset(buf, 0, BUFSIZ);
    }
    return 0;
}
