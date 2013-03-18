#include "main.h"

int main() {
	// Configure the program settings
	if (initializeSettings() != 0) return 1;
	// Begin daemon, no more user interaction from now on!

	// Try to start the daemon. If env = OSX, case will be 99. To escape this, change EXEC_ENVIRONMENT in main.h to BSD or SOL.
	printf("Starting daemon...\nKill this process, continue on child...\n");
	switch(startDaemon()) {
	case 0:
		printf("The daemon was started successfully!\n");
		break;
	case 1:
		printf("Failed to start the daemon!\n");
		return 1;
	case 99:
		printf("Daemon was not started due to the selected environment.\n");
		break;
	default:
		printf("Undefined error\n");
		return 1;
	}

	while (1) {	
		run();
		sleep(SLEEPCYCLE);
	}	
	return 0;
}

/*
 *	Collect, parse and send the ZFS statistics to the specified
 *	MySQL database.
 */
int run() {
	FILE* output;
	switch(EXEC_ENVIRONMENT) {
		case OSX:
			if(!(output = fopen("file", "r"))) {
				printf("niet gelukt!");
				return 1;
			}
			parseKstatStatisticsFile(output);
			break;
		case BSD:
			if(!(output = popen("/sbin/sysctl -q 'kstat.zfs.misc.arcstats'", "r"))) return 1;
			parseSysctlStatisticsFile(output);
			break;
		case SOL:
			if(!(output = popen("/bin/kstat -m zfs -n arcstats -p'", "r"))) return 1;
			parseKstatStatisticsFile(output);
			break;
		default:
			printf("No environment selected!\n");
			return 1;
	}
	char queryString[1024];
	snprintf(queryString, 1024, "INSERT INTO l2arc_stats (date, %s, %s, %s, %s, %s, %s, %s) VALUES (NOW(), '%ld', '%ld', '%ld', '%ld', '%ld', '%ld', '%ld')", COLLECTION[0].columnName, COLLECTION[1].columnName, COLLECTION[2].columnName, COLLECTION[3].columnName, COLLECTION[4].columnName, COLLECTION[5].columnName, COLLECTION[6].columnName, COLLECTION[0].value, COLLECTION[1].value, COLLECTION[2].value, COLLECTION[3].value, COLLECTION[4].value, COLLECTION[5].value, COLLECTION[6].value);
	if (executeQuery(queryString) != 0) return 1;
	if (collectZpoolStats() != 0) exit(1);
	return 0;
}

/*
 *	Create a daemon, creates child and terminates parent.
 *	User interaction won't be possible after execution. (stdin/stdout etc)
 */
int startDaemon() {
	// Dont start the daemon when testing, to avoid using the kill command after eacht test
	if (EXEC_ENVIRONMENT == OSX) {
		return 99;
	}
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
		// TBD: Write to log
		return 1;
	}
	
	// Closing file descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	return 0;
}
