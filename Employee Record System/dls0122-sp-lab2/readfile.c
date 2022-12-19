#include <stdio.h>
#include <stdlib.h>
#include "readfile.h"


int open_file( char filename[], FILE **filep ){

	*filep = fopen(filename, "r+"); 
	
	if(filep == NULL){ 
		printf("File not found within directory.\n");
		return -1;	
	}

	printf("File found.\n");
	return 0;
}

int close_file(FILE *filep){	
	if(filep == NULL){
		printf("Error - no file to close.");
		return -1;
	}

	fclose(filep);
	return 0;
}

int read_float(float *f, FILE *filep){  
	while(fscanf(filep, "%f", f) != EOF){ 
		return 0;
	}

	printf("End-of-file detected.\n");	
	return -1;  
}

int read_int(int *i, FILE *filep){
	while(fscanf(filep, "%d ", i) != EOF){ 
			return 0;
	}	

	printf("End-of-file detected.\n");	
	return -1;
}

int read_string(char s[], FILE *filep){
	while(fscanf(filep, "%s", s) != EOF){  
		return 0;
	}

	printf("End-of-file detected.\n");	
	return -1;				
}