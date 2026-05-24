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

enum class  PotionID{
    HEALTH_P    = 1,
    HEALTH_M    = 2,
    HEALTH_G    = 3,
    MANA_P      = 4,
    MANA_M      = 5,
    MANA_G      = 6,
    STRENGTH_P  = 7,
    STRENGTH_M  = 8,
    STRENGTH_G  = 9
};

struct effectsBonus{
    int16_t   DamageBonus;
    int16_t   CrtBonus;
    int16_t   HealAmount;//  EFEITO ESTANTANEO
    int16_t   ShieldBonus;
    int16_t   SpeedBonus;
    int16_t   ManaBonus;// EFEITO INSTANTÂNEO
    int16_t   LeftTurns;    
};

class setEffects :virtual public HP, virtual public Mana{
    protected: 

        struct Effects{ 
            ConsumableID    ID;
            effectsBonus    Bonus;
            std::string     name;
        };

        struct Potion{
            PotionID        ID;
            effectsBonus    Bonus;
            std::string     name;
        };
    
        std::vector<Effects> effectsTable = { 
            /*         id                | dmg | crt | heal | Shld | spd | mana | turn |  name*/
            { ConsumableID::NONE,         { 0,     0,    0,     0,     0,    0,     0},    "Nada"},
            { ConsumableID::APPLE,         {0,     0,    26,    0,     0,    0,     0},    "Maçã"},
            { ConsumableID::MELON_PIE,     {0,     0,    30,    5,     0,    0,     3},    "Torta de Melão"},
            { ConsumableID::ASSASIN_HERB, {18,    10,    0,     0,    12,    0,     3},    "Erva assasina"},
            { ConsumableID::STAR_FRUIT,   {22,     0,    0,     0,     0,   50,     3},    "Fruta das Estrelas"}
        };

        std::vector<Potion> potionTable = {
            /*         id                | dmg | crt | heal | Shld | spd | mana | turn |  name*/
            { PotionID::HEALTH_P,         {0,     0,    20,    0,     0,    0,     0},    "Poção de Vida Pequena"},
            { PotionID::HEALTH_M,         {0,     0,    50,    0,     0,    0,     0},    "Poção de Vida Média"},
            { PotionID::HEALTH_G,         {0,     0,   100,    0,     0,    0,     0},    "Poção de Vida Grande"},
            { PotionID::MANA_P,           {0,     0,     0,    0,     0,   20,     0},    "Poção de Mana Pequena"},
            { PotionID::MANA_M,           {0,     0,     0,    0,     0,   50,     0},    "Poção de Mana Média"},
            { PotionID::MANA_G,           {0,     0,     0,    0,     0,   100,    0},    "Poção de Mana Grande"},
            { PotionID::STRENGTH_P ,      {10,    5,     0,    5 ,   -5 ,   -5 ,   3},    "Poção de Força Pequena"},
            { PotionID::STRENGTH_M ,      {20 ,   10 ,   0 ,   10 , -10 ,   -10 ,   3},    "Poção de Força Média"},
            { PotionID::STRENGTH_G ,      {30 ,   15 ,   0 ,   15 , -15 ,   -15 ,   3},    "Poção de Força Grande"}
        };
    public:
        bool setConsumable( ConsumableID id){
            int effdex = static_cast <int> (id);
            return (effdex <= 0 || effdex > (int)effectsTable.size()) ?  false :  true;
        }

        bool setPotion( PotionID id){
            int effdex = static_cast <int> (id);
            return (effdex <= 0 || effdex > (int)potionTable.size()) ?  false :  true;
        }

        const Effects *getEffect(ConsumableID id) const {
            for (const auto &entry : effectsTable) {
                if (entry.ID == id) return &entry;
            }
            return nullptr;   /* não encontrado */
        }

        const Potion *getPotion(PotionID id) const {
            for (const auto &entry : potionTable) {
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