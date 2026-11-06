#pragma once
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Stats_point.hpp"

class critical{
    public:
    int gerarCritico(){
        return rand() % 100;
    }

    void CriticalDamage(finalStats &fs, int criticalRateBonus = 0){
        const int criticalChance =
            std::clamp(5 + static_cast<int>(fs.Speed) + criticalRateBonus, 0, 50);

        if (gerarCritico() < criticalChance){
            fs.Damage *= 2;
            std::cout << "  > Critical hit!\n";
        }
        return;
    }

};