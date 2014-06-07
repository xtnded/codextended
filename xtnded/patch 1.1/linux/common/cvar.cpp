#include "../game/shared.hpp"

static Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x806E9B4;
Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x806ECD4;
Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x806EA34;

/*
============
Cvar_VariableValue
============
*/
float Cvar_VariableValue( const char *var_name ) {
	cvar_t  *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return 0;
	}
	return var->value;
}


/*
============
Cvar_VariableIntegerValue
============
*/
int Cvar_VariableIntegerValue( const char *var_name ) {
	cvar_t  *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return 0;
	}
	return var->integer;
}


/*
============
Cvar_VariableString
============
*/
char *Cvar_VariableString( const char *var_name ) {
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return "";
	}
	return var->string;
}


/*
============
Cvar_VariableStringBuffer
============
*/
void Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		*buffer = 0;
	} else {
		Q_strncpyz( buffer, var->string, bufsize );
	}
}

/*
============
Cvar_VariableStringBuffer
============
*/
void Cvar_LatchedVariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		*buffer = 0;
	} else {
		if ( var->latchedString ) {
			Q_strncpyz( buffer, var->latchedString, bufsize );
		} else {
			Q_strncpyz( buffer, var->string, bufsize );
		}
	}
}
