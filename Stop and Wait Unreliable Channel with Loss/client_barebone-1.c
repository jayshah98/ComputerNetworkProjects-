//Jay Shah
//COEN 146L, Lab 6
//Date: 2/24/20
//Description: Build a stop and wait reliable protocol on top of UDP to provide a reliable transport service while considering loss

#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab6.h"
#include <netdb.h>


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




void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet, unsigned retries) {
printf("Entering ClientSend\n");
    while (1) {
printf("Entering ClientSend while\n");
        // calculate the checksum
        int checksum = 0;
	char *ptr = (char *)&packet;
	char *end = ptr + sizeof(Header) + packet.header.len;

	while (ptr < end) {
	    checksum ^= *(ptr++);
	}
        
        // log what is being sent
        printf("Created: ");
        logPacket(packet);

        // Simulate lost packet.
        //if (rand() % PLOSTMSG == 0) {
        //    printf("Dropping packet\n");
        //} else {
            sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
         //   }
       
	printf("[ClientSend]..........1\n");

        // wait until either a packet is received or timeout, i.e using the select statement
        struct timeval tv;
	int rv;

	int sock;
	Packet recvpacket;

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        fd_set readfds;

	fcntl(sock, F_SETFL, O_NONBLOCK);

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
    

	rv = select(sock+1, &readfds, NULL, NULL, &tv);
    
	if(rv == 0) {
	    if(retries >= 3) {
		printf("Max retries limit reached...\n");
		return 1;
	    }
	    printf("Timeout, retrying - this is retry number %d \n", retries+1);   
	    retries += 1;
	}
	else if(rv == 1)
	{
	    printf("There is data to be received...\n");
	    recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, addrlen);
	}   

	printf("There is data to be received...\n");
	    recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, addrlen);
        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);
        // validate the ACK
        if (getChecksum(recvpacket) != recvpacket.header.cksum) {
            // bad checksum, resend packet
            printf("Bad checksum, expected %d\n", getChecksum(recvpacket));
        } else if (packet.header.seq_ack != recvpacket.header.seq_ack) {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected %d\n", packet.header.seq_ack);
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
        }
}
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create and configure the socket
    struct hostent *host;
    host = (struct hostent *)gethostbyname("localhost");
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	printf("Failure to establish socket\n");
	exit(1);
    }

    // initialize the server address structure
    //******STUDENT WORK******
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr = *((struct in_addr*)host->h_addr);

    // open file
    FILE *file = fopen(argv[3], "rb");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    char sbuf[10];
    int ack = 0;

    // send file contents to server
    while(fread(&sbuf, 10, 1, file) > 0)
    {
	printf("Reads: %s \n", sbuf);
        Packet packet;
        strcpy(packet.data, sbuf);
        packet.header.seq_ack = ack;
        packet.header.len = strlen(sbuf);
        packet.header.cksum = getChecksum(packet);
        ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr), packet, 0);
        ack = !ack;
     }
     // send zero-length packet to server to end connection
         //******STUDENT WORK******
     Packet zeroPacket;
     zeroPacket.header.seq_ack = ack;
     zeroPacket.header.len = 0;
     zeroPacket.header.cksum = getChecksum(zeroPacket);
     ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr),zeroPacket, 0);
     // clean up
         //-close the sockets and file
         //******STUDENT WORK******
     fclose(file);
     close(sockfd);
 
     return 0;
}
