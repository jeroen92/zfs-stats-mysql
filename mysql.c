#include "main.h"

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
extern CONFIGPAIR dbconfig[4];

/*	Try to initiate a mysql connection with the global variables defined in main.h */
int initiateMySQL() {
	conn = mysql_init(NULL);

	// Poging doen tot verbinden
	if (!mysql_real_connect(conn, configData.dbhost->value, configData.dbuser->value, configData.dbpasswd->value, configData.dbschema->value, 3306, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return 1;
	}
	return 0;
}

/*	Executes the provided char array as a query. Returns 1 on failure */
int executeQuery(char* query) {
	initiateMySQL();
	
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 1;
	}
	res = mysql_use_result(conn);
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}

/*	Executes the provided char array as a query. For test purposes only */
int printQuery(char* query) {
	initiateMySQL();
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 1;
	}
	res = mysql_use_result(conn);
	int i = 0;
	while ((row = mysql_fetch_row(res)) != NULL) {
    	printf("%s \n", row[i]);
    	i++;
    }
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}

/*
 *	Check in the DB if the table specified in the char* param exists.
 *	Returns 0 if it exists, and 1 if it doesn't.
 */
int checkIfTableExists(char* tableName) {
	char queryString[100];
	snprintf(queryString, 100, "show tables like '%s'", tableName);
	initiateMySQL();
	if (mysql_query(conn, queryString)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 1;
	}
	res = mysql_store_result(conn);
	if (mysql_num_rows(res) >0) {
		return 0;
	}
	mysql_free_result(res);
	mysql_close(conn);
	return 1;
}

/*	Function that queries all the column names
 *	Will be used in the future to dynamically process the data to collect based on
 *	the column names in the database
*/
int getColumnNames(DATAPTR container) {
	initiateMySQL();
	char* query = "SELECT column_name FROM information_schema.columns WHERE table_name = 'struct_l2arc'";
	if (mysql_query(conn, query)) {
		return 1;
	}
	res = mysql_use_result(conn);
	int i = 0;
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s \n", row[i]);
		i++;
	}
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}

