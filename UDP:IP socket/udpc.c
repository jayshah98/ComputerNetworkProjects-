//Jay Shah
//Lab 3 Part 2
//1/31/20
//Description: Client that sends file to a UDP server

//UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SIZE 40

int main(){
 //Declare socket file descriptor. All 
Unix 
I/O 
streams 
are
 referenced
 by 
descriptors
 int sockfd; 

 //Declare sending buffer of size 1k bytes
 char sbuf[1024]; 
 
 //Declare server address
 struct sockaddr_in servAddr; 

 //Converts
 domain
 names
 
into
 numerical 
IP 
addresses 
via 
DNS
 struct hostent *host;
 host = (struct hostent *)gethostbyname("129.210.16.94"); //Local host runs the server. You may use "127.0.0.1": loopback IP address
 
 //Open a socket, if successful, returns 
a
descriptor
 associated 
with
 an endpoint 
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

 //Set the server address to send using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000);
 servAddr.sin_addr = *((struct in_addr *)host->h_addr);

 //Client to send messages to the server continuously using UDP socket 
 while(1){
    printf("Client: Type a filename \n");
    scanf("%s", sbuf);

    FILE *src = fopen(sbuf, "r");
    char buffer[SIZE];
    int bytesread;
    
    if (src == NULL) {
	printf("Error opening source\n");
	exit(1);
    }

    while(!feof(src)) {
	bytesread = fread(&buffer, 1, sizeof(buffer), src);
	sendto(sockfd, buffer, bytesread, 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));
    }   	
    
    sendto(sockfd, NULL, 0, 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));
    fclose(src);
    printf("Done successfully");
    }
 return 0;
}
