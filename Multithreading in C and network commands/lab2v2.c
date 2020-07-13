//Name:JayShah
//1-17-20
//Lab2:part1


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define N 3
#define M 3
#define L 3

double matrixA[N][M];
double matrixB[M][L];
double matrixC[N][L];


void *cRoutine(void *arg){
    int j, k;
    int i=*(int*)arg;
    double temp;
    for (j=0;j<L;j++){
	temp=0;
	for (k=0;k<M;k++){
	    temp+=(matrixA[i][k] * matrixB[k][j]);
	}
	matrixC[i][j] = temp;
    }
    return NULL;
}

int main(){
    srand(time(NULL));
    int i,j;
    printf("\n");
    for (i=0;i<N;i++){
	for (int j=0;j<M;j++){
	    matrixA[i][j] = rand()%10;
	    printf("%f\t", matrixA[i][j]);
	}
	printf("\n");
    }
    printf("\n");
    for (i=0;i<M;i++){
	for (int j=0;j<L;j++){
	    matrixB[i][j] = rand()%10;
	    printf("%f\t", matrixB[i][j]);
	}
	printf("\n");
    }
    for (j=0;j<L;j++){
	double temp=0;
	for (int k=0;k<M;k++){
	    temp += matrixA[i][k] * matrixB[k][j];
	}
    matrixC[i][j] = temp;
    }
    pthread_t threadId[N];
    int index[N];
    for (i=0;i<N;i++){
	index[i] = i;
	pthread_create(&threadId[i],NULL,cRoutine,(void*)&index[i]);
    }
    for (i=0;i<N;i++){
	pthread_join(threadId[i],NULL);
    }
    printf("\n");
    for (i=0;i<N;i++){
	for (j=0;j<L;j++){
	    printf("%f\t",matrixC[i][j]);
	}
    printf("\n");
    }
}
