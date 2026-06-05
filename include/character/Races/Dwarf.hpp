#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "include/Stats/Stats_point.hpp"

enum class id_subDwarf{
    FORGE_DWARF = 0,
    COULINS_DWARF = 1,
    UNDERGROUND_DWARF = 2
};

class Dwarf {
    private:
    struct data_subdwarf{
        id_subDwarf  id;
        std::string nome;
        int bonus_vig;
        int bonus_def;
        int bonus_vel;
        int bonus_for;
        int bonus_int;
    };
    public:

    data_subdwarf Current_Race = {id_subDwarf::FORGE_DWARF,  "Anão Ferreiro",  5,  4,  0,  2, 1};

    std::vector <data_subdwarf> DwarfTable{
        {id_subDwarf::FORGE_DWARF,       "Anão Ferreiro",        5,  4,  0,  2,  1},
        {id_subDwarf::COULINS_DWARF,     "Anão das Colinas",     3,  4,  0,  4,  1},
        {id_subDwarf::UNDERGROUND_DWARF, "Anão do Subsolo",      2,  4,  1,  2,  3}

    };
    bool beRace(id_subDwarf sD){
        long unsigned int Dwarfdex = static_cast <int> (sD);
        if(Dwarfdex < 0 || Dwarfdex >= DwarfTable.size()) return false;

        data_subdwarf select = DwarfTable[Dwarfdex];

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