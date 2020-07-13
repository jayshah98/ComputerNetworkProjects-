//Name:JayShah
//1-17-20
//Lab2:part2


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, const char *argv[]){
    int RTT_0 = 3;
    int RTT_1 = 20;
    int RTT_2 = 26;
    int RTT_HTTP = 47;

    int partA, partB, n;
    float partC1, partC2;

    partA = RTT_0+RTT_1+RTT_2+(2*RTT_HTTP);
    printf("Ammount of time elapsed from click: %d ms\n", partA);

    partB = RTT_0+RTT_1+RTT_2+(2*RTT_HTTP)+((2*RTT_HTTP));
    printf("Ammount of time elapsed w/6 referenced objects %d ms\n", partB);
    printf("Enter number of parallel tcp connections\n");
    scanf("%d",&n);

    float z =ceil(6/(float)n);
    partC1 = RTT_0+RTT_1+RTT_2+(2*RTT_HTTP)+((2*RTT_HTTP)*z);
    partC2 = RTT_0+RTT_1+RTT_2+(2*RTT_HTTP)+(RTT_HTTP*z);

    printf("Amount of time elapsed w/n persistent parallel tcp connection: %.4f ms\n",partC2);
    printf("Amount of time elapsed w/n nonpersistent parallel tcp connection: %.4f ms\n",partC1);

    return 0;
  }
