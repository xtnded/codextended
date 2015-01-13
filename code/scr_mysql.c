/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "script.h"
#ifdef uMYSQL
void GScr_mysql_get_connection(int a1) {
	if(db != NULL) {
		Scr_AddInt((int)db);
	} else {
		Scr_AddUndefined();
	}
}

void GScr_mysql_init(int entityIndex) {
    MYSQL *connection = mysql_init(NULL);
    if(connection != NULL)
        Scr_AddInt((int)connection);
    else
        Scr_AddUndefined();
}

void GScr_mysql_real_connect(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    char* hostname = Scr_GetString(1);
    char* username = Scr_GetString(2);
    char* password = Scr_GetString(3);
    char* database = Scr_GetString(4);
    int port = Scr_GetInt(5);
    if(m != NULL) {
        MYSQL *c = mysql_real_connect(m, hostname, username, password, database, port, NULL, 0);
        Scr_AddInt((int)c);
    } else {
        Scr_AddUndefined();
    }
}

void GScr_mysql_error(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    Scr_AddString((char*)mysql_error(m));
}

void GScr_mysql_errno(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    Scr_AddInt(mysql_errno(m));
}

void GScr_mysql_close(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    mysql_close(m);
}

void GScr_mysql_query(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    char* query = Scr_GetString(1);
    Scr_AddInt((int)mysql_query(m, query));
}

void GScr_mysql_affected_rows(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    Scr_AddInt((int)mysql_affected_rows(m));
}

void GScr_mysql_store_result(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    Scr_AddInt((int)mysql_store_result(m));
}

void GScr_mysql_num_rows(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Scr_GetInt(0);
    Scr_AddInt((int)mysql_num_rows(m));
}

void GScr_mysql_num_fields(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Scr_GetInt(0);
    Scr_AddInt((int)mysql_num_fields(m));
}

void GScr_mysql_field_seek(int entityIndex) {
    MYSQL_RES *m = (MYSQL_RES*)Scr_GetInt(0);
    int offset = Scr_GetInt(1);
    Scr_AddInt((int)mysql_field_seek(m, offset));
}

void GScr_mysql_fetch_field(int entityIndex) {
    MYSQL_RES *res = (MYSQL_RES*)Scr_GetInt(0);
    MYSQL_FIELD *f = mysql_fetch_field(res);
    if(!f)
        Scr_AddUndefined();
    else
        Scr_AddString(f->name);
}

void GScr_mysql_fetch_row(int a1) {
	MYSQL_RES* result = (MYSQL_RES*)Scr_GetInt(0);
	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row) {
		Scr_AddUndefined();
		return;
	}

	Scr_MakeArray();
	
	int num = mysql_num_fields(result);
	for (int i = 0; i < num; i++) {
		if (row[i] == NULL)
			Scr_AddUndefined();
		else
			Scr_AddString(row[i]);
		Scr_AddArray();
	}
}

void GScr_mysql_free_result(int entityIndex) {
    MYSQL_RES *res = (MYSQL_RES*)Scr_GetInt(0);
    mysql_free_result(res);
}

void GScr_mysql_real_escape_string(int entityIndex) {
    MYSQL *m = (MYSQL*)Scr_GetInt(0);
    char* escape = Scr_GetString(1);
    char* str = (char*)malloc(strlen(escape) * 2 + 1);
    mysql_real_escape_string(m, str, escape, strlen(escape));
    Scr_AddString(str);
    free(str);
}
#endif
