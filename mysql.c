#include "main.h"

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

/*	Try to initiate a mysql connection with the global variables defined in main.h */
int initiateMySQL() {
	conn = mysql_init(NULL);
	
	// Poging doen tot verbinden
	if (!mysql_real_connect(conn, DB_HOST, DB_UNAME, DB_PASSWD, DB_SCHEMA, 3306, NULL, 0)) {
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
}

/*	Function that queries all the column names
 *	Will be used in the future to dynamically process the data to collect based on
 *	the column names in the database
*/
int getColumnNames(DATAPTR container) {
	initiateMySQL();
	char* query = "SELECT column_name FROM information_schema.columns WHERE table_name = 'struct_l2arc'";
	if (mysql_query(conn, query)) {
		return;
	}
	res = mysql_use_result(conn);
	int i = 0;
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s \n", row[i]);
		i++;
	}
	
	mysql_free_result(res);
	mysql_close(conn);
}

