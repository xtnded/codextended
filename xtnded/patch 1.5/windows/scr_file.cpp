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

#include "script.hpp"

void GScr_fopen(int entityIndex) {
    char* name = Scr_GetString(0);
    char* mode = Scr_GetString(1);
    FILE* f = fopen(name, mode);
    if(f)
        Scr_AddInt((int)(f));
    else
        Scr_AddInt(-1);
}

void GScr_fexists(int entityIndex) {
    char* name = Scr_GetString(0);
    if(FILE *f = fopen(name, "r")) {
        fclose(f);
        Scr_AddBool(true);
    } else {
        Scr_AddBool(false);
    }
}

void GScr_fread(int entityIndex) {
    int len = Scr_GetInt(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    if(f == NULL) {
        Scr_AddInt(-1);
    } else {
        char txt[len];
        size_t txt_c = 0;
        int c;
        while((c = fgetc(f)) != EOF)
            txt[txt_c++] = c;
        Scr_AddString(txt);
    }
}

void GScr_fwrite(int entityIndex) {
    char* text = Scr_GetString(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    if(f == NULL) {
        Scr_AddBool(false);
    } else {
        Scr_AddBool(true);
        fprintf(f, text);
    }
}

void GScr_fsize(int entityIndex) {
    char* name = Scr_GetString(0);
    FILE *f = fopen(name, "r");
    size_t len;
    if(f) {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        rewind(f);
        Scr_AddInt((int)len);
    } else {
        Scr_AddInt(-1);
    }
}

void GScr_fclose(int entityIndex) {
    FILE *f = (FILE*)(Scr_GetInt(0));
    if(f) {
        Scr_AddBool(true);
        fclose(f);
    } else {
        Scr_AddBool(false);
    }
}
