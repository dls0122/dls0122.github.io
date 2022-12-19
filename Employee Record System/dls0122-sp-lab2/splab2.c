#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"

void insertionSort(employee eArray[], int *num)
{ 
	int i, j;
	employee temporary;
	for (i = 1; i < *num; i++) {
		temporary = eArray[i];
		j = i - 1;
		while (j >= 0 && eArray[j].id > temporary.id) {
			eArray[j+1] = eArray[j];
			j = j-1;
		}
		eArray[j+1]=temporary;	
	}
}

employee* create_array(int *num, FILE *filep)
{   
	static employee *eArray;
	eArray = (employee *)malloc(sizeof(employee)*MAXEMP); 
	if (eArray == 0)
	{
		printf("Error: out of memory\n");
		exit(1);
	}

	int errornum = 0; 
	while(errornum == 0) {  
		errornum = read_int(&eArray[*num].id, filep); 
		if(errornum == 0) {
			errornum = read_string(eArray[*num].firstname, filep);
			if(errornum == 0) {
				errornum = read_string(eArray[*num].lastname, filep);
				if(errornum == 0) {
					errornum = read_int(&eArray[*num].salary, filep);
					++*num;				
				}
			}		
		}
	}
	printf("\nEnd of File detected.  %d number of employees entered\n\n", *num);
	insertionSort(eArray, num);
	return eArray; 
}

void print_database(int *num, employee eArray[])
{
	printf("      Name   				        Salary		   	   ID\n");
	printf("*********************************************\n");
	int i;
	for(i=0; i < *num; i++){ 
			printf("%10s	%10s			%6d			%6d\n", eArray[i].firstname, eArray[i].lastname, eArray[i].salary, eArray[i].id);
	}
	printf("*********************************************\n");
	printf("%15d employees entered\n", *num);
}

void print_employee(employee emp)
{
	printf("\nNAME                       SALARY    ID\n");
    printf("*********************************************\n");
	printf("%10s	%10s			%6d			%6d\n", emp.firstname, emp.lastname, emp.salary, emp.id);
	printf("*********************************************\n\n");
}

int binary_search(employee eArray[], int l, int r, int x)
{
    if(r >= l)
    {
        int mid = l+ (r-l)/2;
        if (eArray[mid].id == x) return mid;
        if (eArray[mid].id > x) return binary_search(eArray, l, mid-1, x);
        return binary_search(eArray, mid+1, r, x);
    }
    return -1;
}

void search_ID(int *num, employee eArray[], int x)
{  
	int output;
	output = binary_search(eArray, 0, *num-1, x);
	if(output == -1){
		printf("ID %d not found", x);
	}
	else{
		print_employee(eArray[output]);	
	}
}

int linear_search(int num, employee eArray[], char *lastname)
{  
	int i;
	for (i = 0; i < num; i++){
		if (strcmp(eArray[i].lastname, lastname) == 0){
			return i;
		}
	}
	return -1;	
}

void search_lastname(int *num, employee eArray[], char *lastname)
{
	int output;
	output = linear_search(*num, eArray, lastname);

	if(output == -1){
		printf("Name %s not found", lastname);
	}
	else{
		print_employee(eArray[output]);
	}
} 

void add_DB(int *num, employee eArray[], int tempsalary, char tempfirstname[], char templastname[])
{
	int newID;
	newID = eArray[*num-1].id + 1;
	employee newemp;
	strcpy(newemp.firstname, tempfirstname);
	strcpy(newemp.lastname, templastname);
	newemp.id = newID;
	newemp.salary = tempsalary;
	eArray[*num] = newemp;
	++*num;
	printf("%s %s %d %d", eArray[*num-1].firstname, eArray[*num-1].lastname, eArray[*num-1].id, eArray[*num-1].salary);
}

void add_employee(int *num, employee eArray[])
{
	int tempsalary;
	char tempfirstname[MAXNAME],templastname[MAXNAME];
	int confirm;
	int loop = 0;
	while(loop == 0) {
		printf("\nEnter the first name of the employee: ");
		scanf("%s", tempfirstname);
		loop = 1;
		if (strlen(tempfirstname) >= 63){
			loop = 0;
			printf("\nName length too long, please ensure it is less than 64 characters\n");
		}
	}
	loop = 0;

	while(loop == 0) {
		printf("\nEnter the last name of the employee: ");
		scanf("%s", templastname);  
		loop = 1;
		if (strlen(templastname) >= 63){
			loop = 0;
			printf("\nName length too long, please ensure it is less than 64 characters\n");	
		}
	}
	loop = 0;

	while(loop == 0) {
		printf("\nEnter the employee's salary (30000 to 150000): ");
		scanf("%d", &tempsalary);
		if(tempsalary >= 30000 && tempsalary <= 150000){
			loop = 1;
		}
		if(loop == 0){
			printf("Non-valid salary, please enter a number between 30000 and 150000");	
		}	
	}

	printf("\nAre you sure you want to add the following employee to the database?\n%s %s, salary: %d", tempfirstname, templastname, tempsalary);
	scanf("%d", &confirm);
	if(confirm == 1) {
		add_DB(num, eArray, tempsalary, tempfirstname, templastname);
		printf("employee number now is %d", *num);
	}
	tempfirstname[0] = '\0';
	templastname[0] = '\0';
	tempsalary = 0;
	confirm = 0;
}

