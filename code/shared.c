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
#include "shared.h"

xentity_t xentities[1024];

void* gamelib;
int base;
char (*pml)[140];

level_locals_t *level;

gentity_t *g_entities;

char* modNames[] = {
    "MOD_UNKNOWN",
    "MOD_PISTOL_BULLET",
    "MOD_RIFLE_BULLET",
    "MOD_GRENADE",
    "MOD_GRENADE_SPLASH",
    "MOD_PROJECTILE",
    "MOD_PROJECTILE_SPLASH",
    "MOD_MELEE",
    "MOD_HEAD_SHOT",
    "MOD_MORTAR",
    "MOD_MORTAR_SPLASH",
    "MOD_KICKED",
    "MOD_GRABBER",
    "MOD_DYNAMITE",
    "MOD_DYNAMITE_SPLASH",
    "MOD_AIRSTRIKE",
    "MOD_WATER",
    "MOD_SLIME",
    "MOD_LAVA",
    "MOD_CRUSH",
    "MOD_TELEFRAG",
    "MOD_FALLING",
    "MOD_SUICIDE",
    "MOD_TRIGGER_HURT",
    "MOD_EXPLOSIVE"
};

#if PATCH == 1
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x805AEF8;
Com_Printf_t _STRIP Com_Printf = (Com_Printf_t)0x806B760;
Com_DPrintf_t _STRIP Com_DPrintf = (Com_DPrintf_t)0x806B79C;
Com_Error_t _STRIP Com_Error = (Com_Error_t)0x806B93C;
Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x805B258;
Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x805B24C;
Cmd_ArgvBuffer_t Cmd_ArgvBuffer = (Cmd_ArgvBuffer_t)0x805B27C;
Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x805B398;
VM_Call_t VM_Call = (VM_Call_t)0x8092158;
#else 
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x806043E;
Com_Printf_t Com_Printf = (Com_Printf_t)0x806FC10;
Com_DPrintf_t Com_DPrintf = (Com_DPrintf_t)0x806FC5F;
Com_Error_t Com_Error = (Com_Error_t)0x806FE74;
Cmd_Argv_t Cmd_Argv = (Cmd_Argv_t)0x80600F4;
Cmd_Argc_t Cmd_Argc = (Cmd_Argc_t)0x80600EA;
Cmd_ArgvBuffer_t Cmd_ArgvBuffer = (Cmd_ArgvBuffer_t)0x806014B;
Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x8060423;

VM_Call_t VM_Call = (VM_Call_t)0x809AFBC;
#endif

//Info_SetValueForKey_t Info_SetValueForKey = (Info_SetValueForKey_t)0x80827D4;

//============================================================================
/*
==================
COM_BitCheck

  Allows bit-wise checks on arrays with more than one item (> 32 bits)
==================
*/
qboolean COM_BitCheck( const int array[], int bitNum ) {
	int i;

	i = 0;
	while ( bitNum > 31 ) {
		i++;
		bitNum -= 32;
	}

	return ( ( array[i] & ( 1 << bitNum ) ) != 0 );  // (SA) heh, whoops. :)
}

/*
==================
COM_BitSet

  Allows bit-wise SETS on arrays with more than one item (> 32 bits)
==================
*/
void COM_BitSet( int array[], int bitNum ) {
	int i;

	i = 0;
	while ( bitNum > 31 ) {
		i++;
		bitNum -= 32;
	}

	array[i] |= ( 1 << bitNum );
}

/*
==================
COM_BitClear

  Allows bit-wise CLEAR on arrays with more than one item (> 32 bits)
==================
*/
void COM_BitClear( int array[], int bitNum ) {
	int i;

	i = 0;
	while ( bitNum > 31 ) {
		i++;
		bitNum -= 32;
	}

	array[i] &= ~( 1 << bitNum );
}

/*
============================================================================

					LIBRARY REPLACEMENT FUNCTIONS

============================================================================
*/

int _STRIP Q_isprint( int c ) {
	if ( c >= 0x20 && c <= 0x7E ) {
		return ( 1 );
	}
	return ( 0 );
}

int _STRIP Q_islower( int c ) {
	if ( c >= 'a' && c <= 'z' ) {
		return ( 1 );
	}
	return ( 0 );
}

int _STRIP Q_isupper( int c ) {
	if ( c >= 'A' && c <= 'Z' ) {
		return ( 1 );
	}
	return ( 0 );
}

int _STRIP Q_isalpha( int c ) {
	if ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) {
		return ( 1 );
	}
	return ( 0 );
}

int _STRIP Q_isnumeric( int c ) {
	if ( c >= '0' && c <= '9' ) {
		return ( 1 );
	}
	return ( 0 );
}

int _STRIP Q_isalphanumeric( int c ) {
	if ( Q_isalpha( c ) ||
		 Q_isnumeric( c ) ) {
		return( 1 );
	}
	return ( 0 );
}

int _STRIP Q_isforfilename( int c ) {
	if ( ( Q_isalphanumeric( c ) || c == '_' ) && c != ' ' ) { // space not allowed in filename
		return( 1 );
	}
	return ( 0 );
}

