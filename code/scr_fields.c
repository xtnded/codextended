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

#include "script.h"

void Scr_ReadOnlyEntityMemberfield(gentity_t *self) {
	Scr_Error("Tried to set a read only entity field");
}

void _Scr_SetOrigin(gentity_t *self) {
	vec3_t org;
	Scr_GetVector(0, &org);
	G_SetOrigin(self, org);
	if(self->linked)
		T_LinkEntity(self);
}

void _Scr_SetAngles(gentity_t *self) {
	vec3_t angles;
	Scr_GetVector(0, &angles);
	G_SetAngle(self, angles);
}

void _Scr_SetHealth(gentity_t *self) {
	int health = Scr_GetInt(0);
	self->health = health;
	if(self->client)
		self->client->ps.health = health;
	else
		self->maxhealth = health;
		
}

scr_entityfield_t scr_entityfields[] = {
	{"classname", 374, 3, Scr_ReadOnlyEntityMemberfield},
	{"origin", 308, 4, _Scr_SetOrigin},
	{"model", 373, 8, Scr_ReadOnlyEntityMemberfield},
	{"spawnflags", 376, 0, Scr_ReadOnlyEntityMemberfield},
	{"speed", 480, 1, 0},
	{"closespeed", 484, 1, 0},
	{"target", 468, 3, 0},
	{"targetname", 470, 3, 0},
	{"message", 456, 3, 0},
	{"teamname", 472, 3, 0},
	{"wait", 616, 1, 0},
	{"random", 620, 1, 0},
	{"count", 592, 0, 0},
	{"health", 560, 0, _Scr_SetHealth},
	{"light", 0, 9, 0},
	{"dmg", 568, 0, 0},
	{"angles", 320, 4, _Scr_SetAngles},
	{"duration", 636, 1, 0},
	{"rotate", 640, 4, 0},
	{"degrees", 464, 1, 0},
	{"time", 480, 1, 0},
	{"_color", 668, 4, 0},
	{"color", 668, 4, 0},
	{"key", 680, 0, 0},
	{"harc", 684, 1, 0},
	{"varc", 688, 1, 0},
	{"delay", 628, 1, 0},
	{"radius", 624, 0, 0},
	{"missionlevel", 692, 0, 0},
	{"start_size", 700, 0, 0},
	{"end_size", 704, 0, 0},
	{"shard", 592, 0, 0},
	{"spawnitem", 712, 3, 0},
	{"track", 732, 3, 0},
	{0,0,0,0}
};