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
#ifndef SCRIPT_H
#define SCRIPT_H

#include "shared.h"
#include "server.h"
#include "bg_public.h"
#include "js.h"
#include <math.h>

typedef struct {
	int map_main;
	int idk;
	int gametype_main;
	int startgametype;
	int playerconnect;
	int playerdisconnect;
	int playerdamage;
	int playerkilled;
} game_script_data;

extern game_script_data *g_scr_data;

typedef struct {
	short active;
	short air_strike;
	short allies;
	short animdone;
	short axis;
	short bodyque;
	short combat;
	short connected;
	short connecting;
	short count;
	short crouch;
	short crowbar;
	short current;
	short damage;
	short death;
	short disconnected;
	short dlight;
	short done;
	short empty;
	short enemy;
	short enemyhidden;
	short enemyvisible;
	short entity;
	short failed;
	short flamebarrel;
	short fraction;
	short func_door;
	short func_door_rotating;
	short func_rotating;
	short func_tramcar;
	short goal;
	short grenade;
	short info_notnull;
	short invisible;
	short key1;
	short key2;
	short killanimscript;
	short left;
	short misc_flak;
	short misc_mg42;
	short misc_tagemitter;
	short mortar;
	short movedone;
	short noclass;
	short noenemy;
	short noncombat;
	short normal;
	short pistol;
	short plane_waypoint;
	short player;
	short position;
	short primary;
	short primaryb;
	short prone;
	short right;
	short rocket;
	short rotatedone;
	short script_brushmodel;
	short script_model;
	short script_origin;
	short scriptcamera;
	short spawned;
	short spectator;
	short stand;
	short surfacetype;
	short tag_engine1;
	short tag_engine2;
	short target_location;
	short target_script_trigger;
	short tempEntity;
	short muzzleEntity;
	short smokegrenade;
	short touch;
	short trigger;
	short trigger_use;
	short trigger_damage;
	short trigger_lookat;
	short truck_cam;
	short xmodel_airborne;
	short xmodel_wehrmacht;
	short worldspawn;
	short begin;
	short dynamite;
	short explosive_indicator;
	short flamechunk;
	short follow;
	short free;
	short freed;
	short func_leaky;
	short info_player_checkpoint;
	short initialize;
	short intermission;
	short item_stamina_brandy;
	short menuresponse;
	short misc_gunner_gun;
	short misc_gunner_ring;
	short mp_info_player_deathmatch;
	short mp_info_player_intermission;
	short mp_team_alliedplayer_respawn;
	short mp_team_alliedplayer_start;
	short mp_team_axisplayer_respawn;
	short mp_team_axisplayer_start;
	short nail;
	short _not;
	short playing;
	short prox_mine;
	short reset;
	short script_mover;
	short script_multiplayer;
	short spear;
	short tag_hand;
	short tag_rider;
	short tag_ring;
	short team_CTF_blueflag;
	short team_CTF_redflag;
	short team_WOLF_checkpoint;
	short team_WOLF_objective;
	short trigger_aidoor;
	short trigger_flagonly;
	short trigger_multiple;
	short trigger_objective_info;
	short waiting_for_players;
	short WP;
	short zombiespit;
	short none;
	short dead;
	short auto_change;
	short manual_change;
} scr_const_t;

typedef struct {
	short texturename;
	short contents;
	
	//assumingly from perk > MAX_PERKS
	short perks[MAX_PERKS];
} xscr_const_t;

extern xscr_const_t xscr_const;
extern scr_const_t *scr_const;

