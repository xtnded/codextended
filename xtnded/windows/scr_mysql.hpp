/*
===========================================================================
    Copyright (C) riicchhaarrd
    Copyright (C) 1999-2005 Id Software, Inc.

    This file is part of CoDExtended source code.

    CoDExtended source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoDExtended source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/
#ifndef SCR_MYSQL_H
#define SCR_MYSQL_H

#include "script.hpp"

void GScr_isMySQLSupported(int);

#ifdef uMYSQL

void GScr_mysql_init(int);
void GScr_mysql_close(int);
void GScr_mysql_affected_rows(int);
void GScr_mysql_errno(int);
void GScr_mysql_error(int);
void GScr_mysql_fetch_field(int);
void GScr_mysql_field_seek(int);
void GScr_mysql_free_result(int);
void GScr_mysql_num_fields(int);
void GScr_mysql_num_rows(int);
void GScr_mysql_query(int);
void GScr_mysql_real_connect(int);
void GScr_mysql_real_escape_string(int);
void GScr_mysql_store_result(int);

#endif //uMYSQL

#endif // SCR_MYSQL_H
