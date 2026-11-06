#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

#include "../../../Systems/Economy/inventory/Bank.hpp"

struct Lootentry{
    std::string name;
    int         weight;
    int         quant;
};

std::vector<Lootentry> LootComunTable = {
    {"Nothing",             60,     0},
    {"Apple",               60,     1},
    {"Melon Pie",           60,     1},
    {"Assassin Herb",       60,     1},
    {"Starfruit",           60,     1}
};

Lootentry playLoot(std::vector<Lootentry> &table){
    int totalweight = 0;
    for(const auto &entry : table){
        totalweight += entry.weight;
    }

    srand(time(NULL));

    int genWeight = rand()%totalweight;

    int acumulated = 0;
    for(const auto &entry : table){
        acumulated += entry.weight;
        if (genWeight < acumulated)
            return entry;
    }

    return table[0];
}

enum class Chestrarity {
    COMUN           = 0,
    INCOMUN         = 1,
    RARE            = 2,
    EPIC            = 3
};

struct ChestData{
    Chestrarity     rarity;
    std::string     name;
    int             spawn;
    int             coinMin;
    int             coinMax;
    std::vector<Lootentry> lootTable;

};

std::vector <ChestData> ChestTypes = {
    {
        Chestrarity::COMUN,  "Common Chest",  60,  5,  10,
        {
            {"Nothing",                         50,  0},
            {"Apple",                           25,  1},
            {"Melon Pie",                       25,  1},
            {"Assassin Herbs",                  25,  1},
            {"Starfruit",                       25,  1}
        }
    },

    {
        Chestrarity::INCOMUN,  "Uncommon Chest", 20,  15,  25,
        {
            {"Nothing",                         30,  0},
            {"Apple",                           40,  2},
            {"Melon Pie",                       40,  1},
            {"Assassin Herbs",                  40,  2},
            {"Starfruit",                       40,  1}
        }
    },

    {
        Chestrarity::RARE,  "Rare Chest",  6,  35,  50,
        {
            {"Nothing",                         10,  0},
            {"Apple",                           50,  2},
            {"Melon Pie",                       40,  3},
            {"Assassin Herbs",                  50,  2},
            {"Starfruit",                       50,  3}
        }
    },

    {
        Chestrarity::EPIC,  "Epic Chest",  2,  75,  100,
        {
            {"Nothing",                         0,  0},
            {"Apple",                           70,  4},
            {"Melon Pie",                       50,  5},
            {"Assassin Herbs",                  60,  5},
            {"Starfruit",                       70,  5}
        }
    }
};

ChestData &choseChest(){
    int totalpeso;
    for (const auto &chest : ChestTypes){
        totalpeso += chest.spawn;
    }
    int rolagem = rand()% totalpeso;

    int acumulated;

    for( auto &chest : ChestTypes){
        acumulated += chest.spawn;
        if(rolagem < acumulated) return chest;
    }
    return ChestTypes[0];
}


void openChest(){
    ChestData &bau = choseChest();

    std::cout << "\n  You found a " << bau.name << "!\n";

    /* 2. coin — valor aleatório dentro do range */
    int coin = bau.coinMin + rand() % (bau.coinMax - bau.coinMin + 1);
    CurrentCoin += coin;
    std::cout << "  + " << coin << " coins\n";

    /* 3. loot — rolagem ponderada */
    Lootentry recompensa = playLoot(bau.lootTable);

    if (recompensa.quant > 0) {
        std::cout << "  + " << recompensa.quant
                  << "x " << recompensa.name << "\n";
        /* adicionar ao inventário aqui */
    } else {
        std::cout << "  The chest was empty...\n";
    }
}
