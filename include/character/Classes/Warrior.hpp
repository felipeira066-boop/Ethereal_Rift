#pragma once
#include "include/Stats/Stats_point.hpp"
#include "include/Systems/Itens/Effects.hpp"
#include "include/Stats/level.hpp"
#include "include/character/Moves/Moves.hpp"
#include "include/Systems/Itens/Armaments.hpp"
#include "include/Systems/Itens/Armors.hpp"
#include "include/Systems/Economy/inventory/myBag.hpp"

/* ═══════════════════════════════════════════════════
   Warrior — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Warrior : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          WaStats = {0, 0, 0, 0, 0};
    warriorArmament armamento;
    Warrior_armor armadura;
    Inventory bag;

public:
    Warrior() {
        /* aponta para a tabela de moves do Warrior */
        moveTable = &warriorMoves;

        WarriorSetBase();

        finalStats fs = totalStats::contagem(WaStats, globalRaceStats);
        initHP(fs);
        initMana(fs);
    }

    /* stats base da classe */
    void WarriorSetBase() {
        WaStats.DEF += 2;
        WaStats.FOR += 3;
    }

    /* level up — atualiza stats e notifica moves novos */
    void gainExp(uint32_t quant) {
        uint16_t nivelAntes = Exp::currentLevel;
        getLvl(quant);

        if (Exp::currentLevel > nivelAntes) {
            upStatusWarrior();
            verificarNovosMovesDesbloqueados(Exp::currentLevel);
        }
    }

    void upStatusWarrior() {
        WaStats.VIG += 2;
        WaStats.DEF += 1;
        WaStats.VEL += 1;
        WaStats.FOR += 2;
        WaStats.INT += 1;

        finalStats fs = totalStats::contagem(WaStats, globalRaceStats);
        initHP(fs);
        initMana(fs);

        std::cout << "  > Status do Warrior aumentados\n";

    }

    /* equipar arma */
    bool equipar(warrior_ArmamentID id) {
        return armamento.equip(id);
    }

    bool Armor(Warrior_armorID id){
        return armadura.equip(id);
    }

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }

};