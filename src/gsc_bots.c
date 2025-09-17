#include "script.h"

void gsc_bots_setbotstance(scr_entref_t ref)
{
    int id = ref.entnum;
    char *stance = Scr_GetString(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setbotstance() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setbotstance() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if (!strcmp(stance, "stand"))
    {
        customPlayerState[id].botUpMove &= ~(-KEY_MASK_JUMP | KEY_MASK_JUMP);
        customPlayerState[id].botWButtons &= ~(KEY_MASK_CROUCH | KEY_MASK_PRONE);
    }
    else if(!strcmp(stance, "crouch"))
        customPlayerState[id].botWButtons |= KEY_MASK_CROUCH;
    else if(!strcmp(stance, "prone"))
        customPlayerState[id].botWButtons |= KEY_MASK_PRONE;
    else if(!strcmp(stance, "jump"))
        customPlayerState[id].botUpMove |= KEY_MASK_JUMP;
    else
    {
        stackError("gsc_bots_setbotstance() invalid argument '%s'. Valid arguments are: 'stand' 'crouch' 'prone' 'jump'", stance);
        Scr_AddUndefined();
        return;
    }

    Scr_AddBool(qtrue);
}

void gsc_bots_meleeweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int melee = Scr_GetInt(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_meleeweapon() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_meleeweapon() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if(!melee)
        customPlayerState[id].botButtons &= ~KEY_MASK_MELEE;
    else
        customPlayerState[id].botButtons |= KEY_MASK_MELEE;

    Scr_AddBool(qtrue);
}

void gsc_bots_setaim(scr_entref_t ref)
{
    int id = ref.entnum;
    int ads = Scr_GetInt(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setaim() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setaim() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if(!ads)
        customPlayerState[id].botButtons &= ~KEY_MASK_ADS_MODE;
    else
        customPlayerState[id].botButtons |= KEY_MASK_ADS_MODE;

    Scr_AddBool(qtrue);
}

void gsc_bots_fireweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int shoot = Scr_GetInt(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_fireweapon() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_fireweapon() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if(!shoot)
        customPlayerState[id].botButtons &= ~KEY_MASK_FIRE;
    else
        customPlayerState[id].botButtons |= KEY_MASK_FIRE;

    Scr_AddBool(qtrue);
}

void gsc_bots_setlean(scr_entref_t ref)
{
    int id = ref.entnum;
    char *lean = Scr_GetString(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setlean() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setlean() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if(!strcmp(lean, "none"))
        customPlayerState[id].botWButtons &= ~(KEY_MASK_LEANLEFT | KEY_MASK_LEANRIGHT);
    else if(!strcmp(lean, "left"))
        customPlayerState[id].botWButtons |= KEY_MASK_LEANLEFT;
    else if(!strcmp(lean, "right"))
        customPlayerState[id].botWButtons |= KEY_MASK_LEANRIGHT;
    else
    {
        stackError("gsc_bots_setlean() invalid argument '%s'. Valid arguments are: 'right' 'left'", lean);
        Scr_AddUndefined();
        return;
    }

    Scr_AddBool(qtrue);
}

void gsc_bots_reloadweapon(scr_entref_t ref)
{
    int id = ref.entnum;
    int reload = Scr_GetInt(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_reloadweapon() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_reloadweapon() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if(!reload)
        customPlayerState[id].botWButtons &= ~KEY_MASK_RELOAD;
    else
        customPlayerState[id].botWButtons |= KEY_MASK_RELOAD;

    Scr_AddBool(qtrue);
}

void gsc_bots_switchtoweaponid(scr_entref_t ref)
{
    int id = ref.entnum;
    int weaponid = Scr_GetInt(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_switchtoweaponid() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_switchtoweaponid() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    customPlayerState[id].botWeapon = weaponid;

    Scr_AddBool(qtrue);
}

void gsc_bots_setwalkdir(scr_entref_t ref)
{
    int id = ref.entnum;
    char *dir = Scr_GetString(0);

    if (id >= MAX_CLIENTS)
    {
        stackError("gsc_bots_setwalkdir() entity %i is not a player", id);
        Scr_AddUndefined();
        return;
    }

    client_t *client = &svs.clients[id];

    if (client->netchan.remoteAddress.type != NA_BOT)
    {
        stackError("gsc_bots_setwalkdir() player %i is not a bot", id);
        Scr_AddUndefined();
        return;
    }

    if (!strcmp(dir, "none"))
    {
        customPlayerState[id].botForwardMove = KEY_MASK_NONE;
        customPlayerState[id].botRightMove = KEY_MASK_NONE;
    }
    else if(!strcmp(dir, "forward"))
        customPlayerState[id].botForwardMove = KEY_MASK_FORWARD;
    else if(!strcmp(dir, "back"))
        customPlayerState[id].botForwardMove = KEY_MASK_BACK;
    else if(!strcmp(dir, "right"))
        customPlayerState[id].botRightMove = KEY_MASK_MOVERIGHT;
    else if(!strcmp(dir, "left"))
        customPlayerState[id].botRightMove = KEY_MASK_MOVELEFT;
    else
    {
        stackError("gsc_bots_setwalkdir() invalid argument '%s'. Valid arguments are: 'none' 'forward' 'back' 'right' 'left'", dir);
        Scr_AddUndefined();
        return;
    }

    Scr_AddBool(qtrue);
}

void gsc_bots_setwalkvalues(scr_entref_t ref)
{
    int id = ref.entnum;
    int fwcount = Scr_GetInt(0);
    int rgcount = Scr_GetInt(1);

    customPlayerState[id].botForwardMove = fwcount;
    customPlayerState[id].botRightMove = rgcount;

    Scr_AddBool(qtrue);
}