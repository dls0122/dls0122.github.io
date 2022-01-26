#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"readfile.h"

int menu()
{
        int option;
        int ret;
        printf("\nEmplyee DB Menu:\n");
        printf("**********************************\n");
        printf("1.Print the Database\n");
        printf("2.Lookup employee by ID\n");
        printf("3.Lookup employee by last name\n");
        printf("4.Add an Employee\n");
        printf("5.Quit\n");
        printf("**********************************\n");
        printf("Enter your option:");
        ret = read_int(&option);
        return option;
}

void printDB(struct person employee[MAXEMP], int num)
{
    printf("\nNAME                       SALARY    ID\n");
    printf("*********************************************\n");
    for(int i=0; i<num; i++){
        printf("%-10s %-10s %10d %10d \n", employee[i].firstname,employee[i].lastname,employee[i].salary,employee[i].id);
    }
    printf("*********************************************\n");
    printf("Number of employees (%d)\n\n",num);
}

//sorted by id(Since employees should be listed in increasing ID order)
int compare_person (const void *a, const void *b)
{
        struct person *c = (struct person*) a;
        struct person *d = (struct person*) b;
        return c->id - d->id;
}


//binary search to find a matching employee in the database
int binary_search(const int arr[], int l, int r, int x){
    if(r >= l){
        int mid = l+ (r-l)/2;
        if(arr[mid] == x)
            return mid;
        if(arr[mid] > x)
            return binary_search(arr, l, mid-1, x);
        return binary_search(arr, mid+1, r, x);
    }
    return -1;
}

//If one is found, print out the Employee information
void print_by_index(struct person employee[MAXEMP], int index){
    printf("\nNAME                       SALARY    ID\n");
    printf("*********************************************\n");
    printf("%-10s %-10s %10d %10d \n", employee[index].firstname,employee[index].lastname,employee[index].salary,employee[index].id);
    printf("*********************************************\n\n");
}

//lookup by id
void lookup_by_ID(struct person employee[MAXEMP], int num, int list[MAXEMP])
{
            int id;
            printf("\nEnter a 6-digit ID(between 100000 and 999999) of Employee:\n");
            scanf("%d", &id);
            int index = binary_search(list, 0, num, id);
            if(index ==-1){
                printf(" not found.\n");
            }
            else
            {
                print_by_index(employee, index);
            }
            return;
}

//compare target last name with last names in database
int search_lastname(struct person employee[MAXEMP], int num, char lastname[]){
    
    int temp=-1;
    for (int i=0; i<num; i++)
    {
        if(strcmp(employee[i].lastname, lastname)==0) temp=i;
    }
    return temp;
}

//lookup by last name
void lookup_by_lastname(struct person employee[MAXEMP], int num)
{
            char lastname[MAXNAME];
            printf("\nEnter the last name of an employee:\n");
            scanf("%s", lastname);
            int index = search_lastname(employee, num, lastname);
            if(index == -1){
                printf("Last name is not found\n");
            }
            else
            {
                print_by_index(employee, index);
            }
            return;
}

//add an employee
void add_employee(struct person (*employee)[MAXEMP], int *num, int (*list)[MAXEMP])
{
            char tempfirstname[MAXNAME], templastname[MAXNAME];
            int salary, confirm;
            int ret;
            printf("\nEnter the first name of an employee:\n");
            ret = read_string(tempfirstname);
            printf("Enter the last name of an employee:\n");
            ret = read_string(templastname);
            printf("Enter the salary of an employee (between $30,000 and $150,000):\n");
            ret = read_int(&salary);
            printf("Would you like to confirm to add this employee? 1 for Yes and 0 for No.\n");
            ret = read_int(&confirm);

            if (confirm ==1)
            {
                if((salary >= 30000) && (salary<=150000))
                {
                    int cur_id = (*employee)[(*num)-1].id;
                    int new_id = cur_id+1;
                    strcpy((*employee)[*num].firstname, tempfirstname);
                    strcpy((*employee)[*num].lastname, templastname);
                    (*employee)[*num].salary = salary;
                    (*employee)[*num].id = new_id;
                    (*list)[(*num)] = (*employee)[(*num)].id;
                    (*num)++;
                    return;
                }
                else printf("Salary input is invalid.\n"); return;
            }
            else printf("Action of add get cancelled.\n"); return;            
}

int main(int argc, char *argv[])
{

    char *fname = "input.txt", firstname[MAXNAME], lastname[MAXNAME];
    struct person employee[MAXEMP];
    int id, salary, option, index, confirm, num = 0, list[MAXEMP];

    FILE *fpointer = fopen(fname, "r");
    while(!feof(fpointer)){
        fscanf(fpointer,"%d %s %s %d\n", &employee[num].id, employee[num].firstname, employee[num].lastname, &employee[num].salary);
        list[num] = employee[num].id;
        num++;
    }
    fclose(fpointer);
    
    qsort(employee, num, sizeof(struct person), compare_person);
    
    int i=0;
    for (i=0;i<num;i++)
    {
        list[i]=employee[i].id;
    }    

    while (option!=5)
    {
        option = menu();
        if (option ==1) printDB(employee, num);
        else if(option ==2) lookup_by_ID(employee,num,list);
        else if(option ==3) lookup_by_lastname(employee, num);
        else if(option ==4) add_employee(&employee, &num, &list);
        else if (option !=5) printf("%d is not available, please try again\n", option); continue;
    }
    printf("\nThe end\n\n");
}
