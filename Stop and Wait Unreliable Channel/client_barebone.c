//Jay Shah
//COEN 146L, Lab 5
//Date: 2/16/20
//Description: Build a stop and wait reliable protocol on top of UDP to provide a reliable transport service

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
#include "lab5.h"
#include <netdb.h>
#include <netinet/in.h>



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



void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {

        // send the packet
        printf("Sending packet\n");
	logPacket(packet);
		//******STUDENT WORK******
	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // receive an ACK from the server
        Packet recvpacket;
		//******STUDENT WORK******
	recv(sockfd, &recvpacket, sizeof(recvpacket), 0);
	//log what was received
	printle(fread(&sbuf, 10, 1, fp) > 0)
131     {
132         printf("Reads: %s \n", sbuf);
133         Packet packet;
134         strcpy(packet.data, sbuf);
135         packet.header.seq_ack = ack;
136         packet.header.len = strlen(sbuf);
137         packet.header.cksum = getChecksum(packet);
138         ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockad    dr), packet);
139         ack = !ack;
140     }
141     // send zero-length packet to server to end connection
142         //******STUDENT WORK******
143     Packet zeroPacket;
144     zeroPacket.header.seq_ack = ack;
145     zeroPacket.header.len = 0;
146     zeroPacket.header.cksum = getChecksum(zeroPacket);
147     ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr),     zeroPacket);
148     // clean up
149         //-close the sockets and file
150         //******STUDENT WORK******
151     fclose(fp);
152     close(sockfd);
153 
154     return 0;
f("Received ACK %d checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        // validate the ACK
        if (getChecksum(recvpacket) != recvpacket.header.cksum) {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", packet.header.cksum);
        } else if (packet.header.seq_ack != recvpacket.header.seq_ack) {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
            break;
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create and configure the socket [name the socket sockfd] 
    struct hostent *host;
    host = (struct hostent *)gethostbyname("localhost");
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	printf("Failure to establish socket\n");
	exit(1);
    }	    

    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr = *((struct in_addr*)host->h_addr);

    // open file using argv[3]
	//******STUDENT WORK******
    FILE *fp;
    fp = fopen(argv[3], "r");	
    if (fp == NULL)
    {
	printf("Error opening client file \n");
	exit(1);
    }
    printf("File opened \n");
    char sbuf[10];
    int ack = 0;

    // send file contents to server
	//-create a packet
	//-set the seq_ack field 
	//-set the len field
	//-calculate the checksum
	//-send the packet
	//******STUDENT WORK******
    while(fread(&sbuf, 10, 1, fp) > 0)
    {
	printf("Reads: %s \n", sbuf);
	Packet packet;
	strcpy(packet.data, sbuf);
	packet.header.seq_ack = ack;
	packet.header.len = strlen(sbuf);
	packet.header.cksum = getChecksum(packet);
	ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr), packet);
	ack = !ack;
    }
    // send zero-length packet to server to end connection
	//******STUDENT WORK******
    Packet zeroPacket;
    zeroPacket.header.seq_ack = ack;
    zeroPacket.header.len = 0;
    zeroPacket.header.cksum = getChecksum(zeroPacket);	
    ClientSend(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr), zeroPacket);
    // clean up
	//-close the sockets and file
    	//******STUDENT WORK******
    fclose(fp);
    close(sockfd);

    return 0;
}
