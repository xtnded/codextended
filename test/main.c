#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/duktape/duktape.h"

typedef struct {
    float x, y;
} Point;

/*
 * This is the point destructor
 */
duk_ret_t js_Point_dtor(duk_context *ctx)
{
    // The object to delete is passed as first argument instead
    duk_get_prop_string(ctx, 0, "\xff""\xff""deleted");

    bool deleted = duk_to_boolean(ctx, -1);
    duk_pop(ctx);

    if (!deleted) {
        duk_get_prop_string(ctx, 0, "\xff""\xff""data");
		
		//delete static_cast<Point *>(duk_to_pointer(ctx, -1)); //for cpp
        free((Point*)duk_to_pointer(ctx, -1));
		duk_pop(ctx);

        // Mark as deleted
        duk_push_boolean(ctx, true);
        duk_put_prop_string(ctx, 0, "\xff""\xff""deleted");
    }

    return 0;
}

/*
 * This is Point function, constructor. Note that it can be called
 * as a standard function call, you may need to check for
 * duk_is_constructor_call to be sure that it is constructed
 * as a "new" statement.
 */
duk_ret_t js_Point_ctor(duk_context *ctx)
{
    // Get arguments
    float x = duk_require_number(ctx, 0);
    float y = duk_require_number(ctx, 1);

    // Push special this binding to the function being constructed
    duk_push_this(ctx);

    // Store the underlying object
	
	Point *p = (Point*)malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	
    duk_push_pointer(ctx, p);
    duk_put_prop_string(ctx, -2, "\xff""\xff""data");

    // Store a boolean flag to mark the object as deleted because the destructor may be called several times
    duk_push_boolean(ctx, false);
    duk_put_prop_string(ctx, -2, "\xff""\xff""deleted");

    // Store the function destructor
    duk_push_c_function(ctx, js_Point_dtor, 1);
    duk_set_finalizer(ctx, -2);

    return 0;
}

/*
 * Basic toString method
 */
duk_ret_t js_Point_toString(duk_context *ctx)
{
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    Point *point = (Point*)(duk_to_pointer(ctx, -1));
    duk_pop(ctx);
    duk_push_sprintf(ctx, "%f, %f", point->x, point->y);

    return 1;
}

// methods, add more here
const duk_function_list_entry methods[] = {
    { "toString",   js_Point_toString,  0   },
    { NULL, NULL,        0   }
};

duk_ret_t js_playerobj_getplayerangles(duk_context *ctx) {
	duk_push_this(ctx);
	duk_pop(ctx);
	duk_push_string(ctx, "ayy lmao");
	return 1;
}

const duk_function_list_entry player_object_methods[] = {
	{"getplayerangles", js_playerobj_getplayerangles, 0},
	{NULL, NULL, 0}
};

const duk_number_list_entry player_object_fields[] = {
	{"name", 0},
	{NULL, 0}	
};

void *add_player(duk_context *ctx, duk_idx_t playerArrayIdx, int id) {
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, player_object_methods);
	duk_put_prop_index(ctx, playerArrayIdx, 0);
	duk_pop(ctx);
	return NULL;
}

typedef struct {
	const char *name;
} client_t;

#define get_client_number(x) (1)

void js_push_player_object(duk_context *ctx, client_t *cl) {
	int clientNum = get_client_number(cl);
	duk_push_object(ctx); //player object
	duk_push_int(ctx, clientNum);
	duk_put_prop_string(ctx, -2, "\xff""num");
	
	duk_put_function_list(ctx, -1, player_object_methods);
	
	/* define fields beforehand ? */
	const duk_number_list_entry *ent = player_object_fields;
	int fieldN = 0;
	char propName[256] = {0};
	
	duk_idx_t obj_index = duk_require_normalize_index(ctx, -1);
	if (ent != NULL) {
		while (ent->key != NULL) {
			duk_push_number(ctx, fieldN++);
			sprintf(propName, "FIELD_%s", ent->key);
			for(char *c = propName; *c;)
				*c++ = toupper(*c);
			duk_put_prop_string(ctx, obj_index, propName);
			
			duk_push_number(ctx, ent->value);
			duk_put_prop_string(ctx, obj_index, ent->key);
			ent++;
		}
	}
	/* end of definement */
	
	duk_push_string(ctx, cl->name);
	duk_put_prop_string(ctx, -2, "name");
}

void update_player_js_data(duk_context *ctx) {
	duk_push_global_object(ctx);
	
	int clientNum;
	
	duk_get_prop_string(ctx, -1, "players");
	for(int i = 0; i < duk_get_length(ctx, -1); i++) {
		duk_get_prop_index(ctx, -1, i);
			duk_get_prop_string(ctx, -1, "name");
			char *str = duk_get_string(ctx, -1);
			printf("oldname = %s\n", str);
			duk_pop(ctx);
			
			duk_push_string(ctx, "hello");
			duk_put_prop_string(ctx, -2, "name");
			
		duk_pop(ctx);
	}
	duk_pop(ctx); //players
	duk_pop(ctx); //global
	
	#if 0
	for(int i = 0; i < players_size; i++) {
		printf("updated for player %d", i);
		duk_get_prop_index(ctx, -1, i);
		
			duk_push_string(ctx, "LOL man");
			duk_put_prop_string(ctx, -2, "name");
		
		duk_pop(ctx); //player
		
	}
	duk_pop(ctx); //length
	
	duk_pop(ctx);
	#endif
}

duk_fatal_function fatal_handler(duk_context *ctx, duk_errcode_t code, const char *msg) {
	printf("FATAL ERROR: code: %d, msg: %s\n", code, msg);
}

int main(void) {
    //duk_context *ctx = duk_create_heap_default();
	duk_context *ctx = duk_create_heap(NULL,NULL,NULL,NULL,(duk_fatal_function)fatal_handler);
	/* add entities */
	#if 1
	//duk_push_global_object(ctx);
	//duk_idx_t arrIdx = duk_push_array(ctx);
	//for(int i = 0; i < 64; i++)
		//add_player(ctx, arrIdx, i);
	duk_idx_t arr_idx = duk_push_array(ctx);

		client_t cl = {"test"};
			
		for(int i = 0; i < 64; i++) {
			js_push_player_object(ctx, &cl);
			duk_put_prop_index(ctx, arr_idx, i);
		}
	duk_put_global_string(ctx, "players");
	
	
	update_player_js_data(ctx);
	
	//duk_pop(ctx);
	#endif
	/* end */
	
    // Create Point function
    duk_push_c_function(ctx, js_Point_ctor, 2);

    // Create a prototype with toString and all other functions
    duk_push_object(ctx);
    duk_put_function_list(ctx, -1, methods);
    duk_put_prop_string(ctx, -2, "prototype");

    // Now store the Point function as a global
    duk_put_global_string(ctx, "Point");
/*
    if (duk_peval_string(ctx, "p = new Point(20, 40); print(p)") != 0) {
        printf("error: %s\n", duk_to_string(ctx, -1));
		exit(-1);
    }
*/

	duk_eval_file(ctx, "main.js");

    return 0;
}