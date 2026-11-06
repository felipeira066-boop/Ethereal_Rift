#pragma once
#include <cstdint>
#include <iostream>
#include "Stats_point.hpp"
#include "Critical.hpp"

class HP : public critical{
    protected:
        int CurrentHP;      // Usamos int para evitar bugs de número negativo
        finalStats myStats; // Esta struct já contém o MaxHP lá dentro

    public:
        // Inicializa o HP usando o valor que vem da struct finalStats
        void initHP(const finalStats &fs) {
            myStats = fs;
            CurrentHP = myStats.MaxHP;
        }

        void takeDamage(const finalStats& attacker, int criticalRateBonus = 0) {
            // Dano do atacante menos o SEU escudo

            finalStats temp = attacker;
            CriticalDamage(temp, criticalRateBonus);

            int FinalDamage = temp.Damage - myStats.Shield;

            if (FinalDamage < 0) FinalDamage = 0;

            CurrentHP -= FinalDamage;

            if (CurrentHP < 0) CurrentHP = 0;

            std::cout << "  Damage: " << FinalDamage << "\n";
        }

        void getHeal(int quantidade) {
            CurrentHP += quantidade;
            if (CurrentHP > (int)myStats.MaxHP)
                CurrentHP = (int)myStats.MaxHP;
            std::cout << "  HP restored! Current: " << CurrentHP << "\n";
        }

        const finalStats &getStats() const { return myStats; }
        const int getHP() const {return CurrentHP;}
        void setShield(std::uint32_t shield) { myStats.Shield = shield; }
        
        // Também é útil ter um método para ler o HP sem alterar
        bool isAlive() const { return CurrentHP > 0; }
};