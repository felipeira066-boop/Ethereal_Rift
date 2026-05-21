#pragma once
#include <cstdint>
#include <iostream>
#include "library/Stats/Stats_point.hpp"

class Mana{

    private:
    uint32_t CurrentMana;

    public:
        /* resetMana é redundante — getMana já faz o teto */
        void getMana(int quantidade, uint32_t maxMana) {
            CurrentMana += quantidade;
            if (CurrentMana > maxMana) CurrentMana = maxMana;
            std::cout << "  Mana restaurada! Atual: " << CurrentMana << "\n";
        }
        void initMana(const finalStats &fs) {       
            CurrentMana = fs.MaxMana;
        }

        void useMana(uint32_t quantidade) {
            if (quantidade > CurrentMana) CurrentMana = 0;
            else CurrentMana -= quantidade;
        }

        int32_t getCurrentMana() const { return CurrentMana; }
};