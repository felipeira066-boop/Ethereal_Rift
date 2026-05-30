#pragma once
#include "library/Stats/Stats_point.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include "library/Stats/level.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Systems/Itens/Armaments.hpp"
#include "library/Systems/Itens/Armors.hpp"
#include "library/Systems/Economy/inventory/myBag.hpp"

/* ═══════════════════════════════════════════════════
   Warrior — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Warrior : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          WaStats = {0, 0, 0, 0, 0};
    warriorArmament armamento;
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

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }

};