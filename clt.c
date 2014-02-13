/*
 *	very simple TCP client program, may become a HTTP client one day
 *	BUG: seems to receive data twice
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFERSZ 80

void bailOut(const char *m)
{
	printf("Last Error: %s", strerror(errno));
	printf("BailOut: %s\n", m);
	exit(-1);
}

int main(int argc, char **argv)
{
	/* create socket */
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		bailOut("socket failed");
	}


	/* server address */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* connect to server */
	if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)	{
		bailOut("connect failed");
	}

	/* create HTTP request */
	char buffer[BUFFERSZ];
	snprintf(buffer, BUFFERSZ, "GET %s HTTP/1.0\n\n", "foo.html");
	buffer[BUFFERSZ-1] = 0;

	/* and send the data */
	if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
		bailOut("send failed");
	}

	/* shutdown for write now */
	shutdown(sockfd, SHUT_WR);

	/* read incoming data */
	int readsz;	
	do {
		readsz = recv(sockfd, buffer, sizeof(buffer)-1, 0);
		buffer[BUFFERSZ-1] = 0;
		if (readsz == -1) {
			bailOut("recv failed");
		}

#if 0
		/* print binary data */
		for (int i=0; i<readsz; ++i)	{
			printf("%xd(%c) ", buffer[i], buffer[i]);
		}
#endif
		printf("%s", buffer);
	} while (readsz > 0);

	close(sockfd);
	return 0;
}
