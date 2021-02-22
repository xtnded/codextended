#include "js.hpp"
#include "../server/server.hpp"
extern "C" {
duk_context *ctx;

void mf_name(int num, JSMemberfield* field) {
    char userinfo[MAX_STRING_CHARS];
    getuserinfo(num, userinfo, sizeof(userinfo));

    char* name = Info_ValueForKey(userinfo, "name");
    duk_push_string(ctx, name);
}

void mf_num(int num, JSMemberfield* field) {
    duk_push_int(ctx, num);
}

JSMemberfield playerfields[] = {
    {"name", FT_STRING, mf_name},
    {"num", FT_INT, mf_num},
    {"team", FT_STRING, mf_name},
    {"state", FT_STRING, mf_name},
    {"maxhealth", FT_INT, mf_name},
    {"maxspeed", FT_FLOAT, mf_name},
    {"score", FT_INT, mf_name},
    {"deaths", FT_INT, mf_name},
    {NULL, NULL}
};

void js_destroy() {
    duk_destroy_heap(ctx);
}

int js_com_printf(duk_context *ctx) {
    const char* str = duk_require_string(ctx, -1);
    printf(str);
    return 0;
}

void js_addfunction(const char* name, int (*func)(duk_context*), int argc) {
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, func, argc);
    duk_put_prop_string(ctx, -2, name);
    duk_pop(ctx);
}

void js_debug() {
    printf("debug;\n");
}

void js_execute() {
    if(ctx) {
        /*duk_push_global_object(ctx);
        duk_push_c_function(ctx, duk_com_printf, 1);
        duk_put_prop_string(ctx, -2, "Com_Printf");
        duk_pop(ctx);
        */

        duk_push_global_object(ctx);
        duk_get_prop_string(ctx, -1, "consolecommand");
        int argc = Cmd_Argc()-1;
        int arr_idx = duk_push_array(ctx);
        for(int i = 0; i < argc; i++) {
            duk_push_string(ctx, Cmd_Argv(i));
            duk_put_prop_index(ctx, arr_idx, i);
        }
        duk_call(ctx, 1);
        duk_pop(ctx);
    }
}

void js_load() {
    duk_eval_file(ctx, "cod.js");
    duk_pop(ctx);
}

int js_setbyte(duk_context* c) {
    byte* off = (byte*)duk_require_pointer(c, -1);
    byte val = (byte)duk_require_int(c, -2);
    *off = val;
    return 0;
}

int js_getbyte(duk_context* c) {
    byte* off = (byte*)duk_require_pointer(c, -1);
    duk_push_int(c, *off);
    return 1;
}

int js_setint(duk_context* c) {
    int* ptr = (int*)duk_require_pointer(c, -1);
    int off = duk_require_int(c, -2);
    int value = duk_require_int(c, -3);
    ptr+=off;
	*ptr = value;
    return 0;
}

int js_getint(duk_context* c) {
    int* ptr = (int*)duk_require_pointer(c, -1);
	int off = duk_require_int(c,-2);
	ptr+=off;
    duk_push_int(c,*ptr);
    return 1;
}

int js_getliboffset(duk_context* c) {
    const char* str = duk_require_string(ctx, -1);
	void* p = (void*)dlsym(gamelib, str);
	duk_push_pointer(c, p);
    return 1;
}

int js_getentint(duk_context* c) {
	int index = duk_require_number(c,-1);
	int offset = duk_require_number(c,-2);
	ENTITY* ent = game->getEntity(index);
	if(ent) {
		int value;
		ent->get(offset, &value, sizeof(value));
		duk_push_int(c,value);
	} else {
		duk_push_int(c,0);
	}
	return 1;
}

int js_setentint(duk_context* c) {
	int index = duk_require_int(c,-1);
	int offset = duk_require_int(c,-2);
	int value = duk_require_int(c,-3);
	ENTITY* ent = game->getEntity(index);
	if(ent) {
		ent->set(offset, &value, sizeof(value));
	}
	return 0;
}

int js_getplayer(duk_context* c) {
	int index = duk_require_int(c, -1);
	int off = duk_require_int(c, -2);
	const char* str = duk_require_string(c, -3);
	printf("-1 = %d\n", index);
	printf("-2 = %d\n", off);
	printf("-3 = %s\n", str);
	ENTITY* ent = game->getEntity(index);
	if(ent) {
		int v;
		ent->get(off, &v, sizeof(v));
		duk_push_int(c,v);
	} else {
	duk_push_int(c,-1);
	}
	/*if(!strcmp(str, "int")) {
		printf("off = %d\n", *(int*)off);
		duk_push_int(ctx, *(int*)off);
	} else if(!strcmp(str, "float")) {
		duk_push_number(ctx, *(float*)off);
	} else if(!strcmp(str, "string")) {
		duk_push_string(ctx, (const char*)off);
	} else if(!strcmp(str, "byte")) {
		duk_push_int(ctx, *(byte*)off);
	}*/
	return 1;
}

int js_setplayer(duk_context* c) {
	int player = duk_require_int(c, -1);
	int duk_off = duk_require_int(c, -2);
	int off = (gentities + GENTITY_SIZE * player) + duk_off;
	const char* str = duk_require_string(c, -3);
	if(!strcmp(str, "int")) {
		*(int*)off = duk_require_int(c,-4);
	} else if(!strcmp(str, "float")) {
		*(float*)off = duk_require_number(c,-4);
	} else if(!strcmp(str, "byte")) {
		*(byte*)off=duk_require_int(c,-4);
	}
	return 0;
}

void js_init() {
    ctx = duk_create_heap_default();
    Cmd_AddCommand("js", js_execute);
    Cmd_AddCommand("js_reload", js_load);
    Cmd_AddCommand("js_debug", js_debug);
    js_addfunction("Com_Printf", js_com_printf, 1);
    js_addfunction("print", js_com_printf, 1);
	js_addfunction("setbyte", js_setbyte, 2);
	js_addfunction("getbyte", js_getbyte, 1);
	js_addfunction("setint", js_setint, 3);
	js_addfunction("getint", js_getint, 2);
	js_addfunction("get_lib_offset", js_getliboffset, 1);
	js_addfunction("setentint", js_setentint, 2);
	js_addfunction("getentint", js_getentint, 3);
    js_load();
}
}
