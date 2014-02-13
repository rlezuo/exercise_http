/*
 *	very simple TCP server program, may become a HTTP server one day
 *	BUG: seems to send wrong data back to client
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
	
	/* prepare for quick restart cycles */
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	/* server address */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* bind to address */
	if (bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1)	{
		bailOut("bind failed");
	}

	/* listen for incoming connections */
	if (listen(sockfd, 0) == -1)	{
		bailOut("listen failed");
	}

	/* server main loop */
	printf("Entering Server main loop\n");
	for (;;)	{
		int clientfd = accept(sockfd, NULL, NULL);
		if (clientfd == -1) {
			bailOut("accept failed");
		}

		printf("Connection accepted\n");

		char buffer[BUFFERSZ];
		int readsz;
		do	{	
			readsz = recv(clientfd, buffer, sizeof(buffer)-1, 0);
			if (readsz == -1) {
				bailOut("recv failed");
			}
		} while (readsz > 0);

		printf("Data read\n");

		/* now process data */

		snprintf(buffer, BUFFERSZ, "%s", "Did you mean bar?");
		buffer[BUFFERSZ-1] = 0;

		/* and send the data */
		if (send(clientfd, buffer, strlen(buffer), 0) == -1) {
			bailOut("send failed");
		}

		close(clientfd);
	}
}
