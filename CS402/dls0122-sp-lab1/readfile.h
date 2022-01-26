#define MAXNAME 64
#define MAXEMP 1024

struct person{
    char firstname[MAXNAME], lastname[MAXNAME];
    int id, salary;
};

int open_file(char *fname){
    if(fopen(fname, "r") == NULL)
        return -1;
    return 0;
}

int read_int(int *address){
    if(scanf("%d", address) ==0)
        return -1;
    return 0;
}

int read_float(float *tempvalue){
    if(scanf("%f", tempvalue) ==0)
        return -1;
    return 0;
}

int read_string(char *address){
    if(scanf("%s", address) == 0)
        return -1;
    return 0;
}

void close_file(FILE *fname){
    fclose(fname);
}