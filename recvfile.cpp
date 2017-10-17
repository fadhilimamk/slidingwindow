#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "buffer.h"
#include "packetAck.h"
#include "segment.h"

#define SERVICE_PORT	21234
#define BUFSIZE 256


void initAddress(sockaddr_in* address) {
	memset((char *) &(*address), 0, sizeof(address));
	(*address).sin_family = AF_INET;
	(*address).sin_addr.s_addr = htonl(INADDR_ANY);
	(*address).sin_port = htons(SERVICE_PORT);
}

int initSocket(int* fd, sockaddr_in address) {
	if ((*fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Cannot create socket\n");
		return -1;
	}
	printf("Socket created: descriptor=%d\n", *fd);

	// Bind address to socket
	if (bind(*fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		return 0;
	}
	printf("Address binding success\n");

	return 0;
}

int main(int argc, char **argv) {
	struct sockaddr_in address, remaddr;
	socklen_t addrlen = sizeof(remaddr);
	unsigned char buf[BUFSIZE];


	
	int recvlen;
	int fd;

	initAddress(&address);
	initSocket(&fd, address);

	// Listening to incoming message
	printf("Listening on port %d\n", SERVICE_PORT);
	int last_ack = 0;
	int windowsize = 10;
	int LFR = -1;
	int LAR = LFR + windowsize;
	while (true) {
		
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("Received %d bytes\n", recvlen);

		for(int i = 0; i < recvlen; i++) {
			printf("%d ", buf[i]);
		}
		printf("\n");
		
		if (recvlen > 0) {
			buf[recvlen] = 0;
			printf("Received message: \"%s\" \n", buf);

			// Add message to receive buffer

			// Send ACK
			char x = last_ack++;
			if (sendto(fd, &x, 1, 0, (struct sockaddr *) &remaddr, addrlen) == -1)
				perror("Error sending ACK");

		}
	}
	

}
