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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "steam/steam_api.h"
#include "steam/isteamuserstats.h"
#include "steam/isteamremotestorage.h"
#include "steam/isteammatchmaking.h"
#include "steam/isteamgameserver.h"
#include "steam/steam_gameserver.h"

#define STEAM_COD1_APPID 2620
#define STEAM_APPID STEAM_COD1_APPID

//#define USE_GS_AUTH_API

extern "C" {
	void    Q_strncpyz( char *dest, const char *src, int destsize );
	void Com_sprintf( char *dest, int size, const char *fmt, ... );
	char* Cvar_VariableString(const char*);
	
	#include "server.h"

}

class CoDSteamServer {
public:
	bool initialized;

	CoDSteamServer( uint32 unIP, uint16 usSteamPort, uint16 usGamePort, uint16 usQueryPort, const char *pchVersionString)
:
	m_CallbackSteamServersConnected( this, &CoDSteamServer::OnSteamServersConnected ),
	m_CallbackSteamServersConnectFailure( this, &CoDSteamServer::OnSteamServersConnectFailure ),
	m_CallbackSteamServersDisconnected( this, &CoDSteamServer::OnSteamServersDisconnected )	
	#if 0
	m_CallbackGSAuthTicketResponse( this, &SteamGameServerBridge::OnValidateAuthTicketResponse ),
	m_CallbackPolicyResponse( this, &SteamGameServerBridge::OnPolicyResponse )
	#endif
	{
		initialized = false;

		const char* pchGameDir = "";
		m_bConnectedToSteam = false;


		bool use_shared_udp = true;

		if ( !SteamGameServer_Init( unIP, usSteamPort, usGamePort, use_shared_udp ? MASTERSERVERUPDATERPORT_USEGAMESOCKETSHARE : usQueryPort, eServerModeAuthentication, pchVersionString ) )
		{
	
			printf("SteamGameServer_Init(...) failed!\n");
			return;
		}
	
		if(!SteamGameServer()) {
			printf("SteamGameServer() is NULL\n");
			return;
		}
	
		initialized = true;
	
	
		// Set the "game dir".
		// This is currently required for all games.  However, soon we will be
		// using the AppID for most purposes, and this string will only be needed
		// for mods.  it may not be changed after the server has logged on
		SteamGameServer()->SetModDir( pchGameDir );
	
		// These fields are currently required, but will go away soon.
		// See their documentation for more info
		SteamGameServer()->SetProduct( "Call of Duty Extended" );
		SteamGameServer()->SetGameDescription( "Call of Duty Extended" );
	
		// Is this is a dedicated server?  The default value is false.
		SteamGameServer()->SetDedicatedServer( true );
		
		// We don't support specators in our game.
		// .... but if we did:
		//SteamGameServer()->SetSpectatorPort( ... );
		//SteamGameServer()->SetSpectatorServerName( ... );
	
		// Initiate Anonymous logon.
		// Coming soon: Logging into authenticated, persistent game server account
		SteamGameServer()->LogOnAnonymous();
	
		// We want to actively update the master server with our presence so players can
		// find us via the steam matchmaking/server browser interfaces
		//#ifdef USE_GS_AUTH_API
			SteamGameServer()->EnableHeartbeats( true );
		//#endif
	}


	~CoDSteamServer() {
		if(SteamGameServer() == NULL)
			return;

#ifdef USE_GS_AUTH_API
		SteamGameServer()->EnableHeartbeats(false);
#endif
		SteamGameServer()->LogOff();
		SteamGameServer_Shutdown();
		//printf("~CoDSteamServer() Deconstructed!!!!!!!!!!!\n");
	}

	bool IsConnectedToSteam()		{ return m_bConnectedToSteam; }
	CSteamID GetSteamID();

	void SendUpdatedServerDetailsToSteam();

private:
	//
	// Various callback functions that Steam will call to let us know about events related to our
	// connection to the Steam servers for authentication purposes.
	//


	// Tells us when we have successfully connected to Steam
	STEAM_GAMESERVER_CALLBACK( CoDSteamServer, OnSteamServersConnected, SteamServersConnected_t, m_CallbackSteamServersConnected );

	// Tells us when there was a failure to connect to Steam
	STEAM_GAMESERVER_CALLBACK( CoDSteamServer, OnSteamServersConnectFailure, SteamServerConnectFailure_t, m_CallbackSteamServersConnectFailure );

	// Tells us when we have been logged out of Steam
	STEAM_GAMESERVER_CALLBACK( CoDSteamServer, OnSteamServersDisconnected, SteamServersDisconnected_t, m_CallbackSteamServersDisconnected );


	// Track whether our server is connected to Steam ok (meaning we can restrict who plays based on 
	// ownership and VAC bans, etc...)
	bool m_bConnectedToSteam;
};


