#pragma once

#include "include/Stats/Stats_point.hpp"
#include <string>
#include <vector>
#include <iostream>

struct ArmorStat{
    stats VIG, DEF, VEL, FOR, INT;
};

template<typename T>

class ArmorBase{
protected:
    struct infoArmor{
        T           id;
        std::string name;
        ArmorStat   stats;
    };

    std::vector <infoArmor> ArmorTable;

    infoArmor CurrentArmor;

public:
    ArmorBase(){
        CurrentArmor = { static_cast <T> (0), "Nenhum", {0,  0,  0,  0,  0}};
    }

    bool equip(T id){
        long unsigned int index = static_cast <int> (id);

        if(index < 0 || index >= ArmorTable.size()){
            std::cout << "Equipamento inválido!\n";
            return false;
        }

        else{
            CurrentArmor = ArmorTable[index];
            std::cout << "Novo item equipado: " << CurrentArmor.name << std::endl;
            return true;
        }
    }

};


/*Armaduras do Guerreiro*/


enum class Warrior_armorID{
    NONE            = 0,
    SILVER_GARMENTS = 1,
    UNIFORM_VIKING  = 2,
    BEAR_SKINS      = 3
};

enum class Arrow_armorID{
    NONE            = 0,
    ASSASINS_KIT    = 1,
    HUNTER_COAT     = 2,
    NINJAS_GEDGETS   = 3
};

enum class Wizard_armorID{
    NONE            = 0,
    WIZARD_ROBE     = 1,
    STARS_HAT       = 2,
    GLITTER_TUNIC   = 3
};

enum class Cleric_armorID{
    NONE            = 0,
    CHAIN_MAIL      = 1,
    SCALE_MAIL      = 2,
    STUDDED_LEATHER = 3
};

class Warrior_armor : public ArmorBase<Warrior_armorID>{
public: 
    Warrior_armor(){
        ArmorTable = {
        //      ID                                   NOME                   VIG  DEF VEL FOR INT
            {Warrior_armorID::NONE,               "NADA",                   {0,  0,  0,  0,  0}},
            {Warrior_armorID::SILVER_GARMENTS,    "Vestimentas de Prata",   {2,  5,  0,  1,  0}},
            {Warrior_armorID::UNIFORM_VIKING,     "Uniforme Viking",        {5,  1,  0,  3,  0}},
            {Warrior_armorID::BEAR_SKINS,         "Pele de Urso",           {2,  2,  0,  2,  0}}
        };

        CurrentArmor = ArmorTable[0];

    }
};


/*Armaduras do caçador*/



class Arrow_armor : public ArmorBase<Arrow_armorID>{
public: 
    Arrow_armor(){
        ArmorTable = {
            /*          ID                   nome                  vig def vel for int*/
            {Arrow_armorID::NONE,             "NADA",               {0,  0,  0,  0,  0}},
            {Arrow_armorID::ASSASINS_KIT,     "Kit de Assasinos",   {0,  1,  3,  1,  0}},
            {Arrow_armorID::HUNTER_COAT,      "Casaco do Caçador",  {1,  3,  5,  0,  0}},
            {Arrow_armorID::NINJAS_GEDGETS,   "Tralhas Ninjas",     {0,  1,  2,  1,  0}}
        };

        CurrentArmor = ArmorTable[0];
    }

    
};


/*Armaduras do Mago*/



class Wizard_armor : public ArmorBase<Wizard_armorID>{
public:
    Wizard_armor(){

        ArmorTable = {
            {Wizard_armorID::NONE,             "NADA",                 {0,  0,  0,  0,  0}},
            {Wizard_armorID::WIZARD_ROBE,      "Roupão do Bruxo",      {1,  2,  0,  0,  3}},
            {Wizard_armorID::STARS_HAT,        "Chapéu das Estrelas",  {0,  1,  0,  0,  2}},
            {Wizard_armorID::GLITTER_TUNIC,    "Tunica de Glitter",    {2,  2,  0,  0,  4}}
        };

        CurrentArmor = ArmorTable[0];

    }
};

/*Armaduras do clerigo*/



class Cleric_armor : public ArmorBase<Cleric_armorID>{
public:

    Cleric_armor(){
        ArmorTable = {
             /*      id                             nome            vig def vel for int*/
            {Cleric_armorID::NONE,              "NADA",             {0,  0,  0,  0,  0}},
            {Cleric_armorID::CHAIN_MAIL,        "Cota de Malha",    {2,  0,  0,  0,  1}},
            {Cleric_armorID::SCALE_MAIL,        "Cota de Escamas",  {3,  0,  0,  0,  2}},
            {Cleric_armorID::STUDDED_LEATHER,   "Couro Batido",     {5,  2,  0,  0,  1}}
        };

        CurrentArmor = ArmorTable[0];
    }
};