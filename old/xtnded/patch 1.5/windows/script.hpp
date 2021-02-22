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

#include "cod.hpp"

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

/*
    There are the default Call of Duty 1.5 functions and methods.

    :: Functions
        print, 20031c50, 1
        println, 20031c90, 1
        iprintln, 20032170, 0
        iprintlnbold, 20032180, 0
        print3d, 20032190, 0
        line, 20032250, 0
        getent, 2003a620, 0
        getentarray, 2003a6d0, 0
        spawn, 20032840, 0
        spawnturret, 20032900, 0
        precacheturret, 20032980, 0
        spawnstruct, 20042920, 0
        assert, 20032310, 1
        isdefined, 20032320, 0
        isalive, 20032360, 0
        getcvar, 200323c0, 0
        getcvarint, 20032420, 0
        getcvarfloat, 20032440, 0
        setcvar, 20032470, 0
        gettime, 20032580, 0
        getentbynum, 20032590, 0
        getweaponmodel, 200325d0, 0
        getweaponclassname, 20032650, 0
        getanimlength, 20032700, 0
        animhasnotetrack, 20032760, 0
        getbrushmodelcenter, 200327a0, 0
        objective_add, 20034100, 0
        objective_delete, 20034250, 0
        objective_state, 200342d0, 0
        objective_icon, 200343a0, 0
        objective_position, 200343f0, 0
        objective_onentity, 200344b0, 0
        objective_current, 20034570, 0
        bullettrace, 20034ea0, 0
        getmovedelta, 20035f60, 0
        getangledelta, 20036050, 0
        randomint, 20035080, 0
        randomfloat, 200350e0, 0
        randomintrange, 20035140, 0
        randomfloatrange, 200351b0, 0
        sin, 20035260, 0
        cos, 20035290, 0
        tan, 200352c0, 0
        asin, 20035340, 0
        acos, 200353b0, 0
        atan, 20035420, 0
        distance, 20035450, 0
        distancesquared, 200354d0, 0
        length, 20035540, 0
        lengthsquared, 20035580, 0
        closer, 200355c0, 0
        vectordot, 200356b0, 0
        vectornormalize, 20035700, 0
        vectortoangles, 20035750, 0
        anglestoup, 20035790, 0
        anglestoright, 200357d0, 0
        anglestoforward, 200358e0, 0
        musicplay, 20035990, 0
        musicstop, 200359c0, 0
        soundfade, 20035a60, 0
        ambientplay, 20035c20, 0
        ambientstop, 20035cf0, 0
        precachemodel, 20035ac0, 0
        precacheshellshock, 20035af0, 0
        precacheitem, 20035b20, 0
        precacheshader, 20035b90, 0
        precachestring, 20035be0, 0
        loadfx, 20036140, 0
        playfx, 20036180, 0
        playfxontag, 200362c0, 0
        playloopedfx, 20036480, 0
        setcullfog, 200367d0, 0
        setexpfog, 20036870, 0
        grenadeexplosioneffect, 20035da0, 0
        radiusdamage, 20035ec0, 0
        setplayerignoreradiusdamage, 20035f40, 0
        getnumparts, 20036f80, 0
        getpartname, 20036fb0, 0
        earthquake, 20037030, 0
        newhudelem, 200240c0, 0
        newclienthudelem, 20024110, 0
        newteamhudelem, 200241c0, 0
        resettimeout, 20042d40, 0
        isplayer, 20036930, 0
        isplayernumber, 20036980, 0
        setwinningplayer, 200369b0, 0
        setwinningteam, 20036aa0, 0
        announcement, 20036ba0, 0
        clientannouncement, 20036c10, 0
        getteamscore, 20036cc0, 0
        setteamscore, 20036d30, 0
        setclientnamemode, 20036dc0, 0
        updateclientnames, 20036e00, 0
        getteamplayersalive, 20036ee0, 0
        objective_team, 20034600, 0
        logprint, 200346a0, 0
        worldentnumber, 20034750, 0
        obituary, 20034760, 0
        positionwouldtelefrag, 20034870, 0
        getstarttime, 20034960, 0
        precachemenu, 20034970, 0
        precachestatusicon, 20034b20, 0
        precacheheadicon, 20034ce0, 0
        map_restart, 200377e0, 0
        exitlevel, 20037850, 0
        addtestclient, 200378b0, 0
        makecvarserverinfo, 200378e0, 0
        setarchive, 200379c0, 0
        prof_begin, 200379e0, 0
        prof_end, 200379f0, 0

    :: Methods

    >> Player
        giveweapon
        takeweapon
        takeallweapons
        getcurrentweapon
        hasweapon
        switchtoweapon
        givestartammo
        givemaxammo
        getfractionstartammo
        getfractionmaxammo
        setorigin
        setplayerangles
        usebuttonpressed
        attackbuttonpressed
        meleebuttonpressed
        isonground
        pingplayer
        setviewmodel
        getviewmodel
        sayall
        sayteam
        saysquad
        allowcomplaint
        showscoreboard
        setspawnweapon
        dropitem
        finishplayerdamage
        suicide
        openmenu
        openmenunomouse
        closemenu
        freezecontrols
        disableweapon
        enableweapon
        setreverb
        getweaponslotweapon
        setweaponslotweapon
        getweaponslotammo
        setweaponslotammo
        getweaponslotclipammo
        setweaponslotclipammo
        iprintln
        iprintlnbold
        spawn
        setentertime
        cloneplayer
        setclientcvar
        islookingat
        playlocalsound
        allowspectateteam
        getguid
        autodemostart
        autodemostop
        autoscreenshot
        name

    >> Entity
        attach
        detach
        detachall
        getattachsize
        getattachmodelname
        getattachtagname
        getattachignorecollision
        linkto
        unlink
        enablelinkto
        getorigin
        getstance
        geteye
        useby
        istouching
        lockdoor
        unlockdoor
        isdoorlocked
        playsound
        playloopsound
        stoploopsound
        delete
        setmodel
        getnormalhealth
        setnormalhealth
        show
        hide
        setcontents
        setcursorhint
        sethintstring
        shellshock
        stopshellshock
        viewkick
        localtoworldcoords
        setrightarc
        setleftarc
        settoparc
        setbottomarc
        getentitynumber
        enablegrenadetouchdamage
        disablegrenadetouchdamage
        enablegrenadebounce
        disablegrenadebounce
        placespawnpoint

    >> HudElem
        settext
        setshader
        settimer
        settimerup
        settenthstimer
        settenthstimerup
        setclock
        setclockup
        setvalue
        fadeovertime
        scaleovertime
        moveovertime
        reset
        destroy

    >> ScriptEnt
        moveto
        movex
        movey
        movez
        movegravity
        rotateto
        rotatepitch
        rotateyaw
        rotateroll
        rotatevelocity
        solid
        notsolid
*/


