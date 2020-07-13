/* 
* Name: Jay Shah
* Date: 
* Title: Lab4 Part 2
* Description:  UDP client to communicate with a UDP server on port 5000 and send the server some character data.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

//UDP CLIENT

int main()
{
  int sock;
  struct sockaddr_in server_addr;
  struct hostent *host;
  char send_data[1024];
  socklen_t addr_len;
  host= (struct hostent *) gethostbyname((char *)"127.0.0.1");

  // open socket
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
      perror("socket");
      exit(1);
    }

	//server info setup
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5000);
  server_addr.sin_addr = *((struct in_addr *)host->h_addr);

	//read and send data from file
  FILE *fptr1, *fptr2; 
  int buffSize=sizeof(char);
  char buffer[0];
  fptr1 = fopen("src.dat", "rb");
  while(fread(&buffer,1,1,fptr1)){
  printf("reading char %s\n", buffer);
    sendto(sock, buffer, strlen(buffer),0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
  }
    int fd=0;
    fd=open("final.dat",O_WRONLY);
    char recv_data[1024];
    size_t bytesRead=1;
    //listens in on socket for characters to then write into output file dest.dat
    while (bytesRead>0)
    {
        bytesRead = recvfrom(sock,recv_data,sizeof(recv_data),0,(struct sockaddr *)&server_addr, &addr_len);
        printf("%s",recv_data);
        //sendto(sock, bytesRead, strlen(bytesRead),0,(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
        write(fd,recv_data,bytesRead);
        //          fflush(stdout);
    }
    
  fclose(fptr1);

}

