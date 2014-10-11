#ifndef SCRIPT_H
#define SCRIPT_H

#include "../codextended.hpp"
#include "shared.hpp"
#include "../entity.hpp"
#include "../server/server.hpp"
#include "js.hpp"
#include <math.h>

/*
.rodata:080E1AE0 aUndefined      db 'undefined',0        ; DATA XREF: .data:off_80EE920o
.rodata:080E1AEA ; char aString_1[]
.rodata:080E1AEA aString_1       db 'string',0           ; DATA XREF: sub_80A92AA:loc_80A939Eo
.rodata:080E1AEA                                         ; .data:080EE924o
.rodata:080E1AF1 aLocalizedStrin db 'localized string',0 ; DATA XREF: .data:080EE928o
.rodata:080E1B02 aVector         db 'vector',0           ; DATA XREF: .data:080EE92Co
.rodata:080E1B09 ; char aFloat_1[]
.rodata:080E1B09 aFloat_1        db 'float',0            ; DATA XREF: sub_80A92AA:loc_80A935Eo
.rodata:080E1B09                                         ; .data:080EE930o
.rodata:080E1B0F ; char aInt[]
.rodata:080E1B0F aInt            db 'int',0              ; DATA XREF: sub_80A92AA:loc_80A937Eo
.rodata:080E1B0F                                         ; .data:080EE934o
.rodata:080E1B13 aCodepos        db 'codepos',0          ; DATA XREF: .data:080EE938o
.rodata:080E1B1B aObject         db 'object',0           ; DATA XREF: .data:080EE93Co
.rodata:080E1B22 aKeyValue       db 'key/value',0        ; DATA XREF: .data:080EE940o
.rodata:080E1B2C aFunction       db 'function',0         ; DATA XREF: .data:080EE944o
.rodata:080E1B35 aStack          db 'stack',0            ; DATA XREF: .data:080EE948o
.rodata:080E1B3B aAnimation      db 'animation',0        ; DATA XREF: .data:080EE94Co
.rodata:080E1B45 aThread         db 'thread',0           ; DATA XREF: .data:080EE950o
.rodata:080E1B4C aEntity         db 'entity',0           ; DATA XREF: .data:080EE954o
.rodata:080E1B53 aStruct         db 'struct',0           ; DATA XREF: .data:080EE958o
.rodata:080E1B5A aArray          db 'array',0            ; DATA XREF: .data:080EE95Co
.rodata:080E1B60 aDeadThread     db 'dead thread',0      ; DATA XREF: .data:080EE960o
.rodata:080E1B6C aDeadEntity     db 'dead entity',0      ; DATA XREF: .data:080EE964o
.rodata:080E1B78 aDeadObject     db 'dead object',0      ; DATA XREF: .data:080EE968o
*/

typedef enum {
	VT_UNDEFINED, //0
	VT_STRING, //1
	VT_LOCALIZED_STRING, //2
	VT_VECTOR, //3
	VT_FLOAT, //4
	VT_INT, //5
	VT_CODEPOS, //6
	VT_OBJECT, //7
	VT_KEY_VALUE, //8
	VT_FUNCTION, //9
	VT_STACK, //10
	VT_ANIMATION, //11
	VT_THREAD, //12
	VT_ENTITY, //13
	VT_STRUCT, //14
	VT_ARRAY, //16
	VT_DEAD_THREAD, //15
	VT_DEAD_ENTITY, //17
	VT_DEAD_OBJECT //18
} script_variable_type;

typedef void (__cdecl *SCRIPTFUNCTIONCALL)(int);

typedef struct {
	const char* name;
	SCRIPTFUNCTIONCALL call;
	int developer;
} SCRIPTFUNCTION;

typedef SCRIPTFUNCTIONCALL (*Script_GetFunction_t)(const char**, int*);
typedef SCRIPTFUNCTIONCALL (*Script_GetMethod_t)(const char**, int*);

