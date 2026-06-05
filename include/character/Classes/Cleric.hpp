#pragma once

#include <map>

#include "include/Stats/Stats_point.hpp"
#include "include/Systems/Itens/Effects.hpp"
#include "include/Stats/level.hpp"
#include "include/character/Moves/Moves.hpp"
#include "include/Systems/Itens/Armaments.hpp"
#include "include/Systems/Itens/Armors.hpp"
#include "include/Systems/Economy/inventory/myBag.hpp"

/* ═══════════════════════════════════════════════════
   Cleric — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Cleric : virtual  public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          ClStats = {0, 0, 0, 0, 0};
    clericArmament armamento;
    Cleric_armor armadura;
    Inventory bag;

public:
    Cleric() {
        /* aponta para a tabela de moves do Cleric */
        moveTable = &clericMoves;

        ClericSetBase();

        finalStats fs = totalStats::contagem(ClStats, globalRaceStats);
        initHP(fs);
        initMana(fs);
    }

    /* stats base da classe */
    void ClericSetBase() {
        ClStats.VIG += 4;
        ClStats.INT += 1;
    }

    /* level up — atualiza stats e notifica moves novos */
    void gainExp(uint32_t quant) {
        uint16_t nivelAntes = Exp::currentLevel;
        getLvl(quant);

        if (Exp::currentLevel > nivelAntes) {
            upStatusCleric();
            verificarNovosMovesDesbloqueados(Exp::currentLevel);
        }
    }

    void upStatusCleric(){

        ClStats.VIG += 2;
        ClStats.DEF += 1;
        ClStats.VEL += 1;
        ClStats.FOR += 1;
        ClStats.INT += 2;

        finalStats fs = totalStats::contagem(ClStats, globalRaceStats);

        initHP( fs);
        initMana(fs);

        std::cout << "  > Status aumentados\n";

    }

    /* equipar arma */
    bool equipar(cleric_ArmamentID id) {
        return armamento.equip(id);
    }

    bool Armor(Cleric_armorID id) {
        return armadura.equip(id);
    }

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t getCurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }
};