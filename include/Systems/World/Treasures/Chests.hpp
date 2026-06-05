#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

#include "include/Systems/Economy/inventory/Bank.hpp"

struct Lootentry{
    std::string name;
    int         weight;
    int         quant;
};

std::vector<Lootentry> LootComunTable = {
    {"nada",                60,     0},
    {"Maçã",                60,     1},
    {"Torta de Melão",      60,     1},
    {"Erva Assasina",       60,     1},
    {"Fruta Estrelar",      60,     1}
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
        Chestrarity::COMUN,  "Baú Comum",  60,  5,  10,
        {
            {"nada",                            50,  0},
            {"Maçã",                            25,  1},
            {"Torta de Melão",                  25,  1},
            {"Ervas Assasinas",                 25,  1},
            {"Frutas Estrelares",               25,  1}
        }
    },

    {
        Chestrarity::INCOMUN,  "Baú Incomum", 20,  15,  25,
        {
            {"nada",                            30,  0},
            {"Maçã",                            40,  2},
            {"Torta de Melão",                  40,  1},
            {"Ervas Assasinas",                 40,  2},
            {"Frutas Estrelares",               40,  1}
        }
    },

    {
        Chestrarity::RARE,  "Baú Incomum",  6,  35,  50,
        {
            {"nada",                            10,  0},
            {"Maçã",                            50,  2},
            {"Torta de Melão",                  40,  3},
            {"Ervas Assasinas",                 50,  2},
            {"Frutas Estrelares",               50,  3}
        }
    },

    {
        Chestrarity::EPIC,  "Baú Épico",  2,  75,  100,
        {
            {"nada",                            0,  0},
            {"Maçã",                            70,  4},
            {"Torta de Melão",                  50,  5},
            {"Ervas Assasinas",                 60,  5},
            {"Frutas Estrelares",               70,  5}
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

    std::cout << "\n  Voce encontrou um " << bau.name << "!\n";

    /* 2. coin — valor aleatório dentro do range */
    int coin = bau.coinMin + rand() % (bau.coinMax - bau.coinMin + 1);
    CurrentCoin += coin;
    std::cout << "  + " << coin << " coin\n";

    /* 3. loot — rolagem ponderada */
    Lootentry recompensa = playLoot(bau.lootTable);

    if (recompensa.quant > 0) {
        std::cout << "  + " << recompensa.quant
                  << "x " << recompensa.name << "\n";
        /* adicionar ao inventário aqui */
    } else {
        std::cout << "  O baú estava vazio...\n";
    }
}
