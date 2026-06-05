#pragma once
#include <cstdint>
#include <iostream>
#include "include/Stats/Stats_point.hpp"

class Mana{

    protected:
        finalStats manaStats;

    public:
        uint32_t CurrentMana;

        /* resetMana é redundante — getMana já faz o teto */
        void getMana(int quantidade, uint32_t maxMana) {
            CurrentMana += quantidade;
            if (CurrentMana > maxMana) CurrentMana = maxMana;
            std::cout << "  Mana restaurada! Atual: " << CurrentMana << "\n";
        }
        void initMana(const finalStats &fs) {       
            manaStats = fs;
            CurrentMana = manaStats.MaxMana;
        }

        void useMana(uint32_t quantidade) {
            if (quantidade > CurrentMana) CurrentMana = 0;
            else CurrentMana -= quantidade;
        }

        uint32_t getMaxMana() const { return manaStats.MaxMana; }
};