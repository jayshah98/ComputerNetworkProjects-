/* 
* Name: Jay Shah
* Date: 
* Title: Lab4 Part 2
* Description: UDP server to listen in on port 5000 for characters to then write into an output file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
//UDP SERVER

int main()
{
    int sock;
    int bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr , client_addr;
    socklen_t addr_len;
//Open socket
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
// Set address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

 //Bind socket to address
    if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Bind");
        exit(1);
    }
    addr_len = sizeof(struct sockaddr);	
	printf("\t\t\t\t\t\t\nUDPServer Waiting for client on port 5000\n");
    fflush(stdout);
    //int fd=0;
    //fd=open("dest.dat",O_RDONLY);
    //fClear(recv_data,fd);
    size_t bytesRead=1;
	//listens in on socket for characters to then write into output file dest.dat
    while (bytesRead>0)
    {
        bytesRead = recvfrom(sock,recv_data,sizeof(recv_data),0,(struct sockaddr *)&client_addr, &addr_len);
        //printf("%s",recv_data);
        sendto(sock, recv_data, strlen(recv_data),0,(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
//        write(fd,recv_data,bytesRead);
//          fflush(stdout);
    }
    fflush(stdout);
    //close(fd);
    close(sock);
  return 0;
  
}
