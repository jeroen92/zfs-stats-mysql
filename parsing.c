#include "main.h"

char* trimWhiteSpace(char *inputString)
{
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