void CoDSteamServer::OnSteamServersConnected( SteamServersConnected_t *pLogonSuccess )
{
	//printf("CoDSteamServer::OnSteamServersConnected(...)\n");
	printf("SteamServer Connected\n");
	m_bConnectedToSteam = true;

	// log on is not finished until OnPolicyResponse() is called

	// Tell Steam about our server details
	SendUpdatedServerDetailsToSteam();
}


void CoDSteamServer::OnSteamServersConnectFailure( SteamServerConnectFailure_t *pConnectFailure )
{
	printf("CoDSteamServer::OnSteamServersConnectFailure(...)\n");
	m_bConnectedToSteam = false;
}

void CoDSteamServer::OnSteamServersDisconnected( SteamServersDisconnected_t *pLoggedOff )
{
	//printf("CoDSteamServer::OnSteamServersDisconnected(...)\n");
	printf("SteamServer Disconnected.\n");
	m_bConnectedToSteam = false;
}

void CoDSteamServer::SendUpdatedServerDetailsToSteam()
{

	// Tell the Steam authentication servers about our game
	char rgchServerName[128];
	strcpy( rgchServerName, Cvar_VariableString("sv_hostname") );

	//
	// Set state variables, relevant to any master server updates or client pings
	//

	// These server state variables may be changed at any time.  Note that there is no lnoger a mechanism
	// to send the player count.  The player count is maintained by steam and you should use the player
	// creation/authentication functions to maintain your player count.
	SteamGameServer()->SetMaxPlayerCount( atoi(Cvar_VariableString("sv_maxclients")) );
	SteamGameServer()->SetPasswordProtected( false );
	SteamGameServer()->SetServerName( rgchServerName );
	SteamGameServer()->SetBotPlayerCount( 0 ); // optional, defaults to zero
	SteamGameServer()->SetMapName( Cvar_VariableString("mapname") );

#ifdef USE_GS_AUTH_API22

	// Update all the players names/scores
	for( uint32 i=0; i < MAX_PLAYERS_PER_SERVER; ++i )
	{
		if ( m_rgClientData[i].m_bActive && m_rgpShips[i] )
		{
			SteamGameServer()->BUpdateUserData( m_rgClientData[i].m_SteamIDUser, m_rgpShips[i]->GetPlayerName(), m_rguPlayerScores[i] );
		}
	}
#endif

	// game type is a special string you can use for your game to differentiate different game play types occurring on the same maps
	// When users search for this parameter they do a sub-string search of this string 
	// (i.e if you report "abc" and a client requests "ab" they return your server)
	//SteamGameServer()->SetGameType( "dm" );

	// update any rule values we publish
	//SteamMasterServerUpdater()->SetKeyValue( "rule1_setting", "value" );
	//SteamMasterServerUpdater()->SetKeyValue( "rule2_setting", "value2" );
}


extern "C" {
	
int CSteamClient_Init() {

	if(!SteamAPI_Init())
		return 0;
	if(!SteamFriends() || !SteamClient())
		return 0;

	return 1;
}

static CoDSteamServer *codsteamserver = NULL;

int CSteamServer_HandleIncomingPacket(const void* pData, int cbData, unsigned int srcIP, unsigned short srcPort) {
	if(codsteamserver == NULL)
		return 0;
	
	if(SteamGameServer()->HandleIncomingPacket(pData, cbData, srcIP, srcPort))
		return 1;
	return 0;
}

int CSteamServer_Init(uint32_t ip, uint16_t steamPort, uint16_t gamePort,
	uint16_t queryPort, const char *versionString) {
	if(codsteamserver != NULL) {
		printf("Already server started!!!! STEAM\n");
		return 0;
	}

	codsteamserver = new CoDSteamServer(ip, steamPort, gamePort, queryPort, versionString);

	if(codsteamserver == NULL)
		return 0;

	if(!codsteamserver->initialized) {

		delete codsteamserver;
		codsteamserver = NULL;
		return 0;
	}

	return 1;

}

void CSteamServer_Shutdown() {
	if(codsteamserver == NULL)
		return;
	delete codsteamserver;
	codsteamserver = NULL;
}

void CSteamServer_RunFrame() {
	if(codsteamserver == NULL)
		return;
	uint32 ip;
	uint16 port;
	char packet[32000] = {0};
	int result;
	while( (result = SteamGameServer()->GetNextOutgoingPacket((void*)&packet,sizeof(packet),&ip,&port)) > 0) {
		//printf("ip = %lu, port = %u, packet = %s\n", ip, port, packet);
		netadr_t to;
		to.type = NA_IP;
		to._ip = ip;
		to.port = port;
		NET_SendPacket(NS_SERVER, result, packet, to);
		memset(packet,0,sizeof(packet));
	}
	
	SteamGameServer_RunCallbacks();

	codsteamserver->SendUpdatedServerDetailsToSteam();
}

char *CSteamClient_GetPersonaName(char *s, int l) {
	Q_strncpyz(s, SteamFriends()->GetPersonaName(), l);
}

void CSteamClient_Shutdown() {
	SteamAPI_Shutdown();
}

}