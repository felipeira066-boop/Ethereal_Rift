#pragma once
#include "library/Stats/Stats_point.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include "library/Stats/level.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Systems/Itens/Armaments.hpp"
#include "library/Systems/Itens/Armors.hpp"
#include "library/Systems/Economy/inventory/myBag.hpp"

/* ═══════════════════════════════════════════════════
   Hunter — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Hunter : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          HuStats = {0, 0, 0, 0, 0};
    hunterArmament armamento;
    Arrow_armor   armadura;
    Inventory bag;

public:
    Hunter() {
        /* aponta para a tabela de moves do Hunter */
        moveTable = &hunterMoves;

        HunterSetBase();

        finalStats fs = totalStats::contagem(HuStats, globalRaceStats);
        initHP(fs);
        initMana(fs);
    }

    /* stats base da classe */
    void HunterSetBase() {
        HuStats.VEL += 3;
        HuStats.FOR += 2;
    }

    /* level up — atualiza stats e notifica moves novos */
    void gainExp(uint32_t quant) {
        uint16_t nivelAntes = Exp::currentLevel;
        getLvl(quant);

        if (Exp::currentLevel > nivelAntes) {
            upStatusHunter();
            verificarNovosMovesDesbloqueados(Exp::currentLevel);
        }
    }

    void upStatusHunter(){

        HuStats.VIG += 1;
        HuStats.DEF += 1;
        HuStats.VEL += 2;
        HuStats.FOR += 2;
        HuStats.INT += 1;

        finalStats fs = totalStats::contagem(HuStats, globalRaceStats);

        initHP( fs);
        initMana(fs);

        std::cout << "  > Status aumentados\n";

    }

    /* equipar arma */
    bool equipar(hunter_ArmamentID id) {
        return armamento.equip(id);
    }

    bool Armor(Arrow_armorID id){
        return armadura.equip(id);
    }

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }

};
