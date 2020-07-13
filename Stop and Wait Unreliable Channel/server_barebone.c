//Jay Shah
//COEN 146L, Lab 5
//Date: 2/16/20
//Description: Build a stop and wait reliable protocol on top of UDP to provide a reliable transport service





#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab5.h"


// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Please find an error from the remaining part getChecksum() function 
//******STUDENT WORK****** 
    while (ptr < end) {
        checksum ^= *(ptr++);
    }

    return checksum;
}

void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Sending a ACK to the client, i.e., letting the client know what was the status of the packet it sent.
void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;
	//-Prepare the packet headers
	//-send the packet
    	//******STUDENT WORK******
    packet.header.seq_ack = seqnum;
    packet.header.len = 0;
    packet.header.cksum = getChecksum(packet);
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);    
		

    printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
}

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;

    while (1) {
        //recv packets from the client
		//******STUDENT WORK******
	Packet packet;
	printf("Attempting to recieve: \n");
	recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        // log what was received
        printf("Received: ");
        logPacket(packet);

        if (packet.header.cksum!=getChecksum(packet)) {
            printf("Bad checksum, expected checksum was: %d\n", packet.header.cksum);
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected sequence number was:%d\n", seqnum);
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else {
            printf("Good packet\n");
            ServerSend(sockfd, address, *addrlen, seqnum);
	    printf("Writing to file: %s \n", packet.data);
	    return packet;
        }
    }

    printf("\n");
}


int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	printf("Failure to establish socket \n");
	exit(1);
    }

    // initialize the server address structure using argv[1]
    struct sockaddr_in address;
    socklen_t addrLen = sizeof(struct sockaddr);
    address.sin_family = AF_INET;
    address.sin_port = htons (5000);
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
	// No need to bind socket on client side because it is a UDP connection
    if(bind(sockfd, (struct sockaddr *)&address, addrLen) < 0)
    {
	perror("Failure to bind server address to endpoint socket");
	exit(1);
    }

    // open file using argv[2]
    FILE *fp;
    fp = fopen(argv[2], "w");
    if (fp==NULL)
    {
	printf("Error opening file \n");
	exit(1);
    }
    printf("File opened \n");

    // get file contents from client
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        packet = ServerReceive(sockfd, (struct sockaddr*)&clientaddr, &clientaddr_len, seqnum);
	//******STUDENT WORK******
	fwrite(packet.data, strlen(packet.data), 1, fp);
	ServerSend(sockfd, (struct sockaddr*)&address, addrLen, seqnum);
	seqnum != seqnum;	
    } while (packet.header.len != 0);

    fclose(fp);
    close(sockfd);
    return 0;
}
