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

Netchan_Setup_t Netchan_Setup = (Netchan_Setup_t)0x808119C;

#if CODPATCH == 1
NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;
#else
NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;
#endif
NET_SendPacket_t NET_SendPacket = (NET_SendPacket_t)0x8080D28;

#define MAX_MSGLEN              32768

/*
===============
NET_OutOfBandPrint

Sends a text message in an out-of-band datagram
================
*/
/*
void QDECL NET_OutOfBandPrint( netsrc_t sock, netadr_t adr, const char *format, ... ) {
	va_list argptr;
	char string[MAX_MSGLEN];

	// set the header
	string[0] = -1;
	string[1] = -1;
	string[2] = -1;
	string[3] = -1;

	va_start( argptr, format );
	vsnprintf( string + 4, sizeof( string ) - 4, format, argptr );
	va_end( argptr );

	// send the datagram
	NET_SendPacket( sock, strlen( string ), string, adr );
}
*/

#if CODPATCH == 1
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x8080C38;
#else if CODPATCH == 5
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x80844E0;
#endif

const char  *NET_AdrToString (netadr_t a) {
    static  char    s[64];

    if (a.type == NA_LOOPBACK) {
        Com_sprintf (s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        Com_sprintf (s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i:%hu",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3], BigShort(a.port));
    } else {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x:%hu",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9], 
        BigShort(a.port));
    }

    return s;
}

const char  *NET_BaseAdrToString (netadr_t a) {
    static  char    s[64];

    if (a.type == NA_LOOPBACK) {
        Com_sprintf (s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        Com_sprintf (s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
    } else {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9]);
    }

    return s;
}

qboolean    NET_CompareAdr( netadr_t a, netadr_t b ) {
	if ( a.type != b.type ) {
		return qfalse;
	}

	if ( a.type == NA_LOOPBACK ) {
		return qtrue;
	}

	if ( a.type == NA_IP ) {
		if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port ) {
			return qtrue;
		}
		return qfalse;
	}

	if ( a.type == NA_IPX ) {
		if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) && a.port == b.port ) {
			return qtrue;
		}
		return qfalse;
	}

	Com_Printf( "NET_CompareAdr: bad address type\n" );
	return qfalse;
}


bool    NET_IsLocalAddress( netadr_t adr ) {
	return adr.type == NA_LOOPBACK;
}


qboolean    NET_CompareBaseAdr( netadr_t a, netadr_t b ) {
	if ( a.type != b.type ) {
		return qfalse;
	}

	if ( a.type == NA_LOOPBACK ) {
		return qtrue;
	}

	if ( a.type == NA_IP ) {
		if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] ) {
			return qtrue;
		}
		return qfalse;
	}

	if ( a.type == NA_IPX ) {
		if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) ) {
			return qtrue;
		}
		return qfalse;
	}


	Com_Printf( "NET_CompareBaseAdr: bad address type\n" );
	return qfalse;
}