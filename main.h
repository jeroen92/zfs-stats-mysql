#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORDSIZE 50
#define SLEEPCYCLE 1800
#define COLUMNS 7

// Defining structs
struct data {
	char* columnName;
	long int value;
};

// Key->value struct for settings
typedef struct configPair {
	char* key;
	char value[50];
} CONFIGPAIR, *CONFIGPAIRPTR;

// Struct to directly access the configPairs
typedef struct configData {
	CONFIGPAIRPTR dbhost;
	CONFIGPAIRPTR dbuser;
	CONFIGPAIRPTR dbpasswd;
	CONFIGPAIRPTR dbschema;
} CONFIGDATA, *CONFIGDATAPTR;

// Type definitions
typedef enum { true, false } BOOLEAN;
typedef enum {OSX, BSD} ENVIRONMENT;
typedef struct data DATA;
typedef DATA *DATAPTR;

// Global constants
static const ENVIRONMENT EXEC_ENVIRONMENT = BSD;
static const char* SETTINGSFILENAME = "settings.xml";

// Global non-constant array
extern DATA COLLECTION[COLUMNS];

// Global structs
extern CONFIGPAIR dbconfig[4];

// Variable to directly access a configpair by name, eg. configData.dbhost->..
extern CONFIGDATA configData;

// Function prototypes
int startDaemon();
void addCharToWord(char, char[]);
void fillStruct(char[], char[]);
char* trimWhiteSpace(char*);
void parseSettingsFile(FILE*);
void parseStatisticsFile(FILE*);
int createSettingsFile(const char[]);
void fillConfig(char[], char[]);
void promptUser();
int checkIfTableExists(char*);
int createDatabaseTables();
