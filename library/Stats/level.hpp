#pragma once
#include <cstdint>
#include <iostream>
#include <cmath>

/* ═══════════════════════════════════════════════════
   Sistema de EXP e Level Up
   Fórmula: EXP necessária = 100 * nivel²
═══════════════════════════════════════════════════ */

class Exp {
protected:
    uint16_t currentLevel = 1;
    uint32_t currentExp   = 0;
    uint16_t maxLevel     = 20;

    /* EXP necessária para o próximo nível */
    uint32_t expParaProximoNivel() const {
        double exp = 100.0 * std::pow(1.2, currentLevel);
        return static_cast<uint32_t>(exp);
    }

public:

    void initLevel() {
        currentLevel = 1;
        currentExp   = 0;
    }

    /* adiciona EXP e sobe de nível automaticamente */
    void getLvl(uint32_t quant) {
        if (currentLevel >= maxLevel) return;

        currentExp += quant;
        std::cout << "  + " << quant << " EXP"
                  << " (" << currentExp << "/"
                  << expParaProximoNivel() << ")\n";

        while (currentExp >= expParaProximoNivel() &&
               currentLevel < maxLevel) {

            currentExp   -= expParaProximoNivel();
            currentLevel += 1;

            std::cout << "\n  *** LEVEL UP! Nível atual: "
                      << currentLevel << " ***\n";
        }
    }

    bool podeSubirNivel() const {
        return currentLevel < maxLevel;
    }

    uint16_t getNivel()  const { return currentLevel; }
    uint32_t getExp()    const { return currentExp; }
    uint32_t getExpMax() const { return expParaProximoNivel(); }
};