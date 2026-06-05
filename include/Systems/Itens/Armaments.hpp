#pragma once
#include "include/Stats/Stats_point.hpp"
#include <string>
#include <vector>
#include <iostream>

struct ArmamentStat{
    stats VIG;
    stats DEF;
    stats VEL;
    stats FOR;
    stats INT;
};

template <typename T>

class ArmamentBase{
    protected: 
        struct infoArmament{
            T               id;
            std::string     name;
            ArmamentStat    stats;
        };

        std::vector <infoArmament> ArmamentsTable;

        infoArmament CurrentArmament;
    public:
        ArmamentBase () {
            CurrentArmament = { static_cast <T> (0), "Nenhum", {0, 0, 0, 0, 0}};
        }

        bool equip(T id){
            long unsigned int index = static_cast <int> (id);

            if(index < 0 || index >= ArmamentsTable.size()) {
                std::cout << "Equipamento invalido!\n";
                return false;
            }
            else {
                CurrentArmament = ArmamentsTable[index];
                std::cout << "Novo item equipado: " << CurrentArmament.name << "\n";
                return true;
            }
        }


};

/* Enum class dos equipamentos das classes*/

enum class warrior_ArmamentID{
    NONE                = 0,
    /* Espadas */
    GRASS_BLADE         = 1,
    /* Machados */
    WOOD_AXE            = 2,
    /*Martelos*/
    SPEAR               = 3
};  


enum class hunter_ArmamentID{
    NONE                = 0,
    /*  Arcos*/
    WOOD_BOW            = 1,
    /*Facas*/
    APRENTICE_KNIVES    = 2,
    /*  Armas de fogo*/
    OLD_GUN             = 3
};

enum class mage_ArmamentID{
    NONE                = 0,

    WOOD_STAFF          = 1,

    mage_TOME         = 2,

    WIND_ORB            = 3
};

enum class cleric_ArmamentID{
    NONE                = 0,

    MACE                = 1,

    WAR_HAMMER          = 2,

    FLAIL               = 3
};
    

class warriorArmament : public ArmamentBase<warrior_ArmamentID>{
    public:

        warriorArmament(){  
            ArmamentsTable = {
            /*       ID                                      NOME                 VIG  DEF VEL FOR INT*/
                {warrior_ArmamentID::NONE,                  "NADA",                 {0,  0,  0,  0,  0}},
                {warrior_ArmamentID::GRASS_BLADE,           "Lamina de Grama",      {0,  0,  1,  2,  0}},
                {warrior_ArmamentID::WOOD_AXE,              "Machado de Madeira",   {0,  2,  0,  2,  0}},
                {warrior_ArmamentID::SPEAR,                 "Lança",                {0,  1,  2,  4,  0}}
            };
            CurrentArmament = ArmamentsTable[0];
        }
};

/*Armas do Arqueiro*/


class hunterArmament: public ArmamentBase<hunter_ArmamentID>{
    public:
        hunterArmament(){
            ArmamentsTable = {
                {hunter_ArmamentID::NONE,                "NADA",                  {0,  0,  0,  0,  0}},
                {hunter_ArmamentID::WOOD_BOW,            "Arco de Madeira",       {0,  0,  2,  2,  0}},
                {hunter_ArmamentID::APRENTICE_KNIVES,    "Facas do Aprendiz",     {0,  0,  5,  1,  0}},
                {hunter_ArmamentID::OLD_GUN,             "Arma Velha",            {0,  0,  3,  3,  0}}
            };
            CurrentArmament = ArmamentsTable[0];
        }

        
};

/*Armas do Mago*/
class mageArmament : public ArmamentBase<mage_ArmamentID>{

    public:
        mageArmament(){
            ArmamentsTable = {
                /*          id                       name                  vig def vel for int*/
                {mage_ArmamentID::NONE,           "NADA",                 {0,  0,  0,  0,  0}},
                {mage_ArmamentID::WOOD_STAFF,     "Cajado de Madeira",    {0,  1,  0,  0,  2}},
                {mage_ArmamentID::mage_TOME,      "Tomo do Bruxo",        {1,  0,  0,  0,  4}},
                {mage_ArmamentID::WIND_ORB,       "Orb dos ventos",       {0,  2,  1,  0,  5}}
            };
            CurrentArmament = ArmamentsTable[0];
        }
        
};

/*Armas do Clérico*/

class clericArmament : public ArmamentBase <cleric_ArmamentID>{
    public:
        clericArmament(){
            ArmamentsTable = {
                /*          id                       name                   vig def vel for int*/
                {cleric_ArmamentID::NONE,           "NADA",                  {0,  0,  0,  0,  0}},
                {cleric_ArmamentID::MACE,           "Maça",                  {2,  4,  0,  0,  0}},
                {cleric_ArmamentID::WAR_HAMMER,     "Martelo de Guerra",     {2,  3,  0,  2,  0}},
                {cleric_ArmamentID::FLAIL,          "Mangual",               {0,  2,  1,  2,  0}}
            };
            CurrentArmament = ArmamentsTable[0];
        }
};