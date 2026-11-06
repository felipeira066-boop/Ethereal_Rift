#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct EnemyData {
    std::string nome;
    int hp;
    int hpmax;
    uint32_t damage;
    uint32_t shield;
    uint32_t expRecompensa;
    uint32_t coinRecompensa;
    int16_t speed;
    int isAlive() const { return hp > 0; }
    int ID;
};

inline std::vector<EnemyData> arenaInimigos = {
    { "Goblin Warrior",              34,   34,    8,    3,   40,   10,   3,  1},
    { "Goblin Mage",                 27,   27,   11,    0,   40,   10,   4,  2},
    { "Goblin Archer",               29,   29,    9,    0,   40,   10,   7,  3},
    { "Elite Goblin Warrior",        50,   50,   12,    5,   60,   18,   4,  4},
    { "Elite Goblin Mage",           42,   42,   15,    1,   60,   18,   5,  5},
    { "Elite Goblin Archer",         45,   45,   13,    1,   60,   18,   9,  6},
    { "Wild Wolf",                   56,   56,   15,    2,   72,   25,   7,  7},
    { "Wild Wolf",                   60,   60,   16,    2,   78,   28,   8,  8},
    { "Alpha Wolf",                  76,   76,   18,    3,   95,   38,   9,  9},
    { "Wild Boar",                   84,   84,   18,    4,  105,   45,   7, 10},
    { "Wild Boar",                   90,   90,   19,    4,  112,   48,   8, 11},
    { "Brown Bear",                 102,  102,   21,    5,  125,   55,   7, 12},
    { "Brown Bear",                 110,  110,   22,    5,  132,   60,   8, 13},
    { "Ogre",                       118,  118,   23,    5,  145,   65,   6, 14},
    { "Ogre",                       126,  126,   24,    6,  155,   70,   7, 15},
    { "Ogre Boss",                  145,  145,   27,    7,  185,   90,   8, 16},
    { "Mountain Dragon",            170,  170,   31,    8,  230,  120,   9, 17}
};
