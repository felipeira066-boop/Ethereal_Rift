#pragma once
#include "library/Stats/Stats_point.hpp"
#include "library/Stats/Effects.hpp"
#include "library/Stats/level.hpp"
#include "library/character/Moves/Moves.hpp"
#include "library/Systems/Itens/Armaments.hpp"
#include "library/Systems/Itens/Armors.hpp"

/* ═══════════════════════════════════════════════════
   Hunter — herda HP, Mana, Exp e MoveSystem
═══════════════════════════════════════════════════ */

class Hunter : virtual public HP, virtual public Mana, public Exp, public MoveSystem, public setEffects {
private:
    stats          HuStats = {0, 0, 0, 0, 0};
    hunterArmament armamento;

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

    int      getMaxHP()       const { return (int)myStats.MaxHP; }
    uint16_t CurrentLevel()const { return Exp::currentLevel; }


    std::map<ConsumableID, int> inventory{  /* inventário de consumíveis (ID e quantidade) */
        { ConsumableID::APPLE,        2 },   /* começa com 2 maçãs */
        { ConsumableID::MELON_PIE,    0 },
        { ConsumableID::ASSASIN_HERB, 0 },
        { ConsumableID::STAR_FRUIT,   0 }
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