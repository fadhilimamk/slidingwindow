#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#define BUFFER_SIZE	    2048
#define IP_BUFFER_SIZE  15
#define SERVICE_PORT	21234

int main(int argc, char *argv[]) {

    char filename[BUFFER_SIZE];
    char destinationIP[IP_BUFFER_SIZE];
    int windowsize;
    int buffersize;
    int destinationPort;

    // Handle user parameter
    if (argc == 2 && memcmp("-h", argv[1], 2) == 0) {
        cout << "   NoName4 Sliding Window Instruction" << endl;
        cout << "   Execute this program with this command:" << endl;
        cout << "   ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>" << endl << endl;
        exit(0);
    } else if (argc != 6) {
        cout << "   Wrong parameter! use -h for instruction" << endl;
        exit(0);
    } else {
        strcpy(filename, argv[1]);
        windowsize = atoi(argv[2]);
        buffersize = atoi(argv[3]);
        strcpy(destinationIP, argv[4]);
        destinationPort = atoi(argv[5]);
    }
    
    int fd;
    struct sockaddr_in address, remoteAddress;
    struct hostent *host;
    char message[BUFFER_SIZE];
    char server[IP_BUFFER_SIZE];
    strcpy(server, destinationIP);
   
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
    remoteAddress.sin_port = htons(destinationPort);
    if (inet_aton(server, &remoteAddress.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Sending messages
    cout << "Sending file " << filename << " to " << destinationIP << ":" << destinationPort << " (" << windowsize << ", " << buffersize << ")" << endl;
    for (int i = 0; i < 5; i++) {
        printf("Sending packet %d to %s port %d\n", i, server, destinationPort);
        sprintf(message, "Hello network! %d", i);
        if (sendto(fd, message, strlen(message), 0, (struct sockaddr *) &remoteAddress, sizeof(remoteAddress)) == -1)
            perror("sendto error");
    }

    close(fd);
    return 0;
}
