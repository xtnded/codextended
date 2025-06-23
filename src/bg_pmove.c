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

#include "server.h"
#include "bg_public.h"

#define LOBYTE(x)   (*((unsigned char*)&(x)))   // low byte
#define LOWORD(x)   (*((short*)&(x)))   // low word
#define LODWORD(x)  (*((int*)&(x)))  // low dword
#define HIBYTE(x)   (*((unsigned char*)&(x)+1))
#define HIWORD(x)   (*((short*)&(x)+1))
#define HIDWORD(x)  (*((int*)&(x)+1))
#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define WORDn(x, n)   (*((short*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

#define OVERCLIP 1.001

pmove_t *pm;

static int (*BG_GetNumWeapons)();
static int (*BG_GetInfoForWeapon)(int);
static int (*BG_AnimScriptEvent)(playerState_t*,int,int,int);
static void (*PM_AddEvent)(int);

static void (*PM_SetWeaponReloadAddAmmoDelay)();

#define QUICK_RELOAD_FRACTION 3

static void PM_Weapon( void ) {
	pmove_t *xm = *(pmove_t**)(int)pm;
	
	int *weaponstate = (int*)((int)xm->ps + 180);
	int *weapons = *(int**)((int)xm->ps + 796);
	int *weapon = (int*)((int)xm->ps + 176);
	int *weaponTime = (int*)((int)xm->ps + 44);
	int *weaponDelay = (int*)((int)xm->ps + 48);
	int prestate = *weaponstate;
	
	playerState_t *ps = xm->ps;
	
	int clientNum = *(int*)((int)ps + 172);
	client_t *cl = getclient( clientNum );
	gentity_t *ent = &g_entities[clientNum];
	
	gclient_t *gclient = ent->client;
	
	void (*o)() = (void(*)())GAME("PM_Weapon");
	o();
	
	#if 1
	if((cl->lastUsercmd.buttons & 0x40) == 0x40) { //usebuttonpressed
		if(!xclients[clientNum].sprinting) {
			
			*(int*)((int)gclient + 180) = WEAPON_RECHAMBERING;
			*(int*)((int)gclient + 980) = WEAP_ALTSWITCHFROM;
			
			*(float*)((int)gclient + 848) = 1.35; //runspeedscale?
			//*(int*)((int)gclient + 828) = //prone
			//*(int*)((int)gclient + 832) = //crouch;
			//*(int*)((int)gclient + 836) = 100;//standing;
			xclients[clientNum].sprinting = 1;
			return;
		}
	} else {
		if(xclients[clientNum].sprinting) {
			*(int*)((int)gclient + 180) = WEAPON_READY;
			*(int*)((int)gclient + 980) = WEAP_IDLE;
			*(float*)((int)gclient + 848) = 1; //runspeedscale?
			xclients[clientNum].sprinting = 0;
			
			return;
		}
	}
	#endif
	
	//prob shouldnt include interrupts
	if(xclients[clientNum].perks[PERK_QUICK_RELOAD] && (prestate == WEAPON_RELOADING || prestate == WEAPON_RELOAD_END || prestate == WEAPON_RELOAD_START || prestate == WEAPON_RECHAMBERING) && (!*weaponDelay || !*weaponTime)) {
		*(int*)((int)gclient + 980) = 17;
	}
	
	/*
	client_t *cl = getclient( clientNum );
	
	xentity_t *xent = &xentities[clientNum];
	gentity_t *ent = g_entities(clientNum);
	gclient_t *gclient = ent->client;
	
	//if(*(int*)((int)ps + 8680) & 0x20 && cl->lastUsercmd.wbuttons & 0x8) {
	if(xent->sprinting) {
		*(int*)((int)gclient + 180) = WEAPON_RECHAMBERING;
		*(int*)((int)gclient + 980) = WEAP_ALTSWITCHFROM;
		
		//*(int*)((int)gclient + 828) = //prone
		//*(int*)((int)gclient + 832) = //crouch;
		//*(int*)((int)gclient + 836) = 100;//standing;
		return;
	}
	
	//*(int*)((int)gclient + 836) = 60;//standing;
	*/
	
}

static int PM_CheckJump() {
	pmove_t *xm = *(pmove_t**)(int)pm;
	#if 0
	if(xm->cmd.wbuttons & WBUTTON_RELOAD) {
		
		return 0;
		
	}
	#endif
	
	int (*cj)() = (int(*)())GAME("BG_PlayerTouchesItem") + 0x7DC;
	return cj();
}

/*
==================
PM_ClipVelocity

Slide off of the impacting surface
==================
*/
void PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce ) {
	float	backoff;
	float	change;
	int		i;
	
	backoff = DotProduct (in, normal);
	
	if ( backoff < 0 ) {
		backoff *= overbounce;
	} else {
		backoff /= overbounce;
	}

	for ( i=0 ; i<3 ; i++ ) {
		change = normal[i]*backoff;
		out[i] = in[i] - change;
	}
}

