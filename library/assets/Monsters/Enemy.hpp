#pragma once

#include "library/Stats/level.hpp"
#include "library/Stats/Critical.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdint>

using std::cout;
using std::string;
using std::uint16_t;

enum class typeEnemies{
    LOBO_SELVAGEM   = 0,
    LOBO_ALFA       = 1,
    JAVALI_SELVAGEM = 2,
    LADRAO          = 3,
    LADRAO_DE_ELITE = 4,
    CAPANGA         = 5,
    MAFIOSO         = 6,
    MAFIOSO_CHEFE   = 7
};

struct dataEnemies{
    typeEnemies id;
    string      nome[20];
    HP          MaxHP;
    HP          Damage;
    uint16_t    Speed;
    HP          Shield;
    Exp         earnExp;
    Money       earnCoin;

};

class Enemies : protected HP, protected Exp{
    private:
        typeEnemies dataEnemies = {typeEnemies::LOBO_SELVAGEM, "Wild Wolf",  50,   12,    5,   5,    10,   5};

    public:
        std::vector<dataEnemies> enemyTable{ // vector é eficas para adicionar futuros mobs de forma facil e rapida e eficiente
            /*       ID             |       nome        | vida | dano | vel | def | xp | coin*/
        {typeEnemies::LOBO_SELVAGEM,   "Wild Wolf",         50,   12,    5,   5,    10,   5},
        {typeEnemies::LOBO_SELVAGEM,   "Wild Wolf",         50,   12,    5,   5,    14,   5},
        {typeEnemies::LOBO_ALFA,       "Alpha Wolf",        80,   22,   25,  12,    30,  10},
        {typeEnemies::JAVALI_SELVAGEM, "Wild Boar",         75,   19,   27,  18,    37,  12},
        {typeEnemies::JAVALI_SELVAGEM, "Wild Boar",         75,   19,   27,  18,    37,  12},
        {typeEnemies::JAVALI_SELVAGEM, "Wild Boar",         75,   19,   27,  18,    37,  12},
        {typeEnemies::LADRAO,          "Thief",            100,   32,   46,  29,    50,  30},
        {typeEnemies::LADRAO,          "Thief",            100,   32,   46,  29,    50,  30},
        {typeEnemies::LADRAO_DE_ELITE, "Elite Thief",       130,   51,   67,  18,   130,  70},
        {typeEnemies::CAPANGA,         "Henchman",         130,   38,   37,  30,    89,  80},
        {typeEnemies::CAPANGA,         "Henchman",         130,   38,   37,  30,    89,  80},
        {typeEnemies::CAPANGA,         "Henchman",         130,   38,   37,  30,    89,  80},
        {typeEnemies::CAPANGA,         "Henchman",         130,   38,   37,  30,    89,  80},
        {typeEnemies::MAFIOSO,         "Mobster",          180,   90,   70,  41,   280, 100},
        {typeEnemies::MAFIOSO,         "Mobster",          180,   90,   70,  41,   280, 100},
        {typeEnemies::MAFIOSO,         "Mobster",          180,   90,   70,  41,   280, 100},
        {typeEnemies::MAFIOSO_CHEFE    "Mob Boss",         230,  100,   63,  62,   450, 167}

        };

        void isAlive(dataEnemies dE){
            if(dE.MaxHP <= 0){
                cout << "  You won!\n";

                levelUp(Money.earnExp);

                cout << "  +" << dE.earnExp << " experience\n";
            }
        }
        void coinEnemy(dataEnemies dE){
            CurrentCoin += Money.earnCoin;
        }
};