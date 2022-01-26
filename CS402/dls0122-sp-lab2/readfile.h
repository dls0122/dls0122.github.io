#define MAXNAME 64  
#define MAXEMP 1024 

typedef struct employee {
	char firstname[MAXNAME], lastname[MAXNAME];
    int id, salary;
} employee;

int open_file( char filename[], FILE **filep ); 

int close_file(FILE *filep); 

int read_float(float *f, FILE *filep);

int read_int(int *i, FILE *filep);

int read_string(char s[], FILE *filep); 