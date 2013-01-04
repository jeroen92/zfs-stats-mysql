#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORDSIZE 30
#define COLUMNS 7

// Database settings
#define DB_HOST ""
#define DB_UNAME ""
#define DB_PASSWD ""
#define DB_SCHEMA ""

struct data {
	char* columnName;
	long int value;
};

typedef struct data DATA;
typedef DATA *DATAPTR;

static DATA COLLECTION[COLUMNS] = {"l2_io_error", 0, "l2_cksum_bad", 0, "l2_size", 0, "l2_hdr_size", 0, "l2_hits", 0, "l2_misses", 0, "l2_write_buffer_bytes_scanned", 0};

void addCharToWord(char, char[]);
void fillStruct(char[], char[]);