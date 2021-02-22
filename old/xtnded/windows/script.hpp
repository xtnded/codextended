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

/*
    //TODO
    - Add script errors for when a function or method is called with wrong amount of parameters or e.g
    "cannot cast %s to <type>"
*/

#ifndef SCRIPT_H
#define SCRIPT_H

#include "codpatch.hpp"
#include "scr_mysql.hpp"
#include "scr_file.hpp"
#include "scr_playermethod.hpp"
#include "scr_entitymethod.hpp"

#define STACK_UNDEFINED 0
#define STACK_STRING 1
#define STACK_LOCALIZED_STRING 2
#define STACK_VECTOR 3
#define STACK_FLOAT 4
#define STACK_INT 5
#define STACK_CODEPOS 6
#define STACK_OBJECT 7
#define STACK_KEY_VALUE 8
#define STACK_FUNCTION 9
#define STACK_STACK 10
#define STACK_ANIMATION 11
#define STACK_THREAD 12
#define STACK_ENTITY 13
#define STACK_STRUCT 14
#define STACK_ARRAY 15
#define STACK_DEAD_THREAD 16
#define STACK_DEAD_ENTITY 17
#define STACK_DEAD_OBJECT 18

typedef int (*loadCallback_t)(char*,char*); //loadcallbackscript
typedef int (*dword_20031760_t)();
typedef int (*Scr_ExecThread_t)(int argc, int threadindex);
typedef int (*Scr_ExecEntThread_t)(int argc, int threadindex, int, int a1); //ex 0-64 0 8910 9
typedef int (*Scr_FreeThread_t)(int threadindex);

extern loadCallback_t loadCallback;
extern dword_20031760_t dword_20031760;
extern Scr_ExecThread_t Scr_ExecThread;
extern Scr_ExecEntThread_t Scr_ExecEntThread;
extern Scr_FreeThread_t Scr_FreeThread;

extern int callbackTest;
extern int callbackPlayerDamage;

typedef void (__cdecl *Scr_FunctionCall)(int);

typedef struct
{
	const char* name;
	Scr_FunctionCall call;
	int developer;
} Scr_Function;

/*
typedef Scr_FunctionCall (__cdecl *Scr_GetFunction_t)(const char**, int*);
Scr_GetFunction_t Scr_GetFunction = (Scr_GetFunction_t)0x20037A00;

typedef Scr_FunctionCall (__cdecl *Scr_GetMethod_t)(const char**, int*);
Scr_GetMethod_t Scr_GetMethod = (Scr_GetMethod_t)0x20037B00;
*/

static std::map<std::string, Scr_Function> scriptFunctions;
static std::map<std::string, Scr_Function> scriptMethods;

int Scr_GetNumParam();
int Scr_GetPointerType(int);
int Scr_GetType(int);
int Scr_AddInt(int);
int Scr_AddBool(BOOL);
int Scr_AddString(char*);
int Scr_AddVector(vec3_t);
BOOL Scr_GetBool(int);
int Scr_GetInt(int);
float Scr_GetFloat(int);
char* Scr_GetString(int);
int Scr_GetVector(int, vec3_t);
int Scr_GetFunc(int);
int Scr_AddUndefined();

Scr_FunctionCall Scr_GetFunction(const char**, int*);
Scr_FunctionCall Scr_GetMethod(const char**, int*);

int GScr_LoadGameTypeScript();
void patchScriptCall();
void InitScriptFunctions();
void patchDeveloperScripts();
void writePlayerMethods();
void writeScripts();

/*
    Could make another function called Scr_AddMethod, but hey why not just use a boolean flag instead? :D
*/
void Scr_AddFunction(const char*, Scr_FunctionCall, int, BOOL);

void GScr_console_print(int);
void GScr_default(int);

#endif // SCRIPT_H
