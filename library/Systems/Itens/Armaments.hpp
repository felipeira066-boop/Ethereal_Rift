#pragma once
#include "../../Stats/Stats_point.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cstddef>

struct ArmamentStat{
    int VIG;
    int DEF;
    int VEL;
    int FOR;
    int INT;
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
            CurrentArmament = { static_cast <T> (0), "None", {0, 0, 0, 0, 0}};
        }

        bool equip(T id){
                const int itemIndex = static_cast<int>(id);

                if(itemIndex < 0 ||
                   static_cast<std::size_t>(itemIndex) >= ArmamentsTable.size()) {
                    std::cout << "Invalid equipment!\n";
                    return false;
                }
                else {
                    CurrentArmament = ArmamentsTable[static_cast<std::size_t>(itemIndex)];
                    std::cout << "New item equipped: " << CurrentArmament.name << "\n";
                    return true;
                }
            }

            bool getItemData(T id, std::string& name, stats& itemStats) const {
                const int itemIndex = static_cast<int>(id);
                if (itemIndex <= 0 ||
                    static_cast<std::size_t>(itemIndex) >= ArmamentsTable.size()) {
                    return false;
                }

                const infoArmament& item =
                    ArmamentsTable[static_cast<std::size_t>(itemIndex)];
                name = item.name;
                itemStats.VIG = item.stats.VIG;
                itemStats.DEF = item.stats.DEF;
                itemStats.VEL = item.stats.VEL;
                itemStats.FOR = item.stats.FOR;
                itemStats.INT = item.stats.INT;
                return true;
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
                {warrior_ArmamentID::NONE,                  "None",                 {0,  0,  0,  0,  0}},
                {warrior_ArmamentID::GRASS_BLADE,           "Grass Blade",          {0,  0,  1,  2,  0}},
                {warrior_ArmamentID::WOOD_AXE,              "Wooden Axe",            {0,  2,  0,  2,  0}},
                {warrior_ArmamentID::SPEAR,                 "Spear",                {0,  1,  2,  4,  0}}
            };
            CurrentArmament = ArmamentsTable[0];
        }
};

/*Armas do Arqueiro*/


class hunterArmament: public ArmamentBase<hunter_ArmamentID>{
    public:
        hunterArmament(){
            ArmamentsTable = {
                {hunter_ArmamentID::NONE,                "None",                  {0,  0,  0,  0,  0}},
                {hunter_ArmamentID::WOOD_BOW,            "Wooden Bow",            {0,  0,  2,  2,  0}},
                {hunter_ArmamentID::APRENTICE_KNIVES,    "Apprentice's Knives",   {0,  0,  5,  1,  0}},
                {hunter_ArmamentID::OLD_GUN,             "Old Gun",               {0,  0,  3,  3,  0}}
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
                {mage_ArmamentID::NONE,           "None",                 {0,  0,  0,  0,  0}},
                {mage_ArmamentID::WOOD_STAFF,     "Wooden Staff",         {0,  1,  0,  0,  2}},
                {mage_ArmamentID::mage_TOME,      "Warlock's Tome",       {1,  0,  0,  0,  4}},
                {mage_ArmamentID::WIND_ORB,       "Wind Orb",             {0,  2,  1,  0,  5}}
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
                {cleric_ArmamentID::NONE,           "None",                 {0,  0,  0,  0,  0}},
                {cleric_ArmamentID::MACE,           "Mace",                 {2,  4,  0,  0,  0}},
                {cleric_ArmamentID::WAR_HAMMER,     "War Hammer",           {2,  3,  0,  2,  0}},
                {cleric_ArmamentID::FLAIL,          "Flail",                {0,  2,  1,  2,  0}}
            };
            CurrentArmament = ArmamentsTable[0];
        }
};