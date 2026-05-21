#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "library/Stats/Stats_point.hpp"

class critical{
    public:
    int gerarCritico(){
        return rand ()%26; 
    }

    void CriticalDamage(finalStats &fs){
        int critical_rate = gerarCritico();

        if (critical_rate >= 25){
            fs.Damage *= 2;
            std::cout << "  > Crítico!\n";
        }
        return;
    }

};