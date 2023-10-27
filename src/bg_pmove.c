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
static void (*PM_SetReloadingState2)();

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

static void PM_SetReloadingState() {
	pmove_t *xm = *(pmove_t**)(int)pm;
	
	int clientNum = *(int*)((int)xm->ps + 172);
	xclient_t *xcl = &xclients[clientNum];
	
	int *weaponstate = (int*)((int)xm->ps + 180);
	int *weapons = *(int**)((int)xm->ps + 796);
	int weapon = *(int*)((int)xm->ps + 176);
	int *weaponTime = (int*)((int)xm->ps + 44);
	int *weaponDelay = (int*)((int)xm->ps + 48);
	
	int weaponinfo = BG_GetInfoForWeapon(weapon);
	
	int v2 = *(int*)((int)pml + 132);
	int event = EV_RELOAD_FROM_EMPTY, v3;
	if (*(int *)(4 * *(int *)(weaponinfo + 424) + (int)xm->ps + 524) || *(int *)(v2 + 112)) {
		if ( xm->ps->pm_type <= 5 ) {
			if (xm->cmd.wbuttons ) {
				v3 = *(int *)((int)xm->ps + 980) & 0x200;
				BYTE1(v3) ^= 2u;
				LOBYTE(v3) = 11;
				*(int *)((int)xm->ps + 980) = v3;
			}
		}
		if(!xcl->perks[PERK_QUICK_RELOAD])
		*weaponTime = *(int *)(v2 + 488);
		else
		*weaponTime = (int)(*(int *)(v2 + 488) / QUICK_RELOAD_FRACTION);
		event = EV_RELOAD;
	} else {
		if ( xm->ps->pm_type <= 5 ) {
			if (xm->cmd.wbuttons ) {
				v3 = *(int *)((int)xm->ps + 980) & 0x200;
				BYTE1(v3) ^= 2u;
				LOBYTE(v3) = 12;
				*(int *)((int)xm->ps + 980) = v3;
			}
		}
		if(!xcl->perks[PERK_QUICK_RELOAD])
		*weaponTime = *(int *)(v2 + 492);
		else
		*weaponTime = (int)(*(int *)(v2 + 492) / QUICK_RELOAD_FRACTION);
	}
	PM_AddEvent(event);
	if ( *weaponstate == 8 )
		*weaponstate = 6; //WEAPON_RELOADING_INTERRUPT??
	else
		*weaponstate = 5; //WEAPON_RELOADING
	//if(!reloadtime->integer)
	PM_SetWeaponReloadAddAmmoDelay();
	if(xcl->perks[PERK_QUICK_RELOAD])
		*weaponDelay = (int)(*weaponDelay / QUICK_RELOAD_FRACTION);
}