char* _STRIP Q_strrchr( const char* string, int c ) {
	char cc = c;
	char *s;
	char *sp = (char *)0;

	s = (char*)string;

	while ( *s )
	{
		if ( *s == cc ) {
			sp = s;
		}
		s++;
	}
	if ( cc == 0 ) {
		sp = s;
	}

	return sp;
}

/*
=============
Q_strncpyz

Safe strncpy that ensures a trailing zero
=============
*/
void _STRIP Q_strncpyz( char *dest, const char *src, int destsize ) {
	if ( !src ) {
		Com_Error( ERR_FATAL, "Q_strncpyz: NULL src" );
	}
	if ( destsize < 1 ) {
		Com_Error( ERR_FATAL,"Q_strncpyz: destsize < 1" );
	}

	strncpy( dest, src, destsize - 1 );
	dest[destsize - 1] = 0;
}

const char* _STRIP Q_stristr( const char *s, const char *find) {
  register char c, sc;
  register size_t len;

  if ((c = *find++) != 0)
  {
    if (c >= 'a' && c <= 'z')
    {
      c -= ('a' - 'A');
    }
    len = strlen(find);
    do
    {
      do
      {
        if ((sc = *s++) == 0)
          return NULL;
        if (sc >= 'a' && sc <= 'z')
        {
          sc -= ('a' - 'A');
        }
      } while (sc != c);
    } while (Q_stricmpn(s, find, len) != 0);
    s--;
  }
  return s;
}

int _STRIP stricmp (const char *p1, const char *p2)
{
  register unsigned char *s1 = (unsigned char *) p1;
  register unsigned char *s2 = (unsigned char *) p2;
  unsigned char c1, c2;

  do
  {
      c1 = (unsigned char) toupper((int)*s1++);
      c2 = (unsigned char) toupper((int)*s2++);
      if (c1 == '\0')
      {
            return c1 - c2;
      }
  }
  while (c1 == c2);

  return c1 - c2;
}

void QDECL _STRIP Com_sprintf( char *dest, int size, const char *fmt, ... ) {
	int ret;
	va_list argptr;

	va_start( argptr,fmt );
	ret = vsnprintf( dest, size, fmt, argptr ); //Q_vsnprintf
	va_end( argptr );
	if ( ret == -1 ) {
		printf( "Com_sprintf: overflow of %i bytes buffer\n", size );
	}
}

#define Q_COLOR_ESCAPE   '^'
#define Q_IsColorString(p)   ( p && *(p) == Q_COLOR_ESCAPE && *((p)+1) && *((p)+1) != Q_COLOR_ESCAPE )

char * _STRIP Q_CleanStr( char *string ) {
	char*   d;
	char*   s;
	int c;

	s = string;
	d = string;
	while ( ( c = *s ) != 0 ) {
		if ( Q_IsColorString( s ) ) {
			s++;
		} else if ( c >= 0x20 && c <= 0x7E )   {
			*d++ = c;
		}
		s++;
	}
	*d = '\0';

	return string;
}

void _STRIP Info_Print( const char *s ) {
	char key[512];
	char value[512];
	char    *o;
	int l;

	if ( *s == '\\' ) {
		s++;
	}
	while ( *s )
	{
		o = key;
		while ( *s && *s != '\\' )
			*o++ = *s++;

		l = o - key;
		if ( l < 20 ) {
			memset( o, ' ', 20 - l );
			key[20] = 0;
		} else {
			*o = 0;
		}
		Com_Printf( "%s", key );

		if ( !*s ) {
			Com_Printf( "MISSING VALUE\n" );
			return;
		}

		o = value;
		s++;
		while ( *s && *s != '\\' )
			*o++ = *s++;
		*o = 0;

		if ( *s ) {
			s++;
		}
		Com_Printf( "%s\n", value );
	}
}

