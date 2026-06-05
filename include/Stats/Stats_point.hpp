#pragma once

#include <cstdint>
#include <iostream>

class HP;
class Mana;

 struct stats{
    int VIG;
    int DEF;
    int VEL;
    int FOR;
    int INT;
};

 struct raceStats{
    int bonus_vig;
    int bonus_def;
    int bonus_vel;
    int bonus_for;
    int bonus_int;
};

struct finalStats{
    uint32_t MaxHP;
    uint32_t Shield;
    uint32_t Speed;
    uint32_t Damage;
    uint32_t MaxMana;
};

extern stats globalStats;
extern raceStats globalRaceStats;

class totalStats{
    public:
        static finalStats contagem(const stats& st, const raceStats& rs){
            finalStats fs;

            fs.MaxHP = (st.VIG + rs.bonus_vig + 10) * 2;
            fs.Shield = st.DEF + rs.bonus_def;
            fs.Speed = st.VEL + rs.bonus_vel + 1;    
            fs.Damage = ((st.FOR + rs.bonus_for) + (st.INT + rs.bonus_int));
            fs.MaxMana = (st.INT + rs.bonus_int + 10) * 5;

            return fs;
        }
};
