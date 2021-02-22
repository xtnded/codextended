#include "../script.hpp"

void GScr_mysql_get_connection(int a1) {
	if(db != NULL) {
		Script_AddInt((int)db);
	} else {
		Script_AddUndefined();
	}
}

void GScr_mysql_init(int entityIndex) {
    MYSQL *connection = mysql_init(NULL);
    if(connection != NULL)
        Script_AddInt((int)connection);
    else
        Script_AddUndefined();
}

void GScr_mysql_real_connect(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    char* hostname = Script_GetString(1);
    char* username = Script_GetString(2);
    char* password = Script_GetString(3);
    char* database = Script_GetString(4);
    int port = Script_GetInt(5);
    if(m != NULL) {
        MYSQL *c = mysql_real_connect(m, hostname, username, password, database, port, NULL, 0);
        Script_AddInt((int)c);
    } else {
        Script_AddUndefined();
    }
}

void GScr_mysql_error(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    Script_AddString((char*)mysql_error(m));
}

void GScr_mysql_errno(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    Script_AddInt(mysql_errno(m));
}

void GScr_mysql_close(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    mysql_close(m);
    Script_AddUndefined();
}

void GScr_mysql_query(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    char* query = Script_GetString(1);
    Script_AddInt((int)mysql_query(m, query));
}

void GScr_mysql_affected_rows(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    Script_AddInt((int)mysql_affected_rows(m));
}

void GScr_mysql_store_result(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    Script_AddInt((int)mysql_store_result(m));
}

void GScr_mysql_num_rows(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Script_GetInt(0);
    Script_AddInt((int)mysql_num_rows(m));
}

void GScr_mysql_num_fields(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Script_GetInt(0);
    Script_AddInt((int)mysql_num_fields(m));
}

void GScr_mysql_field_seek(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Script_GetInt(0);
    int offset = Script_GetInt(1);
    Script_AddInt((int)mysql_field_seek(m, offset));
}

void GScr_mysql_fetch_field(int entityIndex) {
    MYSQL_RES *res = (MYSQL_RES*)Script_GetInt(0);
    MYSQL_FIELD *f = mysql_fetch_field(res);
    if(!f)
        Script_AddUndefined();
    else
        Script_AddString(f->name);
}

void GScr_mysql_fetch_row(int a1) {
	MYSQL_RES* result = (MYSQL_RES*)Script_GetInt(0);
	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row) {
		Script_AddUndefined();
		return;
	}

	Script_MakeArray();
	
	int num = mysql_num_fields(result);
	for (int i = 0; i < num; i++) {
		if (row[i] == NULL)
			Script_AddUndefined();
		else
			Script_AddString(row[i]);
		Script_AddArray();
	}
}

void GScr_mysql_free_result(int entityIndex) {
    MYSQL_RES *res = (MYSQL_RES*)Script_GetInt(0);
    mysql_free_result(res);
    Script_AddUndefined();
}

void GScr_mysql_real_escape_string(int entityIndex) {
    MYSQL *m = (MYSQL*)Script_GetInt(0);
    char* escape = Script_GetString(1);
    char* str = (char*)malloc(strlen(escape) * 2 + 1);
    mysql_real_escape_string(m, str, escape, strlen(escape));
    Script_AddString(str);
    free(str);
}