static void PM_BeginWeaponReload() { //works for kar98 sniper? rest not? mmhm
	
	pmove_t *xm = *(pmove_t**)(int)pm;
	
	int clientNum = *(int*)((int)xm->ps + 172);
	xclient_t *xcl = &xclients[clientNum];
	
	int *weapons = *(int**)((int)xm->ps + 796);
	int weapon = *(int*)((int)xm->ps + 176);
	int *weaponTime = (int*)((int)xm->ps + 44);
	int *weaponDelay = (int*)((int)xm->ps + 48);
	
	int weaponinfo = BG_GetInfoForWeapon(weapon);
	
	int *weaponstate = (int*)((int)xm->ps + 180);
	
	if(*weaponstate == WEAPON_READY || *weaponstate == WEAPON_FIRING || *weaponstate == WEAPON_RECHAMBERING) {
		int weapon = *(int*)((int)xm->ps + 176);
		if(weapon) {
			if(weapon <= BG_GetNumWeapons()) {
				
				int weaponinfo = BG_GetInfoForWeapon(weapon);
				
				if(!*(int*)(weaponinfo + 724))
					BG_AnimScriptEvent(xm->ps, 10, 0, 1);
				
				int v2 = *(int*)((int)pml + 132);
				
				if(*(int*)(v2 + 748) && *(int*)(v2 + 500)) {
					if(xm->ps->pm_type <= 5) {
						if(*(unsigned char*)((int)xm + 10)) {
							int v4 = *(int*)((int)xm->ps + 980) & 0x200;
							BYTE1(v4) ^= 2u;
							LOBYTE(v4) = 13;
							*(int*)((int)xm->ps + 980) = v4;
						}
					}
					if(xcl->perks[PERK_QUICK_RELOAD])
						*weaponTime = (int)(*(int*)(v2 + 500) / QUICK_RELOAD_FRACTION);
					else
						*weaponTime = *(int*)(v2 + 500);
					*weaponstate = WEAPON_RELOAD_START; //7
					
					PM_AddEvent(EV_RELOAD_START);
					PM_SetWeaponReloadAddAmmoDelay();//sub_377B8
					if(xcl->perks[PERK_QUICK_RELOAD])
						*weaponDelay = (int)(*weaponDelay / QUICK_RELOAD_FRACTION);
				} else {
					PM_SetReloadingState();//sub_378BC
				}
			}
		}
	}
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

static int PM_Weapon_CheckForRechamber(int time) {
	return 0;
	#define int_ptr_val(x) (*(int*)((int)x))
	pmove_t *xm = *(pmove_t**)(int)pm;
	
	#define ps_off(type, off) (*(type*)((int)xm->ps + off))
	
	int *weaponstate = (int*)((int)xm->ps + 180);
	int *weapons = *(int**)((int)xm->ps + 796);
	int weapon = *(int*)((int)xm->ps + 176);
	int *weaponTime = (int*)((int)xm->ps + 44);
	int *weaponDelay = (int*)((int)xm->ps + 48);
	
	int v2 = *(int*)((int)pml + 132);
	
	if(!int_ptr_val(v2 + 712))
		return 0;
	
	if(!COM_BitCheck(weapons, weapon))
		return 0;

	if(*weaponstate == WEAPON_RECHAMBERING) {
		if(time) {
			COM_BitClear(weapons, weapon);
			PM_AddEvent(EV_EJECT_BRASS);
			if(*weaponTime)
				return 1;
		}
	}
	
	if(!*weaponTime || ((*weaponstate - WEAPON_FIRING) > WEAPON_RAISING && *weaponstate != WEAPON_MELEE_WINDUP && *weaponstate != WEAPON_MELEE_RELAX && !*weaponDelay)) {
		if(*weaponstate == WEAPON_RECHAMBERING) {
			if(xm->cmd.wbuttons) {
				int *v9 = (int*)((int)xm->ps + 980);
				if(*v9 & 0xFFFFFDFF) {
					if(xm->ps->pm_type <= 5) {
						int v6 = *v9 & 0x200;
						BYTE1(v6) ^= 2;
						*v9 = v6;
					}
				}
			}
			*weaponstate = WEAPON_READY;
			return 0;
		}
	}
	
	if(*weaponstate == WEAPON_READY) {
		if(xm->ps->pm_type > 5 || !xm->cmd.wbuttons)
			goto label_27;
			/*
			
        v8 = 0.75 < *(float *)(v2 + 184);
        v9 = 0;
        v10 = 0.75 == *(float *)(v2 + 184);
        if ( (HIBYTE(v7) & 0x45) == 1 )
        {
          if ( *(_DWORD *)(v2 + 4) > 5 || !*(_BYTE *)(xm + 10) )
            goto LABEL_27;
          v11 = *(_DWORD *)(v2 + 980) & 0x200;
          BYTE1(v11) ^= 2u;
          LOBYTE(v11) = 7;
        }
        else
        {
          if ( *(_DWORD *)(v2 + 4) > 5 || !*(_BYTE *)(xm + 10) )
            goto LABEL_27;
          v11 = *(_DWORD *)(v2 + 980) & 0x200;
          BYTE1(v11) ^= 2u;
          LOBYTE(v11) = 4;
        }
        *(_DWORD *)(v2 + 980) = v11;
		*/
			//set cool stuff for keys?
		//ps_off(int,980) = 
		label_27:
		*weaponstate = WEAPON_RECHAMBERING;
		*weaponTime = int_ptr_val(v2 + 472);
		
		int v13 = int_ptr_val(v2 + 476);
		/*
		if(v13 && v13 < *weaponTime)
			*weaponDelay = v13;
		else
			*weaponDelay = 1;*/
		PM_AddEvent(EV_RECHAMBER_WEAPON);
	}
	
	return 0;
	#undef int_ptr_val
}

#if 0

static void PM_Weapon_CheckForReload() {
  pmove_t *xm = *(pmove_t**)(int)pm;
  
  int reloadRequested = xm->cmd.wbuttons & 8; //check for reload pressed
  
  
  
  v8 = *(_BYTE *)(xm + 9) & 8;
  v0 = 0;
  if ( *(_DWORD *)(*(_DWORD *)&pml[132] + 748) )
  {
    v2 = *(_DWORD *)xm;
    v1 = *(_DWORD *)(*(_DWORD *)xm + 180);
    if ( v1 == 7 || v1 == 5 )
    {
      if ( *(_BYTE *)(xm + 8) & 1 )
      {
        if ( !(*(_BYTE *)(xm + 32) & 1) )
        {
          if ( v1 == 7 )
          {
            *(_DWORD *)(v2 + 180) = 8;
          }
          else
          {
            if ( v1 == 5 )
              *(_DWORD *)(v2 + 180) = 6;
          }
        }
      }
    }
  }
  result = *(_DWORD *)(*(_DWORD *)xm + 180);
  if ( result < 1 || result > 2 && (result > 11 || result < 5) )
  {
    v6 = *(_DWORD *)(dword_7C91C + 4 * *(_DWORD *)(*(_DWORD *)xm + 176));
    v4 = *(_DWORD *)(v6 + 424);
    v5 = *(_DWORD *)(v6 + 416);
    if ( v8 )
    {
      if ( sub_3803C() )
        v0 = 1;
    }
    v7 = *(_DWORD *)xm;
    result = *(_DWORD *)xm + 524;
    if ( !*(_DWORD *)(result + 4 * v4) )
    {
      result = v7 + 268;
      if ( *(_DWORD *)(v7 + 268 + 4 * v5) )
      {
        if ( *(_DWORD *)(v7 + 180) != 3 )
        {
          if ( !(*(_BYTE *)(v7 + 12) & 1) || !*(_WORD *)(xm + 24) )
            v0 = 1;
        }
      }
    }
    if ( v0 )
      result = PM_BeginWeaponReload();
  }
  return result;
}

#endif //decided not to do cuz i dont need

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

// xoxor4d
void PM_ProjectVelocity(vec3_t in, vec3_t normal, vec3_t out) {
	float speedXY, DotNormalXY, normalisedNormalXY, projectionZ, projectionXYZ;

	speedXY = in[1] * in[1] + in[0] * in[0];

	if ((normal[2]) < 0.001f || (speedXY == 0.0f))
	{
		VectorCopy(in, out);
	}
	else
	{
		DotNormalXY = normal[1] * in[1] + in[0] * normal[0];
		normalisedNormalXY = -DotNormalXY / normal[2];

		projectionZ = in[2] * in[2] + speedXY;

		projectionXYZ = sqrtf((projectionZ / (speedXY + normalisedNormalXY * normalisedNormalXY)));

		if (projectionXYZ < 1.0f || normalisedNormalXY < 0.0f || in[2] > 0.0f)
		{
			out[0] = projectionXYZ * in[0];
			out[1] = projectionXYZ * in[1];
			out[2] = projectionXYZ * normalisedNormalXY;
		}
	}
}

cvar_t *x_cl_bounce;

void PM_Bounce(vec3_t in, vec3_t normal, vec3_t out) {
	if (x_cl_bounce->integer) {
		PM_ProjectVelocity(in, normal, out);
	} else {
		PM_ClipVelocity(in, normal, out, 1.001f);
	}
}

static void (*PM_CheckForChangeWeapon)();
static void (*PM_BeginWeaponChange)(int param_1, int param_2);
static void (*PM_FinishWeaponChange)();
void _PM_CheckForChangeWeapon()
{
	pmove_t *xm = *(pmove_t**)(int)pm;

	if ((xm->ps->pm_flags & 0x20000))
	{
		int *weapon = (int*)((int)xm->ps + 176);
		if (*weapon)
		{
			PM_BeginWeaponChange(*weapon, 0);
		}
		return;
	}
	PM_CheckForChangeWeapon();
}

void _PM_FinishWeaponChange()
{
	pmove_t *xm = *(pmove_t**)(int)pm;

	if ((xm->ps->pm_flags & 0x20000))
	{
		int *weapon = (int*)((int)xm->ps + 176);
		if (*weapon)
		{
			*weapon = 0;
		}
		return;
	}
	PM_FinishWeaponChange();
}

void BG_Link()
{
	__call(GAME("PM_StepSlideMove") + 0x40A, (int)PM_Bounce);

	__call(GAME("PM_Weapon") + 0x121, (int)_PM_CheckForChangeWeapon);
	PM_CheckForChangeWeapon = (void(*)())GAME("PM_AdjustAimSpreadScale") + 0x330;
	PM_BeginWeaponChange = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x614;
	__call(GAME("PM_Weapon") + 0x1BA, (int)_PM_FinishWeaponChange);
	PM_FinishWeaponChange = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x8FC;

	/*
		aim in air if client allows it
		maybe add a groundEntityNum = 1023; force???
		- Richard
	*/
	//__jmp( dlsym(gamelib, "PM_UpdateAimDownSightFlag"), _PM_UpdateAimDownSightFlag); //Segmentation fault
	int thk = GAME("PmoveSingle");
	__call(thk + 0x3cc, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x3ea, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x404, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x441, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x49e, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x4d7, _PM_UpdateAimDownSightFlag);
	//__jmp( GAME("PM_ClearAimDownSightFlag"), _PM_ClearAimDownSightFlag);//
	__call( thk + 0xFD, _PM_ClearAimDownSightFlag);
	__call( GAME("vmMain") - 0x1F119, _PM_ClearAimDownSightFlag);

	// Rest is experimental, most people won't need it.
	return;

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
	__jmp(GAME("PM_InteruptWeaponWithProneMove") + 0x530, (int)PM_BeginWeaponReload);
	//__jmp(GAME("PM_InteruptWeaponWithProneMove") + 0x148, (int)PM_Weapon_CheckForRechamber);
	
	__call(GAME("PmoveSingle")+0x455, (int)PM_Weapon);
	__call(GAME("PmoveSingle")+0x535, (int)PM_Weapon);
	
	__call(GAME("BG_PlayerTouchesItem") + 0xEA5, (int)PM_CheckJump);
	#endif
}