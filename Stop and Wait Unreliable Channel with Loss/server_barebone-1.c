//Jay Shah
//COEN 146L, Lab 6
//Date: 2/24/20
//Description: Build a stop and wait reliable protocol on top of UDP to provide a reliable transport service while considering loss




#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab6.h"

//add the missing functions
//******STUDENT WORK******
//******STUDENT WORK******
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
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: %s\"",
	packet.header.seq_ack,
        packet.header.len,
        packet.header.cksum, packet.data);
    //printf(""packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}



void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    // Simulating a chance that ACK gets lost
    if (rand() % PLOSTMSG == 0) {
        //******STUDENT WORK******
        printf("Dropping packet\n");
    }   else {
	    Packet packet;
        	packet.header.seq_ack = seqnum;
        	packet.header.len = 0;
        	packet.header.cksum = getChecksum(packet);
	    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
	    printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    }
}

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum)
{
                     
    Packet packet;
    while (1) {
	//Receive a packet from the client
	printf("Attempting to receive: \n");
	recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
	//printf("Packet recvd, printing the contents\n");
	//logPacket(packet);
        // validate the length of the packet
        //******STUDENT WORK******
	if(strlen(packet.data) != packet.header.len)
	{
	    printf("Length of packet not verified\n");
	}
        // log what was received
        printf("Received: ");
        logPacket(packet);

        //verify the checksum and the sequence number
	
        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected %d\n", getChecksum(packet));
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected %d\n", seqnum);
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
    if(bind(sockfd, (struct sockaddr *)&address, addrLen) < 0) {
	perror("Failure to bind server address to endpoint socket");         
	exit(1); 
    }
    
    //Please answer the following questions
        //[Q1] Briefly explain what is "AF_INET" used for.
	    //AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with
        //[Q2] Briefly explain what is "SOCK_DGRAM" used for.
	    //SOCK_DGRAM is used for sending individually-addressed packets unreliably
        //[Q3] Briefly explain what is "htons" used for.
	    //htons function is used to convert an IP port number in host byte order to the IP port number in network byte order
        //[Q4] Briefly explain why bind is required on the server and not on the client.
	    //It is required on server and not the client because it is a UDP connection
    
    
    
    // open file
    FILE *file = fopen("des.txt", "wb");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // get file contents from client and save it to the file
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
	packet = ServerReceive(sockfd, (struct sockaddr*)&clientaddr, &clientaddr_len, seqnum);
        fwrite(packet.data, strlen(packet.data), 1, file);
        ServerSend(sockfd, (struct sockaddr*)&address, addrLen, seqnum);
        seqnum != seqnum;
    } while (packet.header.len != 0);
 
     fclose(file);
     close(sockfd);
     return 0;
}
