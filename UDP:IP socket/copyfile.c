//Jay Shah
//Lab 3 Part 1
//1/31/20
//Description: Copy a text file to another file

#include <stdlib.h>
#include <stdio.h>

#define SIZE 40

int main(int argc, char *argv[]) {
    if(argc<3)
	printf("Please include a source and destination\n");
    FILE *src = fopen(argv[1], "r");
    FILE *dest = fopen(argv[2], "w");
    
    char buffer[SIZE];
    int bytesread;
    
    if(src == NULL) {
	printf("Error opening source\n");
	exit(1);
    }

    if(dest == NULL) {
	printf("Error opening destination\n");
	exit(1);
    }

    while(!feof(src)) {
	bytesread = fread(&buffer, 1, sizeof(buffer), src);
	fwrite(&buffer, 1, bytesread, dest);
    }
    fclose(src);
    fclose(dest);
    printf("Copied successfully\n");
    return 0;
}
