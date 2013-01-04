#include "main.h"

int main() {
	// Clone to a child process
	pid_t pid = fork();
	
	// Check if something went wrong when forking
	if (pid < 0) exit(EXIT_FAILURE);
	
	// Forking succeeded, kill parent. Continue with child process
	if (pid > 0) exit(EXIT_SUCCESS);
	
	pid_t sid;
	
	// Create own process group
	sid = setsid();
	if (sid < 0) {
		// TBD: Write to log message
		exit(EXIT_FAILURE);
	}
	
	// Closing file descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	// Daemon doing its job..
	while (1) {	
		run();
		sleep(5);
	}	
	return 0;
}

int run() {
	
	FILE* output;
	
	// OSX Testing with file
	//output = fopen("file", "r");
	
	if(!(output = popen("/sbin/sysctl -q 'kstat.zfs.misc.arcstats'", "r"))){
		return 1;
	}
	
	char c;
	char word[MAX_WORDSIZE];
	char value[MAX_WORDSIZE];
	int valueReached = 0;
	DATA result;
		
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
	int i;
	char queryString[1024];
	
	snprintf(queryString, 1024, "INSERT INTO struct_l2arc (date, %s, %s, %s, %s, %s, %s, %s) VALUES (NOW(), '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld')", COLLECTION[0].columnName, COLLECTION[1].columnName, COLLECTION[2].columnName, COLLECTION[3].columnName, COLLECTION[4].columnName, COLLECTION[5].columnName, COLLECTION[6].columnName, COLLECTION[0].value, COLLECTION[1].value, COLLECTION[2].value, COLLECTION[3].value, COLLECTION[4].value, COLLECTION[5].value, COLLECTION[6].value);
		
	//printf("%s\n", queryString);
	executeQuery(queryString);
	fclose(output);
}

void addCharToWord(char c, char word[]) {
	int length = strlen(word);
	if (length > MAX_WORDSIZE) {
		return;
	};
	word[length] = c;
	word[length+1] = '\0';
}

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