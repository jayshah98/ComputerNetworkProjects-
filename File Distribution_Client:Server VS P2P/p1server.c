/*
* Name: Jay Shah
* Date:
* Title: Lab4 Part 1
* Description: TCP server that accepts and responds to multiple client connection requests for file transfer.
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


#define N 1024
int fd;
pthread_t threader[N];
void* calMatrix(void* tracker){
  int bytes_recieved;
  char recv_data[1024];
    int i=(int)tracker;
     while ((bytes_recieved = recv(i,recv_data,1024,0))){  
	    write(fd,recv_data,bytes_recieved);
	    printf("writing %d bytes", bytes_recieved);
	    if(bytes_recieved==0){
	      break;
      }
   }
}
int main()
{
    int sock, connected , true = 1;  
    size_t bytes_recieved;
    char send_data [1024] , recv_data[1024];       
    struct sockaddr_in server_addr,client_addr;    
    int sin_size;
//Open socket        
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {// checks for an error in the  socket
        perror("Socket");
        exit(1);
    }
//Set address
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(5000);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
//Bind socket
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {// prints an error if my client and server are unable to bind
        perror("Unable to bind");
        exit(1);
    }
    printf("\nTCPServer Waiting for client on port 5000: \n");
    if (listen(sock, 1000) == -1) {// prints an error if my listen fails
        perror("Listen");
        exit(1);
    }
    sin_size = sizeof(struct sockaddr_in);
   fd= open("dest.dat",O_WRONLY,0);
   int z=0;
   while(1){
       connected = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
       pthread_create(&threader[z],NULL,calMatrix,(void*)(size_t)connected);
       z++;
}
        printf("I got a connection from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    close(connected);
    close(fd);
    close(sock);
    return 0;
} 
