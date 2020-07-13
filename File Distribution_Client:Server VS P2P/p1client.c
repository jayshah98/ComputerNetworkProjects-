/*
* Name: Jay Shah
* Date:
* Title: Lab4 Part 1
* Description: TCP client that accepts and responds to multiple client connection requests for file transfer.
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


int main()
{
    int sock;
    size_t bytes_recieved;  
    char send_data[1024],recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;  
    host = gethostbyname("127.0.0.1");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
//Set address
    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(5000);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
//Connect
    if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) 
    {
        perror("Connect");
        exit(1);
    }
        printf("Client: Type a message (OR q/ Q to quit): \t");
        scanf("%s",send_data);    
	int fd=open(send_data,O_RDONLY,0);
	int count=1;
	while( count !=0){
	  count= read(fd,send_data,sizeof(send_data));
	  send(sock,send_data,count,0);
	}
    close(sock);   
    return 0;
}