extern Script_GetFunction_t Script_GetFunction;
extern Script_GetMethod_t Script_GetMethod;

SCRIPTFUNCTIONCALL Script_GetCustomFunction(const char**, int*);
SCRIPTFUNCTIONCALL Script_GetCustomMethod(const char**, int*);

typedef int (*Script_GetNumParam_t)();
typedef int (*Script_GetPointerType_t)(int);
typedef int (*Script_GetType_t)(int);

typedef void (*Script_MakeArray_t)();
typedef void (*Script_AddArray_t)();
typedef void (*Script_Error_t)(const char*);

typedef int (*Script_AddFloat_t)(float);
typedef int (*Script_AddInt_t)(int);
typedef int (*Script_AddBool_t)(bool);
typedef int (*Script_AddString_t)(char*);
typedef int (*Script_AddVector_t)(vec3_t);
typedef int (*Script_AddUndefined_t)();
typedef void (*Script_AddEntity_t)(int*);

typedef bool (*Script_GetBool_t)(int);
typedef int (*Script_GetInt_t)(int);
typedef void (*Script_GetAnim_t)(int,int,int);
typedef int (*Script_GetAnimsIndex_t)(int);
typedef float (*Script_GetFloat_t)(int);
typedef int (*Script_GetVector_t)(int, vec3_t);
typedef char* (*Script_GetString_t)(int);
typedef int (*Script_GetFunc_t)(int);

typedef int (*Script_GetConstString_t)(int);
typedef void (*Script_SetString_t)(int*, int);
typedef gentity_t* (*oSpawn_t)(void);
typedef void (*oInitGentity_t)(gentity_t*);
typedef void (*oFreeEntity_t)(gentity_t*);

typedef int (*oBG_AnimationIndexForString_t)(const char*);
extern oBG_AnimationIndexForString_t oBG_AnimationIndexForString;

extern Script_SetString_t Script_SetString;
extern Script_GetConstString_t Script_GetConstString;
extern oSpawn_t oSpawn;
extern oInitGentity_t oInitGentity;
extern oFreeEntity_t oFreeEntity;

extern Script_GetNumParam_t Script_GetNumParam;
extern Script_GetPointerType_t Script_GetPointerType;
extern Script_GetType_t Script_GetType;

extern Script_MakeArray_t Script_MakeArray;
extern Script_AddArray_t Script_AddArray;
extern Script_Error_t Script_Error;

extern Script_AddInt_t Script_AddInt;
extern Script_AddFloat_t Script_AddFloat;
extern Script_AddBool_t Script_AddBool;
extern Script_AddString_t Script_AddString;
extern Script_AddVector_t Script_AddVector;
extern Script_AddUndefined_t Script_AddUndefined;
extern Script_AddEntity_t Script_AddEntity;

extern Script_GetBool_t Script_GetBool;
extern Script_GetInt_t Script_GetInt;
extern Script_GetAnim_t Script_GetAnim;
extern Script_GetAnimsIndex_t Script_GetAnimsIndex;
extern Script_GetFloat_t Script_GetFloat;
extern Script_GetVector_t Script_GetVector;
extern Script_GetString_t Script_GetString;
extern Script_GetFunc_t Script_GetFunc;

typedef int (*Script_LoadScript_t)(const char*);
typedef int (*Script_GetFunctionHandle_t)(const char*, const char*);
typedef int (*Script_ExecThread_t)(int, int);
typedef int (*Script_ExecEntThread_t)(int, int, int, int);
typedef int (*Script_FreeThread_t)(int);

typedef char* (*ScriptL_ConvertToString_t)(unsigned int);

extern int callbackTest;
extern int callbackPlayerCommand;
extern int callbackEntityDamage;
extern int callbackEntityKilled;
extern int callbackEntityUse;
extern int callbackEntityTouch;
extern int callbackEntityThink;

