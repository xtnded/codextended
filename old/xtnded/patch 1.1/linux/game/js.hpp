#ifndef JS_H
#define JS_H

#include "shared.hpp"
extern "C" {
#include "../duktape/duktape.h"

void js_init();
void js_destroy();
void js_execute();

typedef struct JSMemberfield {
    char* name;
    int type;
    void (*func)(int,JSMemberfield*);
};

typedef enum {
    FT_INT,
    FT_STRING,
    FT_FLOAT
} memberfield_types;

extern duk_context* ctx;
extern JSMemberfield playerfields[];
}
#endif // JS_H