/*
	A pointer type their variable type is always 7 [object]
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

typedef SCRIPTFUNCTIONCALL (*Scr_GetFunction_t)(const char**, int*);
typedef SCRIPTFUNCTIONCALL (*Scr_GetMethod_t)(const char**, int*);

extern Scr_GetFunction_t Scr_GetFunction;
extern Scr_GetMethod_t Scr_GetMethod;

SCRIPTFUNCTIONCALL Scr_GetCustomFunction(const char**, int*);
SCRIPTFUNCTIONCALL Scr_GetCustomMethod(const char**, int*);

typedef int (*Scr_GetNumParam_t)();
typedef int (*Scr_GetPointerType_t)(int);
typedef int (*Scr_GetType_t)(int);

typedef void (*Scr_MakeArray_t)();
typedef void (*Scr_AddArray_t)();
typedef void (*Scr_AddArrayStringIndexed_t)(short);
typedef void (*Scr_Error_t)(const char*);

typedef int (*Scr_AddFloat_t)(float);
typedef int (*Scr_AddInt_t)(int);
typedef int (*Scr_AddBool_t)(bool);
typedef int (*Scr_AddString_t)(char*);
typedef int (*Scr_AddIString_t)(char*);
typedef int (*Scr_AddVector_t)(vec3_t);
typedef int (*Scr_AddUndefined_t)();
typedef void (*Scr_AddEntity_t)(int*);

typedef bool (*Scr_GetBool_t)(int);
typedef int (*Scr_GetInt_t)(int);
typedef void (*Scr_GetAnim_t)(int,int,int);
typedef int (*Scr_GetAnimsIndex_t)(int);
typedef float (*Scr_GetFloat_t)(int);
typedef int (*Scr_GetVector_t)(int, vec3_t);
typedef char* (*Scr_GetString_t)(int);
typedef int (*Scr_GetFunc_t)(int);
typedef short (*Scr_GetOffset_t)(unsigned, const char*);

typedef int (*Scr_GetConstString_t)(int);
typedef void (*Scr_SetString_t)(int*, int);
typedef short (*Scr_AllocString_t)(const char*, int);

typedef int (*oBG_AnimationIndexForString_t)(const char*);
extern oBG_AnimationIndexForString_t oBG_AnimationIndexForString;

typedef qboolean (*CallSpawnEntity_t)(int);
extern CallSpawnEntity_t CallSpawnEntity;

extern Scr_AllocString_t Scr_AllocString;
extern Scr_SetString_t Scr_SetString;
extern Scr_GetConstString_t Scr_GetConstString;

extern Scr_GetNumParam_t Scr_GetNumParam;
extern Scr_GetPointerType_t Scr_GetPointerType;
extern Scr_GetType_t Scr_GetType;

extern Scr_MakeArray_t Scr_MakeArray;
extern Scr_AddArray_t Scr_AddArray;
extern Scr_AddArrayStringIndexed_t Scr_AddArrayStringIndexed;
extern Scr_Error_t Scr_Error;

extern Scr_AddInt_t Scr_AddInt;
extern Scr_AddFloat_t Scr_AddFloat;
extern Scr_AddBool_t Scr_AddBool;
extern Scr_AddString_t Scr_AddString;
extern Scr_AddIString_t Scr_AddIString;
extern Scr_AddVector_t Scr_AddVector;
extern Scr_AddUndefined_t Scr_AddUndefined;
extern Scr_AddEntity_t Scr_AddEntity;

extern Scr_GetBool_t Scr_GetBool;
extern Scr_GetInt_t Scr_GetInt;
extern Scr_GetAnim_t Scr_GetAnim;
extern Scr_GetAnimsIndex_t Scr_GetAnimsIndex;
extern Scr_GetFloat_t Scr_GetFloat;
extern Scr_GetVector_t Scr_GetVector;
extern Scr_GetString_t Scr_GetString;
extern Scr_GetFunc_t Scr_GetFunc;
extern Scr_GetOffset_t Scr_GetOffset;

typedef int (*Scr_LoadScr_t)(const char*);
typedef int (*Scr_GetFunctionHandle_t)(const char*, const char*);
typedef int (*Scr_ExecThread_t)(int, int);
typedef int (*Scr_ExecEntThread_t)(int, int, int, int);
typedef int (*Scr_FreeThread_t)(int);

typedef char* (*SL_ConvertToString_t)(unsigned int);
typedef unsigned short (*SL_GetString_t)(const char*, int);

extern int callbackTest;
extern int callbackPlayerCommand;

extern Scr_LoadScr_t Scr_LoadScript;
extern Scr_GetFunctionHandle_t Scr_GetFunctionHandle;
extern Scr_ExecThread_t Scr_ExecThread;
extern Scr_ExecEntThread_t Scr_ExecEntThread;
extern Scr_FreeThread_t Scr_FreeThread;
extern SL_ConvertToString_t SL_ConvertToString;
extern SL_GetString_t SL_GetString;

void scriptInitializing();
char* Scr_GetVariableType(int type);

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
STRING
=============
*/
void Scr_StrTok(int);
void Scr_ToLower(int);
void Scr_ToUpper(int);
void Scr_ucfirst(int);
void Scr_IsSubStr(int);
void Scr_passArray(int);
void Scr_trim(int);
void Scr_convertToIString(int a1);

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
#ifdef uMYSQL
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
#endif
/*
=============
PLAYER METHODS
=============
*/

void PlayerCmd_useButtonPressedX(int);
void PlayerCmd_SetVelocity(int);
void PlayerCmd_GetVelocity(int);
void PlayerCmd_getPlayerAngles(int);
void PlayerCmd_getip(int);
void PlayerCmd_GetInt(int);
void PlayerCmd_SetInt(int);
void PlayerCmd_GetByte(int);
void PlayerCmd_SetByte(int);
void PlayerCmd_GetFloat(int);
void PlayerCmd_SetFloat(int);
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
void PlayerCmd_GetUserInfo(int);
void PlayerCmd_DropClient(int);
void PlayerCmd_kickbot(int);
void PlayerCmd_renamebot(int);
void PlayerCmd_isbot(int);
void PlayerCmd_SendServerCommand(int);
void PlayerCmd_SendGamestate(int);
void PlayerCmd_ispure(int a1);
void PlayerCmd_GetGuid(int a1);
void PlayerCmd_SetPerk(int);
void PlayerCmd_UnsetPerk(int);
void PlayerCmd_HasPerk(int);

/*
=============
ENTITY METHODS
=============
*/

void ScriptEnt_SetBounds(int);
void ScriptEnt_SetBoundCorners(int);
void ScriptEnt_SetTakeDamage(int);
void ScriptEnt_SetAbsMax(int);
void ScriptEnt_SetMaxs(int);
void ScriptEnt_SetMins(int);
void ScriptEnt_SetAbsMin(int);
void ScriptEnt_SetLight(int);

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