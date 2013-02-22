#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORDSIZE 50
#define SLEEPCYCLE 2
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
typedef enum {OSX, BSD, SOL} ENVIRONMENT;
typedef struct data DATA;
typedef DATA *DATAPTR;

// Global constants
//static const ENVIRONMENT EXEC_ENVIRONMENT = BSD; // Old
static const char* SETTINGSFILENAME = "settings.xml";

// Global definitions
extern ENVIRONMENT EXEC_ENVIRONMENT;

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
void parseSysctlStatisticsFile(FILE*);
void parseKstatStatisticsFile(FILE*);
int createSettingsFile(const char[]);
void fillConfig(char[], char[]);
void promptUser();
int checkIfTableExists(char*);
int createDatabaseTables();
