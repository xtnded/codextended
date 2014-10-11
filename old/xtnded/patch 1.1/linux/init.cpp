#include "codextended.hpp"

extern "C" void __attribute__ ((constructor)) lib_load(void) {
    game = new CODEXTENDED;
}

/*
extern "C" void __attribute__ ((destructor)) lib_unload(void) {
    delete game;
}
*/
