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
#ifndef JS_H
#define JS_H

#ifdef BUILD_ECMASCRIPT

#include "shared.h"
#include "duktape/duktape.h"

void js_init();
void js_destroy();
void js_execute();

typedef struct JSMemberfield {
    char* name;
    int type;
    void (*func)(int,struct JSMemberfield*);
} JSMemberfield;

typedef enum {
    FT_INT,
    FT_STRING,
    FT_FLOAT
} memberfield_types;

extern duk_context* ctx;
extern JSMemberfield playerfields[];

#endif

#endif // JS_H
