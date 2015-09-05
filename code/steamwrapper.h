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

#define STEAM_SUPPORT

#ifdef STEAM_SUPPORT

int CSteamClient_Init();
void CSteamClient_Shutdown();
char *CSteamClient_GetPersonaName();

//Server Functions
void CSteamServer_Shutdown();
int CSteamServer_Init(uint32_t ip, uint16_t steamPort, uint16_t gamePort,
	uint16_t queryPort, const char *versionString);
void CSteamServer_RunFrame();

#endif