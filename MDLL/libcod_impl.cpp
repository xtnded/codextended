#define STACK_UNDEFINED 0
#define STACK_STRING 1
#define STACK_LOCALIZED_STRING 2
#define STACK_VECTOR 3
#define STACK_FLOAT 4
#define STACK_INT 5
#define STACK_CODEPOS 6
#define STACK_OBJECT 7
#define STACK_KEY_VALUE 8
#define STACK_FUNCTION 9
#define STACK_STACK 10
#define STACK_ANIMATION 11
#define STACK_THREAD 12
#define STACK_ENTITY 13
#define STACK_STRUCT 14
#define STACK_ARRAY 15
#define STACK_DEAD_THREAD 16
#define STACK_DEAD_ENTITY 17
#define STACK_DEAD_OBJECT 18

#include "libcod.hpp"

typedef struct
{
	void *offsetData;
	int type;
} aStackElement;

int getStack() {
	return 0xAD3430;
}

int stackGetParamInt(int param, int *value) {
	aStackElement *scriptStack = *(aStackElement**)getStack();
	aStackElement *arg = scriptStack - param;
	if (arg->type != STACK_INT)
		return 0;
	*value = (int)arg->offsetData;
	return 1;
}

int stackGetParamVector(int param, float value[3]) {
	aStackElement *scriptStack = *(aStackElement**)getStack();
	aStackElement *arg = scriptStack - param;
	if (arg->type != STACK_VECTOR)
		return 0;
	value[0] = *(float *)((int)(arg->offsetData) + 0);
	value[1] = *(float *)((int)(arg->offsetData) + 4);
	value[2] = *(float *)((int)(arg->offsetData) + 8);
	return 1;
}

int stackGetParamString(int param, char **value) {
	aStackElement *scriptStack = *(aStackElement**)getStack();
	aStackElement *arg = scriptStack - param;
	if (arg->type != STACK_STRING)
		return 0;
    *value = (char *)(*(int *)0x093F098 + 8*(int)arg->offsetData + 4);

	return 1;
}

int stackPushVector(float *ret) {

	int (*signature)(float *);
    *((int *)(&signature)) = 0x04832E0;
	return signature(ret);
}

int stackPushString(char *toPush) {
	int (*signature)(char *);
		*((int *)(&signature)) = 0x0483210;
	return signature(toPush);
}

int stackPushString2(char *toPush) {
	int (*signature)(char *);
		*((int *)(&signature)) = 0x0483260;
	return signature(toPush);
}

int stackPushBool(BOOL b) {
	int (*signature)(BOOL);
		*((int *)(&signature)) = 0x0483070;
	return signature(b);
}

int stackPushInt(int a) {
	int (*signature)(int);
		*((int *)(&signature)) = 0x0483090;
	return signature(a);
}
