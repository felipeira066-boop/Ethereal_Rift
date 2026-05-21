#pragma once

#include "library/Stats/Critical.hpp"
#include "library/Stats/HP.hpp"
#include "library/Stats/Mana.hpp"
#include <cstdint>
#include <vector>
#include <string>

enum class  ConsumableID{
    NONE = 0,
    APPLE = 1,
    MELON_PIE = 2,
    ASSASIN_HERB = 3,
    STAR_FRUIT = 4
};

struct effectsBonus{
    uint16_t   DamageBonus;
    uint16_t   CrtBonus;
    uint16_t   HealAmount;//  EFEITO ESTANTANEO
    uint16_t   ShieldBonus;
    uint16_t   SpeedBonus;
    uint16_t   ManaBonus;// EFEITO INSTANTÂNEO
    uint16_t   LeftTurns;    
};

class setEffects :virtual public HP, virtual public Mana{
    protected: 

        struct Effects{ 
            ConsumableID    ID;
            effectsBonus    Bonus;
            std::string     name;
        };

    
        std::vector<Effects> effectsTable{ 
            /*         id                | dmg | crt | heal | Shld | spd | mana | turn |  name*/
            { ConsumableID::NONE,         { 0,     0,    0,     0,     0,    0,     0},    "Nada"},
            { ConsumableID::APPLE,         {0,     0,    26,    0,     0,    0,     0},    "Maçã"},
            { ConsumableID::MELON_PIE,     {0,     0,    30,    5,     0,    0,     3},    "Torta de Melão"},
            { ConsumableID::ASSASIN_HERB, {18,    10,    0,     0,    12,    0,     3},    "Erva assasina"},
            { ConsumableID::STAR_FRUIT,   {22,     0,    0,     0,     0,   50,     3},    "Fruta das Estrelas"}
        };
    public:
        bool setConsumable( ConsumableID id){
            int effdex = static_cast <int> (id);
            return (effdex <= 0 || effdex > (int)effectsTable.size()) ?  false :  true;

        }

        const Effects *getEffect(ConsumableID id) const {
            for (const auto &entry : effectsTable) {
                if (entry.ID == id) return &entry;
            }
            return nullptr;   /* não encontrado */
        }

    /*---------poções de efeitos diversos -----------------*/
       
    void EffectAplication(const effectsBonus &eff, finalStats &fs, int &critical_rate, HP &hp, Mana &mn) {

        /* efeitos instantâneos — aplicar agora */
        if (eff.HealAmount > 0) {
            getHeal(eff.HealAmount);
        }

        if (eff.ManaBonus > 0) {
            getMana(eff.ManaBonus, fs.MaxMana);
        }

        /* efeitos temporários — só aplicar se LeftTurns > 0 */
        if (eff.LeftTurns > 0) {
            if (eff.ShieldBonus > 0)  fs.Shield       += eff.ShieldBonus;
            if (eff.DamageBonus > 0)  fs.Damage       += eff.DamageBonus;
            if (eff.SpeedBonus > 0)   fs.Speed        += eff.SpeedBonus;
            if (eff.CrtBonus > 0)     critical_rate       += eff.CrtBonus;
        }
    }
};          