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

void Scr_StrTok(int a1) {
	char* str = Scr_GetString(0);
	char* delim = Scr_GetString(1);
	char* tok = strtok(str, delim);
	Scr_MakeArray();
	while(NULL!=tok) {
		Scr_AddString(tok);
		Scr_AddArray();
		tok = strtok(NULL, delim);
	}
}

void Scr_IsSubStr(int a1) {
	char* string = Scr_GetString(0);
	char* substring = Scr_GetString(1);

	Scr_AddBool( ( strstr(string, substring) != NULL ) );
}

void Scr_ToLower(int a1) {
	char* str = Scr_GetString(0);
	char* it = str;
	for(;*it;++it)
		*it = tolower(*it);
	Scr_AddString(str);
}

void Scr_ToUpper(int a1) {
	char* str = Scr_GetString(0);
	char* it = str;
	for(;*it;++it)
		*it = toupper(*it);
	Scr_AddString(str);
}

void Scr_trim(int a1) {
	#if 0
	char* ws = "\n\r\t\f";
	std::string str = Scr_GetString(0);
	std::string r = "";
	bool f;
	for(std::string::iterator it = str.begin(); it != str.end(); ++it) {
		f=0;
		for(char* i = ws; *i; ++i) f=1; break;
		if(!f)
			r.append((char*)*it);
	}
	Scr_AddString((char*)r.c_str());
	#endif
}

/*
int __cdecl sub_80A880C(unsigned int a1, int a2) //getvector
{
  int v2; // eax@2
  int v3; // ebx@2
  int result; // eax@3
  int v5; // edx@3
  int v6; // eax@5
  int v7; // ebx@5
  char v8; // ST57_1@5
  int v9; // eax@5
  char v10; // ST30_1@5
  int v11; // eax@7

  if ( a1 >= dword_82F5944 ) //stack argc
  {
    v11 = (int)sub_80823CC("parameter %d does not exist", a1 + 1);
    sub_80AA158(v11);
  }
  v3 = dword_82F5948 - 8 * a1;
  varType = *(_DWORD *)(v3 + 4);
  *(float *)a2 = **(float **)v3;
  v5 = dword_82F5948 - 8 * a1;
  *(float *)(a2 + 4) = *(float *)(*(_DWORD *)v5 + 4);
  result = *(_DWORD *)v5;
  *(float *)(a2 + 8) = *(float *)(*(_DWORD *)v5 + 8);
  return result;
}
*/

void dumpmem(int* base, int len, char* outputfile) {
	unsigned int result;
	float rf;
	char* rstr;
	
	FILE* f = fopen(outputfile, "wb");
	if(f) {
		fprintf(f, "DUMP\n");
		for(unsigned int i = 0; i < len; i++) {
			result = *(unsigned int*)(base + i);
			rf = *(float*)(base + i);
			rstr = (char*)(base + i);
			fprintf(f, "%d: Hex: %x, Decimal: %d, Float: %f, String: %s\n", i, result, result, rf, rstr);
		}
		fclose(f);
	}
	
	printf("Successfully dumped base\n");
}

void Scr_convertToIString(int a1) {
	char* str = Scr_GetString(0);
	Scr_AddIString(str);
}

void Scr_ucfirst(int a1) {
	char* str = Scr_GetString(0);
	char* it = str;
	for(;*it;++it)
		*it = tolower(*it);
	*str = toupper(*str);
	Scr_AddString(str);
}
