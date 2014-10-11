#include "script.hpp"

void GScr_VectorScale(int a1) {
    vec3_t vec;
    Scr_GetVector(0, vec);
    int scale = Scr_GetInt(1);
    vec[0] *= scale;
    vec[1] *= scale;
    vec[2] *= scale;
    Scr_AddVector(vec);
}