extern Script_LoadScript_t Script_LoadScript;
extern Script_GetFunctionHandle_t Script_GetFunctionHandle;
extern Script_ExecThread_t Script_ExecThread;
extern Script_ExecEntThread_t Script_ExecEntThread;
extern Script_FreeThread_t Script_FreeThread;
extern ScriptL_ConvertToString_t ScriptL_ConvertToString;


void scriptInitializing();

/*
=============
FUNCTIONS
=============
*/

void GScr_printconsole(int);
void GScr_Trace(int);
void GScr_trap_Argv(int);
void GScr_ConcatArgs(int);
void GScr_getChat(int);
void GScr_Cmd_Argv(int);
void GScr_Cmd_Argc(int);
void GScr_return(int);

void GScr_salt_password(int a1);
void GScr_strpos(int);

void GScr_SpawnAnimEnt(int);
void GScr_callspawn(int);
void GScr_GetAnimIndex(int);

/*
=============
MATH
=============
*/

void MScr_cos(int);
void MScr_sin(int);
void MScr_sqrt(int);

/*
=====
BITWISE OPERATORS
=====
*/
void GScr_and(int);
void GScr_or(int);
void GScr_xor(int);
void GScr_rshift(int);
void GScr_lshift(int);
void GScr_not(int);

/*
======
CRYPTO
======
*/
void GScr_md5(int);

/*
=============
MYSQL
=============
*/

void GScr_mysql_init(int);
void GScr_mysql_close(int);
void GScr_mysql_affected_rows(int);
void GScr_mysql_errno(int);
void GScr_mysql_error(int);
void GScr_mysql_fetch_field(int);
void GScr_mysql_fetch_row(int);
void GScr_mysql_field_seek(int);
void GScr_mysql_free_result(int);
void GScr_mysql_num_fields(int);
void GScr_mysql_num_rows(int);
void GScr_mysql_query(int);
void GScr_mysql_real_connect(int);
void GScr_mysql_real_escape_string(int);
void GScr_mysql_store_result(int);
void GScr_mysql_get_connection(int);

/*
=============
PLAYER METHODS
=============
*/

void PlayerCmd_useButtonPressedX(int);
void PlayerCmd_setVelocity(int);
void PlayerCmd_getVelocity(int);
void PlayerCmd_getPlayerAngles(int);
void PlayerCmd_getip(int);
void PlayerCmd_getInt(int);
void PlayerCmd_setInt(int);
void PlayerCmd_getbyte(int);
void PlayerCmd_setbyte(int);
void PlayerCmd_getfloat(int);
void PlayerCmd_setfloat(int);
void PlayerCmd_backButtonPressed(int);
void PlayerCmd_forwardButtonPressed(int);
void PlayerCmd_leftButtonPressed(int);
void PlayerCmd_rightButtonPressed(int);
void PlayerCmd_moveupButtonPressed(int);
void PlayerCmd_movedownButtonPressed(int);
void PlayerCmd_aimButtonPressed(int);
void PlayerCmd_reloadButtonPressed(int);
void PlayerCmd_leanLeftButtonPressed(int);
void PlayerCmd_leanRightButtonPressed(int);
void PlayerCmd_GetUserInfoKey(int);
void PlayerCmd_DropClient(int);
void PlayerCmd_kickbot(int);
void PlayerCmd_renamebot(int);
void PlayerCmd_isbot(int);
void PlayerCmd_SendServerCommand(int);
void PlayerCmd_ispure(int a1);

/*
=============
ENTITY METHODS
=============
*/

void EntCmd_setBounds(int);
void EntCmd_setBoundCorners(int);
void EntCmd_setTakeDamage(int);
void EntCmd_callback(int);
void EntCmd_nextthink(int);

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen(int);
void GScr_fclose(int);
void GScr_fread(int);
void GScr_fexists(int);
void GScr_fsize(int);
void GScr_fwrite(int);

/*
=============
MYSQL FUNCTIONS
=============
*/

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

#endif // SCRIPT_H
