#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include "segment.h"

using namespace std;

#define BUFFER_SIZE	    256
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

    // Read file
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to read " << filename << endl;
        exit(0);
    }
    
    int fd;
    struct sockaddr_in address, remoteAddress;
    socklen_t addrlen = sizeof(remoteAddress);
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

    // Make non blocking listener
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    // Sending messages
    cout << "Sending file " << filename << " to " << destinationIP << ":" << destinationPort << " (" << windowsize << ", " << buffersize << ")" << endl;
    unsigned char buf[BUFFER_SIZE];
    char SendBuffer[BUFFER_SIZE];
    char ch;
    int seq = 0;  

    // Pindahin file bertahap ke buffer
    // Tunggu sampai buffer penuh atau file selesai, kemudian kirim
    // Kirim semua isi dalam buffer sampai selesai

    // Make dummy buffer
    vector<char> buffer;
    while (!file.eof()) {
        char c;
        file.get(c);
        buffer.push_back(c);
    }
    file.close();
    
    // Sending buffer
    int last_ack = -1;
    int last_sent = -1;
    while(last_ack != 256) {

        if (last_sent < 255 && last_sent <= last_ack + windowsize) {
            char c = buffer.at(++last_sent);
            printf("Sending packet %d (%c) to %s port %d\n", seq+1, c, server, destinationPort);
            Segment segmentData(seq++, c);
            char* buffer = segmentData.toBuffer();
            for(int i = 0; i < 9; i++) {
                printf("%d ", buffer[i]);
            }
            printf("\n");
            usleep((rand() % 10 + 1) * 100000);
            if (sendto(fd, buffer, 9, 0, (struct sockaddr *) &remoteAddress, sizeof(remoteAddress)) == -1)
                perror("Error sending packet");
        }

        int recvlen = recvfrom(fd, &buf, BUFFER_SIZE, 0, (struct sockaddr *)&remoteAddress, &addrlen);
        if (recvlen > 0) {
            int ack = buf[0];
            printf("Received ACK : %d\n", ack);
            if (ack == last_ack + 1) {
                last_ack++;
            }
        }

    }


    close(fd);
    return 0;
}