/*
===============
Info_ValueForKey

Searches the string for the given
key and returns the associated value, or an empty string.
FIXME: overflow check?
===============
*/
char* _STRIP Info_ValueForKey( const char *s, const char *key ) {
	char pkey[BIG_INFO_KEY];
	static char value[2][BIG_INFO_VALUE];   // use two buffers so compares
											// work without stomping on each other
	static int valueindex = 0;
	char    *o;

	if ( !s || !key ) {
		return "";
	}

	if ( strlen( s ) >= BIG_INFO_STRING ) {
		Com_Error(ERR_FATAL, "Info_ValueForKey: oversize infostring [%s] [%s]", s, key );
	}

	valueindex ^= 1;
	if ( *s == '\\' ) {
		s++;
	}
	while ( 1 )
	{
		o = pkey;
		while ( *s != '\\' )
		{
			if ( !*s ) {
				return "";
			}
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value[valueindex];

		while ( *s != '\\' && *s )
		{
			*o++ = *s++;
		}
		*o = 0;

		if ( !stricmp( key, pkey ) ) {
			return value[valueindex];
		}

		if ( !*s ) {
			break;
		}
		s++;
	}

	return "";
}

/*
===================
Info_RemoveKey
===================
*/
void _STRIP Info_RemoveKey( char *s, const char *key ) {
	char    *start;
	char pkey[MAX_INFO_KEY];
	char value[MAX_INFO_VALUE];
	char    *o;

	if ( strlen( s ) >= MAX_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_RemoveKey: oversize infostring [%s] [%s]", s, key );
		return;
	}

	if ( strchr( key, '\\' ) ) {
		return;
	}

	while ( 1 )
	{
		start = s;
		if ( *s == '\\' ) {
			s++;
		}
		o = pkey;
		while ( *s != '\\' )
		{
			if ( !*s ) {
				return;
			}
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while ( *s != '\\' && *s )
		{
			if ( !*s ) {
				return;
			}
			*o++ = *s++;
		}
		*o = 0;

		if ( !Q_stricmp( key, pkey ) ) {
			// rain - arguments to strcpy must not overlap
			//strcpy (start, s);	// remove this part
			memmove( start, s, strlen( s ) + 1 ); // remove this part
			return;
		}

		if ( !*s ) {
			return;
		}
	}

}

/*
==================
Info_SetValueForKey

Changes or adds a key/value pair
==================
*/
void _STRIP Info_SetValueForKey( char *s, const char *key, const char *value ) {
	char newi[MAX_INFO_STRING];
	
	if ( strlen( s ) >= MAX_INFO_STRING ) {
		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring [%s] [%s] [%s]", s, key, value );
		return;
	}

	if ( strchr( key, '\\' ) || strchr( value, '\\' ) ) {
		printf( "Can't use keys or values with a \\\n" );
		return;
	}

	if ( strchr( key, ';' ) || strchr( value, ';' ) ) {
		printf( "Can't use keys or values with a semicolon\n" );
		return;
	}

	if ( strchr( key, '\"' ) || strchr( value, '\"' ) ) {
		printf( "Can't use keys or values with a \"\n" );
		return;
	}

	Info_RemoveKey( s, key );
	if ( !value || !strlen( value ) ) {
		return;
	}

	Com_sprintf( newi, sizeof( newi ), "\\%s\\%s", key, value );

	if ( strlen( newi ) + strlen( s ) > MAX_INFO_STRING ) {
		printf( "Info string length exceeded\n" );
		return;
	}

	strcat( s, newi );
}

int _STRIP Q_stricmpn( const char *s1, const char *s2, int n ) {
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( !n-- ) {
			return 0;       // strings are equal until end point
		}

		if ( c1 != c2 ) {
			if ( c1 >= 'a' && c1 <= 'z' ) {
				c1 -= ( 'a' - 'A' );
			}
			if ( c2 >= 'a' && c2 <= 'z' ) {
				c2 -= ( 'a' - 'A' );
			}
			if ( c1 != c2 ) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while ( c1 );

	return 0;       // strings are equal
}

int _STRIP Q_strncmp( const char *s1, const char *s2, int n ) {
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if ( !n-- ) {
			return 0;       // strings are equal until end point
		}

		if ( c1 != c2 ) {
			return c1 < c2 ? -1 : 1;
		}
	} while ( c1 );

	return 0;       // strings are equal
}

int _STRIP Q_stricmp( const char *s1, const char *s2 ) {
	return ( s1 && s2 ) ? Q_stricmpn( s1, s2, 99999 ) : -1;
}

char *Q_strlwr( char *s1 ) {
	char*   s;

	for ( s = s1; *s; ++s ) {
		if ( ( 'A' <= *s ) && ( *s <= 'Z' ) ) {
			*s -= 'A' - 'a';
		}
	}

	return s1;
}

char _STRIP *Q_strupr( char *s1 ) {
	char* cp;

	for ( cp = s1 ; *cp ; ++cp ) {
		if ( ( 'a' <= *cp ) && ( *cp <= 'z' ) ) {
			*cp += 'A' - 'a';
		}
	}

	return s1;
}


// never goes past bounds or leaves without a terminating 0
void _STRIP Q_strcat( char *dest, int size, const char *src ) {
	int l1;

	l1 = strlen( dest );
	if ( l1 >= size ) {
		Com_Error( ERR_FATAL, "Q_strcat: already overflowed" );
	}
	Q_strncpyz( dest + l1, src, size - l1 );
}

/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
FIXME: make this buffer size safe someday

Ridah, modified this into a circular list, to further prevent stepping on
previous strings
============
*/

char    * _STRIP QDECL va( char *format, ... ) {
	va_list argptr;
	#define MAX_VA_STRING   32000
	static char temp_buffer[MAX_VA_STRING];
	static char string[MAX_VA_STRING];      // in case va is called by nested functions
	static int index = 0;
	char    *buf;
	int len;


	va_start( argptr, format );
	vsprintf( temp_buffer, format,argptr );
	va_end( argptr );

	if ( ( len = strlen( temp_buffer ) ) >= MAX_VA_STRING ) {
		Com_Error( ERR_DROP, "Attempted to overrun string in call to va()\n" );
	}

	if ( len + index >= MAX_VA_STRING - 1 ) {
		index = 0;
	}

	buf = &string[index];
	memcpy( buf, temp_buffer, len + 1 );

	index += len + 1;

	return buf;
}
