#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVICE_PORT	21234
#define BUFSIZE 2048

int main(int argc, char **argv) {
	struct sockaddr_in address, remoteAddress;
	struct sockaddr_in remaddr;
	socklen_t addrlen = sizeof(remaddr);
	unsigned char buf[BUFSIZE];
	
	int recvlen;
	int fd;

	// Creating UDP socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Cannot create socket\n");
		return 0;
	}
	printf("Socket created: descriptor=%d\n", fd);

	// Initializing socket address
	memset((char *) &address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(SERVICE_PORT);

	// Bind address to socket
	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		return 0;
	}
	printf("Addres binding success\n");

	// Listening to incoming message
	while (true) {
		printf("Listening on port %d\n", SERVICE_PORT);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("Received %d bytes\n", recvlen);
		if (recvlen > 0) {
			buf[recvlen] = 0;
			printf("Received message: \"%s\"\n", buf);
		}
	}
	
}