#ifdef xDEBUG
void __dump_events() {
	char **events = (char**)GAME("eventnames");
	FILE *fp = fopen("/home/rawcod/event.dump", "w");
	if(!fp)
		return 0;
	int i ;
	for( i = 0; i < EV_MAX_EVENTS; i ++) {
		fprintf(fp, "\"%s\", //%d\n", events[i], i);
		fflush(fp);
	}
	fclose(fp);
}
#endif

cvar_t *x_cl_adsair;

void _PM_ClearAimDownSightFlag() {
	/*
	qpmove_t *pp = (qpmove_t*)pm;
	qps *ps = pp->ps;
	
	ps->pm_flags &= 0xDFu;
	*/
	if(!x_cl_adsair->integer)
		((void(*)())GAME("PM_ClearAimDownSightFlag"))();
}

void _PM_UpdateAimDownSightFlag() {
	#if 0
		void (*_BG_UpdateConditionValue)(int,int,int,qboolean);
		*(int*)&_BG_UpdateConditionValue = GAME("BG_UpdateConditionValue");
		
		qpmove_t *pp = (qpmove_t*)pm;
		qps *ps = pp->ps;
		
		int v3 = *(int*)(&ps->cmdtime + 180);
		
		//something = *(int *)( *(int *)&pml[132] + 716)
		
		if(ps->pm_type <= 5 && pp->cmd.buttons & 0x10 && /*&& something*/v3 != 2 && v3 != 1 && v3 != 10 && v3 != 11 /*&& (*(int *)&pml[48] || pm_type == 1) )*/) {
			if(ps->pm_flags & 1) {
				if(!pp->oldcmd.flags & 0x10 || !pp->oldcmd.serverTime) {
					ps->pm_flags |= 0x20;
					*(byte*)&ps->pm_flags |= 4;
				}
			} else {
				ps->pm_flags |= 0x20;
			}
		} else {
			ps->pm_flags &= 0xDFu;
		}
		
		//for animations
		if ( ps->pm_flags & 0x20 )
			_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 1, 1);
		else
			_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 0, 1);
	#endif
	
	int *pp = (int*)pm;
	int *ps = *pp;
	int *gclient = *ps;
	
	int *v4 = (int *)(ps + 12);
	
	int val = *(int*)(gclient + 21); //336? 84*4=336 /84/4=21??
	
	//Com_DPrintf("val = %d\n", val);
	
	if (val == 1023 && x_cl_adsair->integer) {
		*v4 |= 0x20;
		return;
	}
		
	void (*call)();
	*(int*)&call=GAME("PM_UpdateAimDownSightFlag");
	call();
}

void BG_Link() {
	return; //experimental stuff and most people wont need this
	#ifdef xDEBUG
	Cmd_AddCommand("debug_dumpevents", __dump_events);
	#endif

	BG_GetNumWeapons = (int(*)())GAME("BG_GetNumWeapons");
	BG_GetInfoForWeapon = (int(*)(int))GAME("BG_GetInfoForWeapon");
	BG_AnimScriptEvent = (int(*)(playerState_t*,int,int,int))GAME("BG_AnimScriptEvent");
	PM_AddEvent = (void(*)(int))GAME("PM_AddEvent");
	
	//sub_37488
	//GAME("PM_InteruptWeaponWithProneMove");
	
	PM_SetReloadingState2 = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x434;
	PM_SetWeaponReloadAddAmmoDelay = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x330;
	
	#ifdef xDEBUG
	__jmp(GAME("PM_InteruptWeaponWithProneMove") + 0x434, (int)PM_SetReloadingState);
	
	__call(GAME("PmoveSingle")+0x455, (int)PM_Weapon);
	__call(GAME("PmoveSingle")+0x535, (int)PM_Weapon);
	
	__call(GAME("BG_PlayerTouchesItem") + 0xEA5, (int)PM_CheckJump);
	#endif
	/*
		aim in air if client allows it
		maybe add a groundEntityNum = 1023; force???
		- Richard
	*/
	//__jmp( dlsym(gamelib, "PM_UpdateAimDownSightFlag"), _PM_UpdateAimDownSightFlag);
	int thk = GAME("PmoveSingle");
	__call(thk + 0x3cc, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x3ea, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x404, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x441, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x49e, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x4d7, _PM_UpdateAimDownSightFlag);
	//__jmp( GAME("PM_ClearAimDownSightFlag"), _PM_ClearAimDownSightFlag);
	__call( thk + 0xFD, _PM_ClearAimDownSightFlag);
	__call( GAME("vmMain") - 0x1F119, _PM_ClearAimDownSightFlag);
}