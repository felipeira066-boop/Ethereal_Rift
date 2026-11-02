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
    { "Goblin",           25,   25,   6,    0,  40,  10,   4,  1},
    { "Goblin Guerreiro", 28,   28,   10,   1,  44,  12,   5,  2},
    { "Goblin Arqueiro",  35,   35,   16,   0,  44,  12,   5,  3},
    { "Goblin de Elite",  42,   42,   22,   1,  55,  18,   6,  4},
    { "Lobo Selvagem",    54,   54,   24,   2,  60,  25,   7,  5},
    { "Javali Selvagem",  68,   68,   28,   3,  98,  50,  10,  6},
    { "Lobo Alfa",        88,   88,   28,   3,  70,  30,  15,  7},
    { "Urso Pardo",       95,   95,   30,   4, 110,  60,  18,  8},
    { "Ogro",             100,  100,  30,   3,  95,  50,  21,  9},
    { "Ogro",             100,  100,  30,   3,  95,  50,  21, 10},
    { "Ogro Chefe",       124,  124,  33,   4, 150,  80,  25, 11},
    { "Dragao",           152,  152,  40,   5, 200, 150,  30, 12}
};
