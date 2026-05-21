#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "library/Stats/Stats_point.hpp"

enum class id_subElf{
    BOOD_ELF = 0,
    MOUNTAIN_ELF =1,
    SNOW_ELF  =2
};

class Elf {
    private:
    struct data_subElf{
        id_subElf  id;
        std::string nome;
        int bonus_vig;
        int bonus_def;
        int bonus_vel;
        int bonus_for;
        int bonus_int;
    };
    public:

    data_subElf Current_Race= {id_subElf::BOOD_ELF,  "Elfo de Sangue",  3,  1,  2,  2, 3};

    std::vector <data_subElf> ElfTable{
        {id_subElf::BOOD_ELF,      "Elfo de Sangue",      3,  1,  2,  2,  3},
        {id_subElf::MOUNTAIN_ELF,  "Elfo das Montanhas",  2,  1,  3,  4,  2},
        {id_subElf::SNOW_ELF,      "Elfo das neves",      3,  2,  4,  1,  2}
    };
    bool beRace(id_subElf sE){
        long unsigned int Elfdex = static_cast <int> (sE);
        if(Elfdex < 0 || Elfdex >= ElfTable.size()) return false;

        data_subElf select = ElfTable[Elfdex];

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