void remove_employee(int *num, employee eArray[])
{
	int tempid;
	int tempms;
	int numtoremove = -1;
	int i;
	

	while(numtoremove == -1){
		printf("\nPlease enter an employee ID to remove.\n");
		scanf("%d", &tempid);
		numtoremove = binary_search(eArray, 0, *num-1, tempid);	
		if(numtoremove == -1){
			printf("Employee Number not found.");
		}
		tempid = 0;
	}
	printf("\nAre you sure you want to remove this employee?  Press 1 for yes or 0 for no\n");
	scanf("%d", &tempms);
	if(tempms == 1){
		for(i = numtoremove; i < *num; i++){
			eArray[i] = eArray[i+1];
		}	
		printf("Employee %d removed", tempid);
	}
}

void update_employee(int *num, employee eArray[])
{
	
	int tempid;
	int numtoupdate;
	int updateinput;
	char updatestring[MAXNAME];
	int updateInt;

	printf("\nPlease enter an employee ID to update.\n");
	scanf("%d", &tempid);
	numtoupdate = binary_search(eArray, 0, *num-1, tempid);
	if(numtoupdate == -1){
		printf("Employee number is not valid.");
		return; 
	}
	print_employee(eArray[numtoupdate]);
	printf("\nDo you want to change the employee's first name? 1 if yes, 0 if no.\n");
	scanf("%d", &updateinput);
	if(updateinput == 1){
		printf("Please enter the first name you would like to change to:\n");
		scanf("%s", updatestring);
		strcpy(eArray[numtoupdate].firstname, updatestring);
	}
	updatestring[0] = '\0';
	updateinput = 0;

	printf("\nWould you like to change the employee's last name? 1 if yes, 0 if no.\n");
	scanf("%d", &updateinput);
	if(updateinput == 1){
		printf("Please enter the last name you would like to change to:\n");
		scanf("%s", updatestring);
		strcpy(eArray[numtoupdate].lastname, updatestring);
	}
	updatestring[0] = '\0';
	updateinput = 0;

	printf("\nWould you like to change the employee's ID number? 1 if yes, 0 if no.\n");
	scanf("%d", &updateinput);
	if(updateinput == 1){
		printf("Please enter the employee ID you'd like to change to:\n");
		scanf("%d", &updateInt);
		eArray[numtoupdate].id = updateInt;
	}
	updateInt = 0;
	updateinput = 0;

	printf("\nWould you like to change the employee's salary number? 1 if yes, 0 if no.\n");
	scanf("%d", &updateinput);
	if(updateinput == 1){
		printf("Please enter the employee's salary you'd like to change to:\n");
		scanf("%d", &updateInt);
		eArray[numtoupdate].salary = updateInt;
	}
	updateInt = 0;
	updateinput = 0;

	printf("\nNew employee information is:\n");
	print_employee(eArray[numtoupdate]);
}

void swap(employee *x, employee *y)
{
	employee temporary = *x;
	*x = *y;
	*y = temporary;
}

void selection_sort_salary(int *num, employee eArray[])
{
	int i, p, max;
	
	for (i = 0; i < *num-1; i++){
		max = i;
		for (p = i+1; p < *num; p++){
			if(eArray[p].salary > eArray[max].salary){
				max = p;
			}
		}
		swap(&eArray[max], &eArray[i]);
	}
}

void print_salaries(int *num, employee eArray[])
{
	int empToPrint;
	employee teArray[20];
	employee empToMove;
	int i;
	int p;	

	printf("\nPlease enter how up to how many of the top salaries you'd like to print (up to 20):\n");
	scanf("%d", &empToPrint);

	selection_sort_salary(num, eArray); 
	print_database(&empToPrint, eArray);  
	insertionSort(eArray, num);
}

void print_match_lastnames(int *num, employee eArray[])
{
	char empNameToPrint[MAXNAME];
	int i;
	int p = 0;
	employee employeeMatchArray[20];

	printf("\nPlease enter a last name you'd like to print all multiples of:\n");
	scanf("%s", empNameToPrint);	
	printf("\n");
	for(i = 0; i < *num; i++){
		if(strcmp(empNameToPrint, eArray[i].lastname) == 0){
			employeeMatchArray[p] = eArray[i];
			p++;
		} 		
	}
	print_database(&p, employeeMatchArray);
		
}

void menu(int *num, employee eArray[])
{
	int input;
	char nameinput[MAXNAME];

	while(input != 5){
		printf("\nEmplyee DB Menu:\n");
        printf("**********************************\n");
        printf("1.remove an employee\n");
        printf("2.Update an employee's information\n");
        printf("3.Print the M Employees with the highest salaries\n");
        printf("4.Find all employees with matching last name.\n");
        printf("5.Quit\n");
        printf("**********************************\n");
        printf("Enter your option:");
		scanf("%d", &input);

		switch(input)
		{
			case 1: printf("You entered: %d\n", input);
			remove_employee(num, eArray);
			break;

			case 2: printf("You entered: %d\n", input);
			update_employee(num, eArray);
			break;

			case 3: printf("You entered: %d\n", input);
			print_salaries(num, eArray);
			break;

			case 4: printf("You entered: %d\n", input);
			print_match_lastnames(num, eArray);
			break;

			case 5: printf("You entered: %d\nQuitting...\n", input);
			break;

			default: printf("You entered: %d\nPlease enter a valid number.\n", input);
		}

	}
}

int main(int argc, char** argv)
{ 	
	FILE *filep;
	employee* eArray;
	int num = 0;

	if(argv[1] == NULL){
		printf("No file found - please ensure input.txt file is included\n");
		printf("Exiting...\n");
		return 0;
	}

	open_file(argv[1], &filep);	
	eArray = create_array(&num, filep);
	close_file(filep); 
	menu(&num, eArray);
	free(eArray);

	return 0;
}