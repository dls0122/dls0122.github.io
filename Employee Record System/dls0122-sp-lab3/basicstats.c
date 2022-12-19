#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//totalArray: A array which includes all the input values(floating point).
//Numofvalues: the number of values from the totalArray.

//Get the middle value in the set.
float median(float totalArray[], int Numofvalues)
{
	if(Numofvalues % 2 == 0){
		return((totalArray[Numofvalues/2] + totalArray[Numofvalues/2 - 1]) / 2);
	} else {
		return totalArray[Numofvalues/2];
	}
}
//Get the average of values in the set
float mean(float totalArray[], int Numofvalues)
{
	float Sum = 0;
	for(int i = 0; i < Numofvalues; i++){
		Sum = Sum + totalArray[i];
	}
	float mean = (Sum/Numofvalues);
	return mean;
}
//Get the standard deviation.
float stddev(float totalArray[], int Numofvalues)
{
	float Stddev = 0;
	float Mean = mean(totalArray, Numofvalues);

	for(int i = 0; i < Numofvalues; ++i){  
		Stddev += pow((totalArray[i] - Mean), 2);
	}
	Stddev = sqrt(Stddev/Numofvalues);
	return Stddev;
}
//Swap function: swap the values of x and y.
void swap(float *x, float *y){
	float temp = *x;
	*x = *y;
	*y = temp;
}
//Get the sorted array. 
void sortedarray(float totalArray[], int Numofvalues){ 
	for (int i = 0; i < Numofvalues-1; i++){
		for(int j = 0; j < Numofvalues-i-1; j++){
			if(totalArray[j] > totalArray[j+1])
				swap(&totalArray[j], &totalArray[j+1]);
		}
	}
}
//Resize the totalArray. 
float *resizedarray(float totalArray[], int *arraylength)
{
	float *tempArray = (float *)malloc(sizeof(float) * *arraylength*2);	
	for(int i = 0; i < *arraylength; i++){
		tempArray[i] = totalArray[i];
	}
	
	*arraylength = *arraylength * 2;
	return tempArray;
}
//Print out the results
void print_file(float mean, float median, float stddev, int Numofvalues, int arraylength)
{
	printf("\nResults:\n________\n");
	printf("\nNum values:\t%d\n", Numofvalues);
    printf("mean:\t\t%f\n",mean);
	printf("median:\t\t%f\n", median);
    printf("stddev:\t\t\%f\n",stddev);
	printf("________\n");
	printf("Unused Array Capacity: %d\n", arraylength-Numofvalues);
}


void results(float *MEAN, float *MEDIAN, float *STDDEV, int Numofvalues, float totalArray[])
{
	*MEAN = mean(totalArray, Numofvalues);
	*MEDIAN = median(totalArray, Numofvalues);
	*STDDEV = stddev(totalArray, Numofvalues);
}

//FP: pointer to the INPUT file.
float * read_file(FILE *FP)
{
	int Numofvalues = 0; 
	int arraylength = 20; //From the Lab Description
	float *totalArray,*tempArray;
	float MEAN, MEDIAN, STDDEV;
	
	totalArray = (float *)malloc(sizeof(float)*arraylength);

	while(fscanf(FP, "%f", &totalArray[Numofvalues]) != EOF){		
		Numofvalues++;	
		if(Numofvalues >= arraylength){
			tempArray = resizedarray(totalArray, &arraylength);
			free(totalArray);
			totalArray = tempArray;		
		}			
	}
	sortedarray(totalArray, Numofvalues);
	results(&MEAN, &MEDIAN, &STDDEV, Numofvalues, totalArray);
	print_file(MEAN, MEDIAN, STDDEV, Numofvalues, arraylength);
	return totalArray;
}

int main(int argc, char** argv){
	FILE *FP;
	float *totalArray;

	FP = fopen(argv[1], "r");
	totalArray = read_file(FP);	
	free(totalArray);
	return 0;
}