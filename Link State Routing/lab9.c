/* 
 * Name: Jay Shah
 * Date: February 25th, 2020
 * Title: Lab7s
 * Description: runs a link state algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <limits.h> 

#define N 5
#define TRUE 0
#define FALSE 1

typedef struct {
    int port;
    char id[100];
    char ip[100];
} Machines;

Machines machines[N];
int costs[N][N];
int sockfd;

pthread_t threads[3];
pthread_mutex_t lock;

struct sockaddr_in servAddr[N];
struct sockaddr_in clientAddr;

void print_costs(void) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            
            printf("%d ", costs[i][j]);
            
        }
        printf("\n");
    }
    printf("\n");
}

void dijkstra(int src) { 
    int dist[N]; // The output array.  dist[i] will hold the shortest distance from src to i
    int sptSet[N];
    int count = 0;
    int i;

    print_costs();
  
    // Initialize all distances as INFINITE and stpSet[] as false 
    for (count = 0; count < N; count++) {
        //Mofiication
            dist[count] = costs[src][count];
            printf("DISTANCES while intilizing :: %d \n", dist[count]);    
        //
        // dist[count] = 1000;
        sptSet[count] = FALSE;
    }
  
    // Distance of source vertex from itself is always 0 
    //dist[src] = 0; 
  
    // Find shortest path for all vertices 
    for (count = 0; count < N; count++) { 
        // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in the first iteration. 
        int u = min_distance(dist, sptSet); 
  
        // Mark the picked vertex as processed 
        sptSet[u] = TRUE; 

        printf("DISTANCES :: %d, %d, %d, %d\n", dist[0],dist[1],dist[2],dist[3]);    
        // Update dist value of the adjacent vertices of the picked vertex. 
        for (i = 0; i < N; i++) {
  
            // Update dist[i] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to  v through u is smaller than current value of dist[v] 
            if (sptSet[i] && costs[u][i] && dist[u] != 1000) {
                // printf("inside the if satement\n");
                int x  = dist[u] + costs[u][i];
                int y = dist[i];
                printf("inside the if satement dist[u] + costs[u][i]:: %d, dist[i]:%d\n", dist[u] + costs[u][i], dist[i]);
                if (x < y)  {
                    dist[i] = x; 
                } else {
                    dist[i]= y;
                }
            }
        }
                
    } 
  
    // print the constructed distance array 
    print_solution(dist, src); 
}

int min_distance(int dist[], int sptSet[]) 
{ 
    // Initialize min value 
    int min = 1000, min_index = 0, v; 
  
    for (v = 0; v < N; v++) 
        if (sptSet[v] == FALSE && dist[v] <= min) {
            min = dist[v];
            min_index = v; 
        }
  
    return min_index; 
} 
  
// A utility function to print the constructed distance array 
int print_solution(int dist[], int n) 
{ 
    printf("Source Node: %d \n", n);
    printf("Vertex \t\t Distance from Source\n");
    int i;
    for (i = 0; i < N; i++) 
        printf("%d \t\t %d\n", i, dist[i]);
    printf("\n");
} 

void * thread1(void * args) {
    struct sockaddr_in * clientAddr = args;
    socklen_t addrSize = sizeof(struct sockaddr);
    
    while (1) {
        int update[5];
        int i;
        for (i = 0; i < N; i++) {
            int x = recvfrom(sockfd, &update, sizeof(update), 0, (struct sockaddr *)&clientAddr, &addrSize);

            pthread_mutex_lock(&lock);
            // print_costs();
            costs[update[0]][update[1]] = update[2];
            costs[update[1]][update[0]] = update[2];
            // print_costs();
            pthread_mutex_unlock(&lock);
        }
    }
}

void * thread2(void * args) {
    int updates = N;
    struct sockaddr_in * servAddr = args;

    while (updates != 0) {
        int one, two, updatedCost;
        // Get users input
        printf("Enter two positions to change and the cost seperated by spaces: \n");
        scanf("%d %d %d", &one, &two, &updatedCost);
        pthread_mutex_lock(&lock);
        // print_costs();
        costs[one][two] = updatedCost;
        costs[two][one] = updatedCost;
        // print_costs();
        pthread_mutex_unlock(&lock);


        int i;
        for (i = 0; i < N; i++) {
            int update[3] = {one,
                            two,
                            updatedCost};
            sendto(sockfd, &update, sizeof(update), 0, (struct sockaddr *)&servAddr[i], sizeof(struct sockaddr));
        }
        updates--;
        sleep(3);
    }
}

void * thread3() {
    while (1) {
        int randSleep = (rand() % 11 + 10);
        int i;
        print_costs();
        for (i = 0; i < N; i++) {
            dijkstra(i);
        }

        print_costs();
        sleep(randSleep);
    }
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <costs file> <machines file> <self machine>\n", argv[0]);
        return 1;
    }

    int startRouter = atoi(argv[3]);
    FILE *costsf;
    FILE *machinesf;
    costsf = fopen (argv[1],"r");
    machinesf = fopen (argv[2],"r");

    int i;
    // Collect all info
    for (i = 0; i < N; i++) {
        int num0, num1, num2, num3, num4;
        // Get costs
        fscanf(costsf, "%d %d %d %d %d", &num0, &num1, &num2, &num3, &num4);
        costs[i][0] = num0;
        costs[i][1] = num1;
        costs[i][2] = num2;
        costs[i][3] = num3;
        costs[i][4] = num4;
        
        char machineName[100], machineIP[100];
        //Get machines
        fscanf(machinesf, "%s %s %d", &machineName, &machineIP, &num0);
        strncpy(machines[i].id, machineName, 100);
        strncpy(machines[i].ip, machineIP, 100);
        machines[i].port = num0;
    }
    print_costs();

    int sockfd;

    // Open a socket, if successful, returns 
a
descriptor
 associated 
with
 an endpoint 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("Failure to setup an endpoint socket");
      exit(1);
    }

    //Setup the server address to bind using socket addressing structure
    for (i = 0; i < N; i++) {
        servAddr[i].sin_family = AF_INET;
        servAddr[i].sin_port = htons(machines[i].port);
        servAddr[i].sin_addr.s_addr = INADDR_ANY;
    }

    //Set address/port of server endpoint for socket socket descriptor
    if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
      perror("Failure to bind server address to the endpoint socket");
      exit(1);
    }

    // Thread 1
    pthread_create(&threads[0], NULL, thread1, (void *)&servAddr[startRouter - 1]);
    // Thread 2
    pthread_create(&threads[1], NULL, thread2, (void *)servAddr);
    // Thread 3
    pthread_create(&threads[2], NULL, thread3, (void *)servAddr);

    // Join the three threads
    for (i = 0; i <= 2; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
