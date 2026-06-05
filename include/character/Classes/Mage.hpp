#pragma once
#include "include/Stats/Stats_point.hpp"
#include "include/Systems/Itens/Effects.hpp"
#include "include/Stats/level.hpp"
#include "include/character/Moves/Moves.hpp"
#include "include/Systems/Itens/Armaments.hpp"
#include "include/Systems/Itens/Armors.hpp"
#include "include/Systems/Economy/inventory/myBag.hpp"

/* ═══════════════════════════════════════════════════
   Mage — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Mage : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          MgStats = {0, 0, 0, 0, 0};
    mageArmament armamento;
    Wizard_armor armadura;
    Inventory bag;

public:
    Mage() {
        /* aponta para a tabela de moves do Mage */
        moveTable = &mageMoves;

        MageSetBase();

        finalStats fs = totalStats::contagem(MgStats, globalRaceStats);
        initHP(fs);
        initMana(fs);
    }

    /* stats base da classe */
    void MageSetBase() {
        MgStats.VIG += 1;
        MgStats.INT += 4;
    }

    /* level up — atualiza stats e notifica moves novos */
    void gainExp(uint32_t quant) {
        uint16_t nivelAntes = Exp::currentLevel;
        getLvl(quant);

        if (Exp::currentLevel > nivelAntes) {
            upStatusMage();
            verificarNovosMovesDesbloqueados(Exp::currentLevel);
        }
    }

    void upStatusMage() {
        MgStats.VIG += 2;
        MgStats.DEF += 1;
        MgStats.VEL += 1;
        MgStats.FOR += 2;
        MgStats.INT += 1;

        finalStats fs = totalStats::contagem(MgStats, globalRaceStats);
        initHP(fs);
        initMana(fs);

        std::cout << "  > Status do Mago aumentados\n";
    }

    /* equipar arma */
    bool equipar(mage_ArmamentID id) {
        return armamento.equip(id);
    }

    bool Armor(Wizard_armorID id){
        return armadura.equip(id);
    }

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }

};