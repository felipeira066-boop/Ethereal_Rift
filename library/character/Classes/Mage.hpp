#pragma once
#include "library/Stats/Stats_point.hpp"
#include "library/Systems/Itens/Effects.hpp"
#include "library/Stats/level.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Systems/Itens/Armaments.hpp"
#include "library/Systems/Itens/Armors.hpp"

/* ═══════════════════════════════════════════════════
   Mage — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Mage : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          MgStats = {0, 0, 0, 0, 0};
    mageArmament armamento;

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

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }
    uint16_t getCurrentMana() const { return CurrentMana; }

    std::map<ConsumableID, int> inventory{  /* inventário de consumíveis (ID e quantidade) */
        { ConsumableID::APPLE,        2 },   /* começa com 2 maçãs */
        { ConsumableID::MELON_PIE,    0 },
        { ConsumableID::ASSASIN_HERB, 0 },
        { ConsumableID::STAR_FRUIT,   5 }
    };

    bool usarConsumivel(ConsumableID id, finalStats &fs, int &critRate) {
        /* verifica se tem no inventário */
        if (inventory.count(id) == 0 || inventory[id] <= 0) {
            std::cout << "  Voce nao tem esse item!\n";
            return false;
        }

        /* busca o efeito na tabela */
        const Effects *eff = getEffect(id);
        if (!eff) return false;

        /* aplica o efeito */
        EffectAplication(eff->Bonus, fs, critRate, *this, *this);

        /* desconta do inventário */
        inventory[id]--;
        std::cout << "  Usou: " << eff->name
                  << " — restam " << inventory[id] << "\n";
        return true;
    }

    /* exibe inventário atual */
    void exibirinventory() const {
        std::cout << "\n  inventário:\n";
        for (const auto &par : inventory) {
            const Effects *eff = getEffect(par.first);
            if (eff && par.second > 0)
                std::cout << "  - " << eff->name
                          << " x" << par.second << "\n";
        }
    }
};