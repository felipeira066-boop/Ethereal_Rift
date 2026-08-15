#pragma once

#include "library/Stats/Stats_point.hpp"
#include "library/Stats/level.hpp"
#include "library/Stats/HP.hpp"

#include <string>

class wolf : public Exp, public HP{

    public:
        wolf(){
            initHP();
            std::string name = "Wolf";
            myStats.Shield = 3;
            myStats.Damage = 10;
            myStats.Speed  = 5;
        }
};