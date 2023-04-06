#pragma once
#include "ordre.h"

class OrdreDeplacer : public Ordre {
    int _dx;
    int _dy;
public:
    OrdreDeplacer(int dx, int dy) : _dx(dx), _dy(dy) {};
};