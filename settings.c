#include "main.h"

// Initializing variables declared as extern in main.h
ENVIRONMENT EXEC_ENVIRONMENT = OSX;
CONFIGPAIR dbconfig[5] = {
		{"dbhost"}, {"dbuser"}, {"dbpasswd"}, {"dbschema"}, {"poolname"}};
CONFIGDATA configData = {&dbconfig[0], &dbconfig[1], &dbconfig[2], &dbconfig[3], &dbconfig[4]};
DATA COLLECTION[COLUMNS] = {
		{"l2_io_error", 0},
		{"l2_cksum_bad", 0},
		{"l2_size", 0},
		{"l2_hdr_size", 0},
		{"l2_hits", 0},
		{"l2_misses", 0},
		{"l2_write_buffer_bytes_scanned", 0}
};

/*
 * 	Checks if saved settings exists. If not, create a
 * 	settings.xml file and prompts user to enter the
 * 	correct settings, which will be saved in the settings.xml
 *
 * 	If file exists, parse the file and load settings.
 */
int initializeSettings() {
	FILE* settingsFile;
	
	// Retrieve the OS environment
	char environmentInput[4];
	printf("Please enter your OS environment. The supported options are BSD and SOL, meaning FreeBSD and Solaris\n");
	printf("Operating system:\t");
	fgets(environmentInput, sizeof environmentInput, stdin);
	trimWhiteSpace(environmentInput);
	if (strcmp(environmentInput, "OSX") == 0) EXEC_ENVIRONMENT = OSX;
	else if (strcmp(environmentInput, "BSD") == 0) EXEC_ENVIRONMENT = BSD;
	else if (strcmp(environmentInput, "SOL") == 0) EXEC_ENVIRONMENT = SOL;
	else {
		printf("Incorrect value. Only BSD and SOL are valid. Exiting...\n");
		return 1;
	}
	
	// Try to open config file. If exists, load configuration
	if (settingsFile = fopen(SETTINGSFILENAME, "r")) {
		// Settings.xml exists. parsing...
		settingsFile = fopen(SETTINGSFILENAME, "r");
		parseSettingsFile(settingsFile);
	} else {
		// No settings.xml available. create one
		printf("No config file found, creating one...\n");
		if ((settingsFile = fopen(SETTINGSFILENAME, "a")) != NULL) {
			/*if (promptUser() == 0) {
				createSettingsFile(SETTINGSFILENAME);
			}
			else {
				return 1;
			}*/
			promptUser();
			createSettingsFile(SETTINGSFILENAME);
		} else {
			// Creating settings.xml failed
			printf("Failed to create settings file\n");
			return 1;
		}
	}

	// Testing the filled in values
	if(!initiateMySQL()) {
		printf("Database entries are correct.\n");
		printf("Verifying existence of tables...\n");
		if (checkIfTableExists("l2arc_stats") == 0) {
			printf("ZFS Stats tables are present.\n");
		} else {
			printf("Tables are not yet present, creating them...\n");
				if (createDatabaseTables() == 0) {
					printf("Table creation succeeded :)\n");
				} else return 1;
		}
		return 0;
	} else {
		printf("\n\n Entered values are not correct.\nCould not connect to the specified DB\n");
		return 1;
	}
}

/*
 * 	Prompts user to enter the OS and the database's:
 * 	host, user, password and name.
 * 	Puts the received text in configData.dbhost.
 */
void promptUser() {
	// Flush the stdin input buffer
	int temp;
	while ((temp = getchar()) != '\n' && temp != EOF);
	
	printf("\nDatabase host:\t");
	fgets(configData.dbhost->value, sizeof configData.dbhost->value, stdin);
	trimWhiteSpace(configData.dbhost->value);

	printf("\nDatabase user:\t");
	fgets(configData.dbuser->value, sizeof configData.dbuser->value, stdin);
	trimWhiteSpace(configData.dbuser->value);

	printf("\nUsers password:\t");
	fgets(configData.dbpasswd->value, sizeof configData.dbpasswd->value, stdin);
	trimWhiteSpace(configData.dbpasswd->value);

	printf("\nSchema name:\t");
	fgets(configData.dbschema->value, sizeof configData.dbschema->value, stdin);
	trimWhiteSpace(configData.dbschema->value);

	printf("\nZFS Pool name:\t");
        fgets(configData.poolname->value, sizeof configData.poolname->value, stdin);
        trimWhiteSpace(configData.poolname->value);
	return;
}

/*
 * Fill the settings.xml file, given as the FILE* param. Iterates
 * trough the dbconfig array. Every iteration is a new XML <key> and
 * value, and so they will be written to the settings.xml.
 * All the written XML nodes are part of the master node <config>
 */
int createSettingsFile(const char settingsFilename[]) {
	int i;
	FILE* configFile = fopen(settingsFilename, "a");
	//printf("%s", dbconfig[1].value);
	fwrite("<config>\n", sizeof(char), strlen("<config>\n"), configFile);
	for (i = 0; i < (sizeof(dbconfig)/sizeof(CONFIGPAIR)); i++) {
		fwrite("\t<", sizeof(char), strlen("\t<"), configFile);
		fwrite(dbconfig[i].key, sizeof(char), strlen(dbconfig[i].key), configFile);
		fwrite(">\n\t\t", sizeof(char), strlen(">\n\t\t"), configFile);
		fwrite(dbconfig[i].value, sizeof(char), strlen(dbconfig[i].value), configFile);
		fwrite("\n\t</", sizeof(char), strlen("\n\t</"), configFile);
		fwrite(dbconfig[i].key, sizeof(char), strlen(dbconfig[i].key), configFile);
		fwrite(">\n", sizeof(char), strlen(">\n"), configFile);
	}
	fwrite("</config>\n", sizeof(char), strlen("</config>\n"), configFile);
	fclose(configFile);
	return 0;
}

/*
 *	Create the required tables in the specified database.
 *	Return 0 on success and 1 on failure.
 */
int createDatabaseTables() {
	char* queryString = "CREATE TABLE `l2arc_stats` (`date` datetime NOT NULL,`l2_hits` double DEFAULT NULL,`l2_misses` double DEFAULT NULL,`l2_cksum_bad` double DEFAULT NULL,`l2_io_error` double DEFAULT NULL,`l2_size` double DEFAULT NULL,`l2_hdr_size` double DEFAULT NULL,`l2_write_buffer_bytes_scanned` double DEFAULT NULL,PRIMARY KEY (`date`)) ENGINE=InnoDB DEFAULT CHARSET=latin1";
	if(executeQuery(queryString) != 0)
		return 1;
	queryString = "CREATE TABLE `io_stats` (`date` datetime NOT NULL,`iop_read` BIGINT DEFAULT NULL,`iop_write` BIGINT DEFAULT NULL,`bandwidth_read` BIGINT DEFAULT NULL,`bandwidth_write` BIGINT DEFAULT NULL,`space` BIGINT DEFAULT NULL,`space_alloc` BIGINT DEFAULT NULL,`checksum_errors` BIGINT DEFAULT NULL,`state` BIGINT DEFAULT NULL, PRIMARY KEY (`date`)) ENGINE=InnoDB DEFAULT CHARSET=latin1";
	if(executeQuery(queryString) != 0)
		return 1;
	return 0;
}
