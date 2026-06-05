#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "include/Stats/Stats_point.hpp"

enum class id_subHuman{
    JUNGLE_HUMAN = 0,
    MOUNTAIN_HUMAN =1
};

class Human {
    private:
    
    struct data_subHuman{
        id_subHuman  id;
        std::string nome;
        int bonus_vig;
        int bonus_def;
        int bonus_vel;
        int bonus_for;
        int bonus_int;
    };

    public:

    data_subHuman Current_Race = {id_subHuman::JUNGLE_HUMAN,  "Humano da Selva",  2,  2,  2,  3,  2};

    std::vector <data_subHuman> HumanTable{
        {id_subHuman::JUNGLE_HUMAN,    "Humano da Selva",       2,  2,  2,  3,  2},
        {id_subHuman::MOUNTAIN_HUMAN,  "Humano das Montanhas",  3,  2,  2,  2,  2}
    };

    bool beRace(id_subHuman sH){
        long unsigned int Humandex = static_cast <int> (sH);
        if(Humandex < 0 || Humandex >= HumanTable.size()) return false;

        data_subHuman select = HumanTable[Humandex];

        Current_Race = select;
        std::cout << "  Raça definida: " << Current_Race.nome << std::endl;
        return true;
    }

    raceStats getRaceStats() const {
        return raceStats{
            Current_Race.bonus_vig,
            Current_Race.bonus_def,
            Current_Race.bonus_vel,
            Current_Race.bonus_for,
            Current_Race.bonus_int
        };
    }
};

