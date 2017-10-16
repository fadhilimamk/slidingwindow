#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SERVICE_PORT	21234

int main(int argc, char **argv) {
    
    int fd;
    struct sockaddr_in address, remoteAddress;
    struct hostent *host;
    char message[2048];
    char server[15] = "127.0.0.1";
   
    // Creating UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Cannot create socket");
        return 0;
    }
    printf("Socket created: descriptor=%d\n", fd);

    // Initializing socket address
    memset((char *) &address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(0);

    // Bind address to socket
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 0;
    }
    printf("Address binding success\n");

    
    // Defining remote client
    memset((char* ) &remoteAddress, 0, sizeof(remoteAddress));
    remoteAddress.sin_family = AF_INET;
    remoteAddress.sin_port = htons(SERVICE_PORT);
    if (inet_aton(server, &remoteAddress.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Sending messages
    for (int i = 0; i < 5; i++) {
        printf("Sending packet %d to %s port %d\n", i, server, SERVICE_PORT);
        sprintf(message, "Hello network! %d", i);
        if (sendto(fd, message, strlen(message), 0, (struct sockaddr *) &remoteAddress, sizeof(remoteAddress)) == -1)
            perror("sendto error");
    }

    close(fd);
    return 0;
}
