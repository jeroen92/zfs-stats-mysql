#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>

// Constants
#define MAX_WORDSIZE 30
#define COLUMNS 7

// Defining structs
struct data {
	char* columnName;
	long int value;
};

// Type definitions
typedef enum boolean { true, false } boolean;
typedef enum environment { osx, bsd } environment;
typedef struct data DATA;
typedef DATA *DATAPTR;

// Global variables
environment env;
static DATA COLLECTION[COLUMNS] = {"l2_io_error", 0, "l2_cksum_bad", 0, "l2_size", 0, "l2_hdr_size", 0, "l2_hits", 0, "l2_misses", 0, "l2_write_buffer_bytes_scanned", 0};
char DB_HOST[50];
char DB_UNAME[50];
char DB_PASSWD[50];
char DB_SCHEMA[50];

// Function prototypes
void addCharToWord(char, char[]);
void fillStruct(char[], char[]);
char* trimWhiteSpace(char*);