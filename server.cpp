#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>

struct sockaddr_in local;

int main() {

	int s = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socet: %i\n", s);

	local.sin_family = AF_INET;
	local.sin_port = htons(1234);
	inet_pton(AF_INET, "127.0.0.1", &(local.sin_addr));

	bind(s, (struct sockaddr*)&local, sizeof(local));
	listen(s, 5);
	int ac = accept(s, NULL, NULL);

	char buf[BUFSIZ];

	while(true) {
		int rc = recv(ac, buf, BUFSIZ, 0);
		if (rc <= 0) break;
		printf("RC: %i, Data: %s", rc, buf);
		memset(buf, 0, BUFSIZ);
	}

	return 0;
}
