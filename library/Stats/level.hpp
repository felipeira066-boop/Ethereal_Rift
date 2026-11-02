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
        double exp = 23.0 * std::pow(1.2, currentLevel);
        return static_cast<uint32_t>(exp);
    }

public:

    void initLevel() {
        currentLevel = 1;
        currentExp   = 0;
    }

    uint16_t addExp(uint32_t quant) {
        if (currentLevel >= maxLevel) return 0;

        currentExp += quant;
        const uint16_t previousLevel = currentLevel;

        while (currentExp >= expParaProximoNivel() &&
               currentLevel < maxLevel) {
            currentExp   -= expParaProximoNivel();
            currentLevel += 1;
        }

        return currentLevel - previousLevel;
    }

    /* adiciona EXP e sobe de nível automaticamente */
    void getLvl(uint32_t quant) {
        if (currentLevel >= maxLevel) return;

        const uint16_t previousLevel = currentLevel;
        addExp(quant);
        std::cout << "  + " << quant << " EXP"
                  << " (" << currentExp << "/"
                  << expParaProximoNivel() << ")\n";

        for (uint16_t level = previousLevel; level < currentLevel; ++level) {
            std::cout << "\n  *** LEVEL UP! Nível atual: "
                      << level + 1 << " ***\n";
        }
    }

    bool podeSubirNivel() const {
        return currentLevel < maxLevel;
    }

    uint16_t getNivel()  const { return currentLevel; }
    uint32_t getExp()    const { return currentExp; }
    uint32_t getExpMax() const { return expParaProximoNivel(); }
};