extern int callbackTest;
extern int callbackPlayerCommand;
extern int callbackEntityDamage;
extern int callbackEntityKilled;
extern int callbackEntityUse;
extern int callbackEntityTouch;
extern int callbackEntityThink;

typedef int (*loadCallback_t)(char*,char*); //loadcallbackscript
typedef int (*dword_20031760_t)();
typedef int (*Scr_ExecThread_t)(int, int);
typedef int (*Scr_ExecEntThread_t)(int, int, int, int);
typedef int (*Scr_FreeThread_t)(int);

typedef char* (*SL_ConvertToString_t)(unsigned int);

extern loadCallback_t loadCallback;
extern dword_20031760_t dword_20031760;
extern Scr_ExecThread_t Scr_ExecThread;
extern Scr_ExecEntThread_t Scr_ExecEntThread;
extern Scr_FreeThread_t Scr_FreeThread;
extern SL_ConvertToString_t SL_ConvertToString;

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

typedef int (*Scr_GetNumParam_t)();
typedef int (*Scr_GetPointerType_t)(int);
typedef int (*Scr_GetType_t)(int);

typedef void (*Scr_MakeArray_t)();
typedef void (*Scr_AddArray_t)();
typedef void (*Scr_Error_t)(const char*);

typedef int (*Scr_AddInt_t)(int);
typedef int (*Scr_AddFloat_t)(float);
typedef int (*Scr_AddBool_t)(BOOL);
typedef int (*Scr_AddString_t)(char*);
typedef int (*Scr_AddVector_t)(vec3_t);
typedef int (*Scr_AddUndefined_t)();
typedef void (*Scr_AddEntity_t)(int);

