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

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> // bk001204

#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <errno.h>
#include <net/if.h>

void SV_NocPacket(netadr_t,msg_t*);

void SockadrToNetadr( struct sockaddr_in *s, netadr_t *a ) {
	*(int *)&a->ip = *(int *)&s->sin_addr;
	a->port = s->sin_port;
	a->type = NA_IP;
}

qboolean    Sys_GetPacket( netadr_t *net_from, msg_t *net_message ) {
	int ret;
	struct sockaddr_in from;
	int fromlen;
	int net_socket;
	int protocol;
	int err;

	for ( protocol = 0 ; protocol < 2 ; protocol++ )
	{
		/*
		if ( protocol == 0 ) {
			//net_socket = ip_socket;
		} else {
			//net_socket = ipx_socket;
			
		}
		*/
		net_socket = (protocol == 0) ? *(int*)0x80E688C : *(int*)0x83C8700;
		
		if ( !net_socket ) {
			continue;
		}

		fromlen = sizeof( from );
		ret = recvfrom( net_socket, net_message->data, net_message->maxsize
						, 0, (struct sockaddr *)&from, &fromlen );

		SockadrToNetadr( &from, net_from );
		// bk000305: was missing
		net_message->readcount = 0;

		if ( ret == -1 ) {
			err = errno;

			if ( err == EWOULDBLOCK || err == ECONNREFUSED ) {
				continue;
			}
			//Com_Printf( "NET_GetPacket: %s from %s\n", NET_ErrorString(), NET_AdrToString( *net_from ) );
			continue;
		}
		
		if ( ret == net_message->maxsize ) {
			Com_Printf( "Oversize packet from %s\n", NET_AdrToString( *net_from ) );
			continue;
		}

		net_message->cursize = ret;
		SV_NocPacket(*net_from, net_message);
		return qtrue;
	}

	return qfalse;
}