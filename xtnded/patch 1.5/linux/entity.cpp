#include "entity.hpp"

int gentities;

int ENTITY::getPlayerState() {
    if(!this->base)
        return 0;
    return *(int*)(base+EOFF_PLAYERSTATE);
}

bool ENTITY::isPlayer() {
    int client = *(int*)(base+EOFF_PLAYERSTATE);
    return client;
}

bool ENTITY::isEntity() {
    return((*(int*)this->base) == this->index);
}

void ENTITY::switchState() {
    if(this->ps)
        this->ptr = this->getPlayerState();
    else
        this->ptr = this->base;
    this->ps = !this->ps;
}

void ENTITY::toPlayerState() {
    this->ptr = this->getPlayerState();
    this->ps = true;
}

void ENTITY::toEntityState() {
    this->ptr = this->base;
    this->ps = false;
}

void ENTITY::set(int offset, void* v, unsigned int len) {
    memcpy((void*)(ptr+offset), v, len);
}

void ENTITY::get(int offset, void* v, unsigned int len) {
    memcpy(v, (void*)(ptr+offset), len);
}

void G_Damage(int* targ, int* inflictor, int* attacker,  vec3_t dir, vec3_t point, int damage, int dflags, int mod, int idk) {

}
