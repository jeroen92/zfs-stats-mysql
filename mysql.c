#include "main.h"

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

int initiateMySQL() {
	
	conn = mysql_init(NULL);
	
	// Poging doen tot verbinden
	if (!mysql_real_connect(conn, DB_HOST, DB_UNAME, DB_PASSWD, DB_SCHEMA, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return 1;
	}
}

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

