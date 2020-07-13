//Jay Shah
//1-9-20
//Lab 1 Part 1





#include <stdio.h>
#include <sys/types.h>  
#include <unistd.h>     
#include <stdlib.h>     
#include <errno.h>      
#include <pthread.h>

void *childRoutine(void *arg);

int main(int argc, char *argv[]){

	int i;
	pthread_t threadId;
	int arg = atoi(argv[i]);
	pthread_create(&threadId,NULL,childRoutine,&arg);
	
	for(i = 0; i<100; i++)
	{
		printf("\t \t \tParent Thread %d\n", i);
		usleep(arg);
	}

	//pthread_join(threadId,NULL);
	return 0;

}

void *childRoutine(void *arg){

	int i, *arug=arg;
	for(i = 0; i<100; i++)
	{
		printf("Child Thread %d\n",i);
		usleep(*arug);
	}
	
}



