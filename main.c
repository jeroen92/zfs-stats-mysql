#include "main.h"

int main() {
	init();
	
	// Begin daemon, no more user interaction from now on!
	if (demoniseer() != 0) return 1;

	// Daemon doing daemon things...
	while (1) {	
		run();
		sleep(5);
	}	
	return 0;
}

/*	Prompts user for DB credentials */
int init() {
	fputs("Database host:\t", stdout);
	fgets(DB_HOST, sizeof DB_HOST, stdin);
	trimWhiteSpace(DB_HOST);
	printf("\nDatabase user:\t");
	fgets(DB_UNAME, sizeof DB_UNAME, stdin);
	trimWhiteSpace(DB_UNAME);
	printf("\nUsers password:\t");
	fgets(DB_PASSWD, sizeof DB_PASSWD, stdin);
	trimWhiteSpace(DB_PASSWD);
	printf("\nSchema name:\t");
	fgets(DB_SCHEMA, sizeof DB_SCHEMA, stdin);
	trimWhiteSpace(DB_SCHEMA);
	if(!initiateMySQL()) {
		printf("Values are correct!\n");
		return 0;
	}
	printf("\n\n Entered values are not correct.\nCould not connect to the specified DB\n");
	return 1;
}

/* Execute sysctl and parse its output */
int run() {
	FILE* output;
	
	//	Testing for development on non FreeBSD OS.
	//	Open file containing output of sysctl
	//		output = fopen("file", "r");
	
	if(!(output = popen("/sbin/sysctl -q 'kstat.zfs.misc.arcstats'", "r"))){
		//	Return when sysctl returned > 0
		return 1;
	}
	char c;
	char word[MAX_WORDSIZE];
	char value[MAX_WORDSIZE];
	int valueReached = 0;
	DATA result;
	
	// Iterate trough 'file'
	while(c != EOF) {
		while((c = getc(output)) != '\n' && c != EOF) {
			if (c == '.') word[0] = '\0';
			else if (c == ':') valueReached = 1;
			else if ((c >= '0' && c <= '9') && (valueReached == 1)) addCharToWord(c, value);
			else if ((c >= 'a' && c <= 'z') || (c == '_') || (c >= '0' && c <= '9')) addCharToWord(c, word);
		}
		//printf("%s:\t%s\n", word, value);
		fillStruct(word, value);
		word[0] = '\0';
		value[0] = '\0';
		valueReached = 0;
	}
	char queryString[1024];
	snprintf(queryString, 1024, "INSERT INTO struct_l2arc (date, %s, %s, %s, %s, %s, %s, %s) VALUES (NOW(), '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld')", COLLECTION[0].columnName, COLLECTION[1].columnName, COLLECTION[2].columnName, COLLECTION[3].columnName, COLLECTION[4].columnName, COLLECTION[5].columnName, COLLECTION[6].columnName, COLLECTION[0].value, COLLECTION[1].value, COLLECTION[2].value, COLLECTION[3].value, COLLECTION[4].value, COLLECTION[5].value, COLLECTION[6].value);
	executeQuery(queryString);
	fclose(output);
}

/*
 *	@param c char to add
 *	@param word destination of the char
 *	Adds the char c at the end of the char array word
*/
void addCharToWord(char c, char word[]) {
	int length = strlen(word);
	if (length > MAX_WORDSIZE) {
		return;
	};
	word[length] = c;
	word[length+1] = '\0';
}

/*	@param word, string to add to COLLECTION
 *	@param value, string to add to COLLECTION
 *	Compares the word with all the COLLECTION.columnName's
 *	If a match occurs, the string value is parsed to a double
 *	and added to the COLLECTION, on the position of the 
 *	matched word.
*/
void fillStruct(char word[], char value[]) {
	int i;
	for (i = 0; i < COLUMNS; i++) {
		//printf("hoi");
		if (strcmp(word, COLLECTION[i].columnName) == 0) {
			COLLECTION[i].value = strtol(value, NULL, 10);
			//printf("%s: ", COLLECTION[i].columnName);
			//printf("%ld\n", COLLECTION[i].value);
			return;
		}
	}
}

/*
 *	Create a daemon, creates child and terminates parent.
 *	User interaction won't be possible after execution. (stdin/stdout etc)
*/
int demoniseer() {
	// Clone to a child process
	pid_t pid = fork();
	
	// Check if something went wrong when forking
	if (pid < 0) return 1;
	
	// Forking succeeded, kill parent. Continue with child process
	if (pid > 0) exit(EXIT_SUCCESS);
	
	pid_t sid;
	
	// Create own process group
	sid = setsid();
	if (sid < 0) {
		// TBD: Write to log message
		return 1;
	}
	
	// Closing file descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}