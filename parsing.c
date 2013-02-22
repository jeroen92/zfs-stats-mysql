#include "main.h"

/*
 * 	Function that erases all the whitespaces in an array of chars.
 * 	Iterates trough an array of chars (aka String), provided
 * 	with the param char*.
 */
char* trimWhiteSpace(char *inputString)	{
	char* end;
	
	// Trim leading space
	while(isspace(*inputString)) inputString++;
	
	// Whole string is whitespace
	if(*inputString == 0) return inputString;
	
	// Trim trailing space
	end = inputString + strlen(inputString) - 1;
	while(end > inputString && isspace(*end)) end--;
	
	// Write new null terminator
	*(end+1) = 0;
	
	return inputString;
}

/*
 *	For BSD systems only.
 *
 *	Function that parses every line of the file provided with
 *	the FILE* parameter. The output will be parsed as a key->value
 *	pair, and then is send to fillStruct().
 *	When it encounters a dot, the word variable is erased. Only the word
 *	after the last dot will be rememberd, and saved as the key. After the
 *	colon, the value part of the line is reached.
 *
 *	On this way, the line:
 *	kstat.zfs.misc.arcstats.mfu_hits: 3328512701
 *
 *	Will be parsed into:
 *	char* key = mfu_hits
 *	char* value = 3328512701
 */
void parseSysctlStatisticsFile(FILE* statsFile) {
	char c, key[MAX_WORDSIZE], value[MAX_WORDSIZE];
	int valueReached = 0;
	DATA result;

	// Iterate trough 'file'
	while(c != EOF) {
		while((c = getc(statsFile)) != '\n' && c != EOF) {
			if (c == '.') key[0] = '\0';
			else if (c == ':') valueReached = 1;
			else if ((c >= '0' && c <= '9') && (valueReached == 1)) addCharToWord(c, value);
			else if ((c >= 'a' && c <= 'z') || (c == '_') || (c >= '0' && c <= '9')) addCharToWord(c, key);
		}
		//printf("%s -> %s\n", key, value);
		fillStruct(key, value);
		key[0] = '\0';
		value[0] = '\0';
		valueReached = 0;
	}
	fclose(statsFile);
}

/*	
 *	For Solaris systems only.
 *
 *	Function that parses every line of the file provided with
 *	the FILE* parameter. The output will be parsed as a key->value
 *	pair, and then is send to fillStruct().
 *	When it encounters a dot, the word variable is erased. Only the word
 *	after the last dot will be rememberd, and saved as the key. After the
 *	colon, the value part of the line is reached.
 *
 *	On this way, the line:
 *	zfs:0:arcstats:prefetch_data_hits       1406046
 *
 *	Will be parsed into:
 *	char* key = prefetch_data_hits
 *	char* value = 1406046
 */
void parseKstatStatisticsFile(FILE* statsFile) {
	char c, key[MAX_WORDSIZE], value[MAX_WORDSIZE];
	int valueReached = 0;
	DATA result;

	// Iterate trough 'file'
	while(c != EOF) {
		while((c = getc(statsFile)) != '\n' && c != EOF) {
			if (c == ':') key[0] = '\0';
			else if (c == '\t') valueReached = 1;
			else if ((c >= '0' && c <= '9') && (valueReached == 1)) addCharToWord(c, value);
			else if ((c >= 'a' && c <= 'z') || (c == '_') || (c >= '0' && c <= '9')) addCharToWord(c, key);
		}
		//printf("%s -> %s\n", key, value);
		fillStruct(key, value);
		key[0] = '\0';
		value[0] = '\0';
		valueReached = 0;
	}
	fclose(statsFile);
}

/*
 * 	Function that parses every node in the file provided with
 * 	the FILE* param into the fillConfig(char*, char*) function.
 *
 * 	Every <node> it encounters, will be treated as one level upwards.
 * 	Every </node> it encounters, will be treated as one level downwards.
 * 	The level will be kept in the keyReached variable.
 *	At the second level it will start collecting data. The <node>'s name
 *	is stored in the key var. When the loop encounters a '>' char, the
 *	value of the second rows key is reached. Thus, the int keyReached will
 *	be set to one.
 *	If the loop encounters a '<' char and the next char is a '/' it will then
 *	end the value. The keyReached will be increased to three, so the program
 *	knows it is not yet at the end of the <node>. The collected key and value
 *	will be passed to fillConfig(char*, char*) and the buffers will be emptied.
 */
void parseSettingsFile(FILE* configFile) {
	char c, key[30], value[30];
	int keyReached, valueReached = 0;
		while ((c = getc(configFile)) != EOF) {
			// Start of a key node
			if (c == '<') {
				// Encountered a second level <node>
				if (keyReached == 1) {
					keyReached = 2;
				}
				// Probably encountered the ending of a second level node
				// Corresponding key is stored
				else if (valueReached == 1) {
					// If next char after the '<' is a '/', the end node is reached.
					if ((c = getc(configFile)) == '/') {
						// Process collected data
						fillConfig(key, value);
						// Empty the two var's
						strncpy(key, "", sizeof(key));
						strncpy(value, "", sizeof(value));
						keyReached = 3;
						valueReached = 0;
					}
				}
				// Entering the first level
				else {
					keyReached = 1;
					valueReached = 0;
				}
			}
			
			// Probably the end of a key node
			else if (c == '>') {
				// The end of the end key node is reached. Go one level back
				if (keyReached == 3) {
					keyReached = 1;
				}
				// The end of the first key node is reached.  Now it's the values turn
				else if (keyReached == 2 && valueReached != 1) {
					keyReached = 3;
					valueReached = 1;
				}
			}
			// Write to the value
			else if ((c != '\n') && (c != '\t') && (c != EOF) && valueReached == 1) {
				// value is reached!
				addCharToWord(c, value);
			}			
			// Write to the key
			else if (((c >= 'a' && c <= 'z') || (c >= '1' && c <= '9')) && keyReached == 2) {
				// <key> is reached!
				addCharToWord(c, key);
			}
		}
	return;
}

/*
 * This function iterates trough an array of structs. Every iteration
 * 	it compares the array.key with the parameter key. If a match occurs,
 * 	the parameter value is written into the array.value.
 */
void fillConfig(char key[], char value[]) {
	int i;
	for (i = 0; i < (sizeof(dbconfig)/sizeof(CONFIGPAIR)); i++) {
		if (strcmp(key, dbconfig[i].key) == 0) {
			// Keys are equal
			strcpy(dbconfig[i].value, value);
			return;
		}
	}
}

/*	@param word, string to add to COLLECTION
 *	@param value, string to add to COLLECTION
 *	Compares the word with all the COLLECTION.columnName's
 *	If a match occurs, the string value is parsed to a double
 *	and added to the COLLECTION, on the position of the
 *	matched word.
*/
void fillStruct(char key[], char value[]) {
	int i;
	for (i = 0; i < COLUMNS; i++) {
		if (strcmp(key, COLLECTION[i].columnName) == 0) {
			COLLECTION[i].value = strtol(value, NULL, 10);
			return;
		}
	}
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