typedef BOOL (*Scr_GetBool_t)(int);
typedef int (*Scr_GetInt_t)(int);
typedef float (*Scr_GetFloat_t)(int);
typedef int (*Scr_GetVector_t)(int, vec3_t);
typedef char* (*Scr_GetString_t)(int);
typedef int (*Scr_GetFunc_t)(int);
typedef Scr_FunctionCall (*Scr_GetFunction_t)(const char**, int*);
typedef Scr_FunctionCall (*Scr_GetMethod_t)(const char**, int*);

extern Scr_GetNumParam_t Scr_GetNumParam;
extern Scr_GetPointerType_t Scr_GetPointerType;
extern Scr_GetType_t Scr_GetType;

extern Scr_MakeArray_t Scr_MakeArray;
extern Scr_AddArray_t Scr_AddArray;
extern Scr_Error_t Scr_Error;

extern Scr_AddInt_t Scr_AddInt;
extern Scr_AddFloat_t Scr_AddFloat;
extern Scr_AddBool_t Scr_AddBool;
extern Scr_AddString_t Scr_AddString;
extern Scr_AddVector_t Scr_AddVector;
extern Scr_AddUndefined_t Scr_AddUndefined;
extern Scr_AddEntity_t Scr_AddEntity;

extern Scr_GetBool_t Scr_GetBool;
extern Scr_GetInt_t Scr_GetInt;
extern Scr_GetFloat_t Scr_GetFloat;
extern Scr_GetVector_t Scr_GetVector;
extern Scr_GetString_t Scr_GetString;
extern Scr_GetFunc_t Scr_GetFunc;
extern Scr_GetFunction_t Scr_GetFunction;
extern Scr_GetMethod_t Scr_GetMethod;

int GScr_LoadGameTypeScript();
void Scr_LoadGameType();
void InitScript();
void dumpScriptMethods(const char*);
void dumpScriptFunctions(const char*);

/*
    Could make another function called Scr_AddMethod, but hey why not just use a boolean flag instead? :D
*/
void Scr_AddFunction(const char*, Scr_FunctionCall, int, BOOL);

void GScr_console_print(int);
void GScr_default(int);


/*
    Player Methods
*/
void GScr_setVelocity(int);
void GScr_getVelocity(int);
void GScr_getPlayerAngles(int);
void GScr_getIP(int);
void GScr_backButtonPressed(int);
void GScr_forwardButtonPressed(int);
void GScr_leftButtonPressed(int);
void GScr_rightButtonPressed(int);
void GScr_moveupButtonPressed(int);
void GScr_movedownButtonPressed(int);
void GScr_aimButtonPressed(int);
void GScr_reloadButtonPressed(int);

/*
    Entity Methods
*/
void ScrCmd_setBounds(int);
void ScrCmd_setTakeDamage(int);
void GScr_callback(int);
void GScr_nextthink(int);

/*
    Math
*/

void GScr_VectorScale(int);

/*
    MySQL
*/

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

/*
    FILE functions
*/

void GScr_fopen(int);
void GScr_fclose(int);
void GScr_fread(int);
void GScr_fexists(int);
void GScr_fsize(int);
void GScr_fwrite(int);

#endif // SCRIPT_H
